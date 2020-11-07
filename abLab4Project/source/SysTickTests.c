/********************************************************************
* SysTickTests - Misc tests for the SysTickDelay module
* 
* TDM, 10/23/2018
* TDM, modified for latest SysTickDelay module, 10/31/20
*********************************************************************
* Include Header Files
********************************************************************/
#include "MCUType.h"
#include "K65TWR_ClkCfg.h"
#include "SysTickDelay.h"
#include "K65TWR_GPIO.h"

#define SLICE_PER (INT8U)50

/*************************************************************************
* Global variable and function prototypes
*************************************************************************/
static void DlyTask(void);
static void UpDTask(void);
/*************************************************************************
 * main() - Initialization and simple timeslicer.
 * Todd Morton, 01/22/2014
 * *************************************************************************************/
void main(void) {
    
	K65TWR_BootClock();
    GpioDBugBitsInit();
    SysTickDlyInit();
    while(1){
    	DB0_TURN_ON();
//    	__WFI();						//WFI test, did not measure power
    	SysTickWaitEvent(SLICE_PER);	//SysTickWaitEvent() test
    	DB0_TURN_OFF();
        DlyTask();
        UpDTask();
    }
}

/****************************************************************************************
*
****************************************************************************************/
static void DlyTask(void){
	INT32U curr_task_cnt;
	static INT32U last_task_cnt = 0;

	curr_task_cnt = SysTickGetSliceCount();
	if((curr_task_cnt - last_task_cnt) >= 10){
		DB1_TURN_ON();
		last_task_cnt = curr_task_cnt;
		SysTickDelay(10);	//SysTickDelay() Test
//		start_cnt = SysTickGetmsCount();
//		while(SysTickGetmsCount() < (start_cnt + 10)){}	//SysTckGetmsCount() test
		DB1_TURN_OFF();

	}else{}
}

/****************************************************************************************
*
****************************************************************************************/
static void UpDTask(void){
	static INT32U upd_task_cntr = 0;

	upd_task_cntr++;
	if(upd_task_cntr >= 5){
		DB2_TURN_ON();
		upd_task_cntr = 0;
		SysTickDelay(10);	//SysTickDelay() Test
		DB2_TURN_OFF();

	}else{}
}

