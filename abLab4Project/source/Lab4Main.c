/*******************************************************************************
* EECE344 Lab 4 Code
*	This program allows the user to activate and deactivate an alarm tone
* August Byrne, 11/12/2020
*******************************************************************************/
#include "MCUType.h"               /* Include header files                    */
#include "MemTest.h"
//#include "BasicIO.h"
#include "K65TWR_ClkCfg.h"
//#include "MK65F18.h"
#include "K65TWR_GPIO.h"
#include "Key.h"
#include "LCD.h"
#include "SysTickDelay.h"

#define POLL_PERIOD 10

void ControlDisplayTask(void);

static INT8U ALARM_STATE = 0;	//initial state is alarm off
static INT8U PAST_STATE = 1;

void main(void){
    K65TWR_BootClock();             /* Initialize MCU clocks                  */
    BIOOpen(BIO_BIT_RATE_115200);   /* Initialize Serial Port                 */
    SysTickDlyInit();
    GpioDBugBitsInit();
    LcdDispInit();

    INT32U low_addr = 0x00000000;		//Received low address
    INT32U high_addr = 0x001FFFFF;		//Received high address
	BIOPutStrg("CS: ");					//program start initial checksum
	BIOOutHexWord(low_addr);
	BIOPutStrg("-");
	BIOOutHexWord(high_addr);
	BIOPutStrg("  ");
	INT16U math_val = CalcChkSum((INT8U *)low_addr,(INT8U *)high_addr);
	BIOOutHexHWord(math_val);
	BIOPutStrg("\n\r");

    while(1){
    	SysTickWaitEvent(POLL_PERIOD);
    	ControlDisplayTask();
    	//AlarmWaveControlTask();
    	KeyTask();
    }
}

void ControlDisplayTask(void){
	DB1_TURN_ON();
	current_key = KeyScan();
	switch (ALARM_STATE){
	case 0:
		if (PAST_STATE != ALARM_STATE){		//display "alarm off" on the lcd
			LcdDispString("ALARM OFF");
			PAST_STATE = ALARM_STATE;
		}
		if (current_key == 0x04){			//if a is pressed, set alarm state as on
			ALARM_STATE = 1;
		}
											//alarm signal is off
		break;
	case 1:
		if (PAST_STATE != ALARM_STATE){		//display "alarm on" on the lcd
			LcdDispString("ALARM ON");
			PAST_STATE = ALARM_STATE;
		}
		if (current_key == 0x10){			//if d is pressed, set alarm state as off
			ALARM_STATE = 0;
		}
		//AlarmWaveControlTask();			//alarm signal is on
		break;
	default:
		ALARM_STATE = 0;
	}
	DB1_TURN_OFF();
}
