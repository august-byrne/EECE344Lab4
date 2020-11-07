/*******************************************************************************
* EECE344 Lab 4 Code
*	This program allows the user to do things
* August Byrne, 11/6/2020
*******************************************************************************/
#include "MCUType.h"               /* Include header files                    */
//#include "MemTest.h"
#include "BasicIO.h"
#include "K65TWR_ClkCfg.h"
//#include "MK65F18.h"
#include "K65TWR_GPIO.h"
#include "Key.h"

#include "SysTickDelay.h"

#define POLL_PERIOD 10

void ControlDisplayTask(void);

void main(void){

    K65TWR_BootClock();             /* Initialize MCU clocks                  */
    BIOOpen(BIO_BIT_RATE_115200);   /* Initialize Serial Port                 */
    SysTickDlyInit();
    GpioDBugBitsInit();
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

	DB1_TURN_OFF();
}
