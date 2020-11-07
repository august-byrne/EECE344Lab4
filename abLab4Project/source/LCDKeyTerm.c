/********************************************************************
* LCDKeyTerm.c - A simple keypad demonstration program. 
* It reads a keypad by using the key.c module and outputs the key
* pressed to the LCD display using LCD.c. 
* This version is dependent on SysTickDelay.c, LCD.c, and Key.c
* 
* TDM, 11/03/2015
* Todd Morton, Modified for MCUXpresso, 10/29/2018
*********************************************************************
* Include Project Master Header File
********************************************************************/
#include "MCUType.h"
#include "LCD.h"
#include "Key.h"
#include "SysTickDelay.h"
#include "K65TWR_GPIO.h"
#include "K65TWR_ClkCfg.h"

#define SLICE_PER (INT8U)10
#define LINELENGTH (INT8U)16
#define NUMLINES (INT8U)2

/*************************************************************************
* Global variable and function prototypes
*************************************************************************/
static void DispKeyTask(void);
/*************************************************************************
 * main() - Initialization and simple timeslicer to read keypad and
 *          display on LCD.
 * Todd Morton, 01/22/2014
 * **********************************************************************/
void main(void) {
    
    K65TWR_BootClock();
    GpioDBugBitsInit();
    SysTickDlyInit();
    KeyInit();
    LcdDispInit();
    LcdDispClear();
    LcdCursorMove(1,1);
    LcdCursorMode(1,1);
    while(1){
        SysTickWaitEvent(SLICE_PER);
        KeyTask();
        DispKeyTask();
    }
}

/*************************************************************************
* DispKeyTask() - Initializes the display lines if needed then displays a 
*                 key, if pressed.
*
* Todd Morton, 01/23/2014
*************************************************************************/
static void DispKeyTask(void){
    INT8C key;
    static INT8U currentcol = 1;
    static INT8U currentlin = 1;
    DB2_TURN_ON();
    /* Control cursor */
    if((currentcol > 16) && (currentlin == 1)){
        currentcol = 1;
        currentlin = 2;
        LcdDispLineClear(2);
    }else if ((currentcol > 16) && (currentlin == 2)){
        currentcol = 1;
        currentlin = 1;
        LcdDispLineClear(1);
    }else{} /* Do nothing */
    /* Check for keypress and display */
    key = KeyGet();
    switch(key){
    case '\0':
        break;
    case DC1: /* Define action for DC1 key */
        LcdDispChar('A');
        currentcol++;
        break;
    case DC2: /* Define action for DC2 key */
        LcdDispChar('B');
        currentcol++;
        break;
    case DC3: /* Define action for DC3 key */
        LcdDispChar('C');
        currentcol++;
        break;
    case DC4: /* Define action for DC4 key */
        LcdDispChar('D');
        currentcol++;
        break;
    default:
        LcdDispChar(key);
        currentcol++;
        break;
    }
    DB2_TURN_OFF();
}
/*******************************************************************/

