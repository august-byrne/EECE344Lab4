/*******************************************************************************
* EECE344 Lab 4 Code
*	This program allows the user to activate and deactivate an alarm tone
* August Byrne, 11/15/2020
*******************************************************************************/
#include "MCUType.h"               /* Include header files                    */
#include "MemTest.h"
#include "K65TWR_ClkCfg.h"
#include "K65TWR_GPIO.h"
#include "Key.h"
#include "LCD.h"
#include "SysTickDelay.h"

#define POLL_PERIOD 10

static void ControlDisplayTask(void);

typedef enum {ALARM_OFF, ALARM_ON} ALARM_STATE;
static ALARM_STATE CurrentAlarmState = ALARM_OFF;	//initial state is alarm off
static ALARM_STATE PreviousAlarmState = ALARM_ON;

void main(void){
	const INT32U low_addr = 0x00000000;			//low memory address
	const INT32U high_addr = 0x001FFFFF;		//high memory address
	K65TWR_BootClock();             /* Initialize MCU clocks                  */
	SysTickDlyInit();
	GpioDBugBitsInit();
	LcdDispInit();
	KeyInit();

	LcdCursorMove(2,1);					//program start initial checksum
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
	switch (CurrentAlarmState){
	case ALARM_OFF:
		if (PreviousAlarmState != CurrentAlarmState){		//display "alarm off" on the lcd
			LcdDispLineClear(1);
			LcdDispString("ALARM OFF");
			PreviousAlarmState = CurrentAlarmState;
		}
		if (KeyGet() == DC1){			//if a is pressed, set alarm state as on
			CurrentAlarmState = ALARM_ON;
		}
											//alarm signal is off
		break;
	case ALARM_ON:
		if (PreviousAlarmState != CurrentAlarmState){		//display "alarm on" on the lcd
			LcdDispLineClear(1);
			LcdDispString("ALARM ON");
			PreviousAlarmState = CurrentAlarmState;
		}
		if (KeyGet() == DC4){			//if d is pressed, set alarm state as off
			CurrentAlarmState = ALARM_OFF;
		}
		//AlarmWaveControlTask();			//alarm signal is on, so make noise
		break;
	default:
		CurrentAlarmState = ALARM_OFF;
		PreviousAlarmState = ALARM_ON;
	}
	DB1_TURN_OFF();
}
