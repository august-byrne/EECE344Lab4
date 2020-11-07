/*******************************************************************************
* EECE344 Lab 3 Code
*	This program allows the user to choose between three types of counters
*	and use them with SW2 (switch 2 on our board). The user can also quit out
*	of any of the types with q, and try a different one.
* August Byrne, 10/30/2020
*******************************************************************************/
#include "MCUType.h"               /* Include header files                    */
#include "MemTest.h"
#include "BasicIO.h"
#include "K65TWR_ClkCfg.h"
#include "MK65F18.h"
#include "K65TWR_GPIO.h"

#define STRGLEN 2

void PORTA_IRQHandler(void);

typedef enum {SW_CNT, HW_CNT, CO_CNT} COUNT_STATES;
COUNT_STATES CountingState;

INT16U PulseCnt;

void main(void){

	INT8C in_strg[STRGLEN];			//a character array representing a string
    INT8C char_in;					//Received character
    INT8U in_err;					//Input error
    INT8U is_valid;					//must be 1 in order to move onto the next step in the program
    INT32U low_addr = 0x00000000;				//Received low address
    INT32U high_addr = 0x001FFFFF;				//Received high address

    K65TWR_BootClock();             /* Initialize MCU clocks                  */
    BIOOpen(BIO_BIT_RATE_115200);   /* Initialize Serial Port                 */

	BIOPutStrg("CS: ");				//program start initial checksum
	BIOOutHexWord(low_addr);
	BIOPutStrg("-");
	BIOOutHexWord(high_addr);
	BIOPutStrg("  ");
	INT16U math_val = CalcChkSum((INT8U *)low_addr,(INT8U *)high_addr);
	BIOOutHexHWord(math_val);
	BIOPutStrg("\n\r");

    while(1){
        is_valid = 0;
    	while (is_valid == 0){
    		BIOPutStrg("\n\rEnter the type of PulseCnt you want to implement\n\r(Enter s, h , or c for software, hardware, or combination counters respectively)\n\r");	//prompt message
    		in_err = BIOGetStrg(STRGLEN,in_strg);		//receiving the PulseCnt type
    		if(in_err == 0){
    			switch(in_strg[0]){
    			case 's':
    				is_valid = 1;
    				CountingState = SW_CNT;
    				break;
    			case 'h':
    				is_valid = 1;
    				CountingState = HW_CNT;
    				break;
    			case 'c':
    				is_valid = 1;
    				CountingState = SW_CNT;
    				break;
    			default:
    				BIOPutStrg("\n\rThat input is not an option. Try Again\n\r");	//prompt message
    			}
    		}else{
    			BIOPutStrg("\n\rThat input is not an option; to many characters. Try Again\n\r");	//prompt message
    		}
    	}
    	char_in = 'a';						//an unused value to initialize with
        switch(CountingState){
        case SW_CNT:
        	PulseCnt = 0;
        	GpioSw2Init(PORT_IRQ_OFF);
        	INT8U last_in;
    		last_in &= ~GPIO_PIN(SW2_BIT); //preset to zero so a 1 is required to start
    		INT8U cur_sig;
    		BIOPutStrg("\r\r\r");
    		BIOOutDecWord(PulseCnt,3,BIO_OD_MODE_LZ);
        	while('q' != char_in){
        		char_in = BIORead();
        		//poll for button state
        		cur_sig = SW2_INPUT;
        		if((cur_sig == 0x00) && (last_in == GPIO_PIN(SW2_BIT))) { //falling-edge
        		PulseCnt++;
    			if (PulseCnt > 999){
    				PulseCnt = 0;
    			}
        		BIOPutStrg("\r\r\r");
        		BIOOutDecWord(PulseCnt,3,BIO_OD_MODE_LZ);
        		}
        		last_in = cur_sig;
        	}
        	break;
        case HW_CNT:
        	PulseCnt = 0;
        	INT8U CountOld = 0;
        	NVIC_ClearPendingIRQ(PORTA_IRQn);
        	NVIC_EnableIRQ(PORTA_IRQn);
    		GpioSw2Init(PORT_IRQ_FE); //initialize SW2 to detect a falling edge
    		SW2_CLR_ISF(); // clear interrupt flag
    		BIOPutStrg("\r\r\r");
    		BIOOutDecWord(PulseCnt,3,BIO_OD_MODE_LZ);
        	while('q' != char_in){
        		char_in = BIORead();
        		if(CountOld != PulseCnt){
        			if (PulseCnt > 999){
        				PulseCnt = 0;
        			}
        			CountOld = PulseCnt;
            		BIOPutStrg("\r\r\r");
            		BIOOutDecWord(PulseCnt,3,BIO_OD_MODE_LZ);
        		}
        	}
        	GpioSw2Init(PORT_IRQ_OFF);
        	break;
        case CO_CNT:
        	PulseCnt = 0;
    		GpioSw2Init(PORT_IRQ_FE); //initialize SW2 to detect a falling edge
    		SW2_CLR_ISF(); // clear interrupt flag
    		BIOPutStrg("\r\r\r");
    		BIOOutDecWord(PulseCnt,3,BIO_OD_MODE_LZ);
        	while('q' != char_in){
        		char_in = BIORead();
        		if(SW2_ISF != 0){ //check flag
        			SW2_CLR_ISF(); // Clear flag
        			PulseCnt++;
        			if (PulseCnt > 999){
        				PulseCnt = 0;
        			}
        			BIOPutStrg("\r\r\r");
            		BIOOutDecWord(PulseCnt,3,BIO_OD_MODE_LZ);
        		}
        	}
        	GpioSw2Init(PORT_IRQ_OFF);
        	break;
        default:
        	BIOPutStrg("\nThis input broke the program!\n\r");	//prompt message
        }
    }
}

void PORTA_IRQHandler(void){
if(SW2_ISF != 0x00){
SW2_CLR_ISF();
PulseCnt++;
}else{
//some other PORTA bit was set
}
}

