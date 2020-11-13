/*******************************************************************************
* EECE344 Lab 4 Code
*	This program allows the user to activate and deactivate an alarm tone
* August Byrne, 11/12/2020
*******************************************************************************/
#include "MCUType.h"               /* Include header files                    */
#include "MemTest.h"
#include "K65TWR_ClkCfg.h"
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
    SysTickDlyInit();
    GpioDBugBitsInit();
    LcdDispInit();
    KeyInit();

	LcdCursorMove(2,1);					//program start initial checksum
    INT32U low_addr = 0x00000000;		//Received low address
    INT32U high_addr = 0x001FFFFF;		//Received high address
	INT16U math_val = CalcChkSum((INT8U *)low_addr,(INT8U *)high_addr);
	LcdDispString("CS: ");
	LcdDispHexWord(math_val,4);
	LcdCursorMove(1,1);

    while(1){
    	SysTickWaitEvent(POLL_PERIOD);
    	ControlDisplayTask();
    	//AlarmWaveControlTask();
    	KeyTask();
    }
}

void ControlDisplayTask(void){
	DB1_TURN_ON();
	switch (ALARM_STATE){
	case 0:
		if (PAST_STATE != ALARM_STATE){		//display "alarm off" on the lcd
			LcdDispLineClear(1);
			LcdDispString("ALARM OFF");
			PAST_STATE = ALARM_STATE;
		}
		if (KeyGet() == DC1){			//if a is pressed, set alarm state as on
			ALARM_STATE = 1;
		}
											//alarm signal is off
		break;
	case 1:
		if (PAST_STATE != ALARM_STATE){		//display "alarm on" on the lcd
			LcdDispLineClear(1);
			LcdDispString("ALARM ON");
			PAST_STATE = ALARM_STATE;
		}
		if (KeyGet() == DC4){			//if d is pressed, set alarm state as off
			ALARM_STATE = 0;
		}
		//AlarmWaveControlTask();			//alarm signal is on, so make noise
		break;
	default:
		ALARM_STATE = 0;
	}
	DB1_TURN_OFF();
}
