/*******************************************************************
** LcdDemo.c
*
*    DESCRIPTION: A demonstration/test of the Lcd.c module.
*
*    AUTHOR: Todd Morton
*
*    HISTORY: 02/22/1999 Originally written by Kiet Tran
*             04/15/2000 Revised by Todd Morton    
*
********************************************************************
* Master Include File  
*******************************************************************/
#include "MCUType.h"
#include "LCD.h"
#include "SysTickDelay.h"
#include "K65TWR_ClkCfg.h"

/********************************************************************
* Local Definitions                                                 *
********************************************************************/

/********************************************************************
* Local Macros
********************************************************************/

/********************************************************************
* Variables and Stored Constants
********************************************************************/
INT8U Count;
/********************************************************************
* Main Function
********************************************************************/

void main(void){

    K65TWR_BootClock();
    SysTickDlyInit();
    LcdDispInit();

    while(1){
        LcdDispClear();
        LcdCursorMove(1,1);
        LcdDispString("Hi Prof. Morton!");
        SysTickDelay(1500);
        LcdDispLineClear(1);
        LcdDispString("DispChar");
        LcdDispLineClear(2);
        LcdDispChar('c');
        SysTickDelay(1500);
        LcdDispLineClear(1);
        LcdDispString("HexWord");
        for(INT8U i = 0; i<9;i++){
            LcdDispLineClear(2);
            LcdDispHexWord(0xFEDCBA98, i);
            SysTickDelay(1000);
        }
        SysTickDelay(1500);
        LcdDispLineClear(1);
        LcdDispString("DecWord LZ");

        LcdDispLineClear(2);
        LcdDispDecWord(123, 4, LCD_DEC_MODE_LZ);
        LcdCursorMove(2,7);
        LcdDispDecWord(12345, 4, LCD_DEC_MODE_LZ);
        LcdCursorMove(2,13);
        LcdDispDecWord(0, 4, LCD_DEC_MODE_LZ);
        SysTickDelay(1500);
        LcdDispLineClear(1);
        LcdDispString("DecWord AR");

        LcdDispLineClear(2);
        LcdDispDecWord(123, 4, LCD_DEC_MODE_AR);
        LcdCursorMove(2,7);
        LcdDispDecWord(12345, 4, LCD_DEC_MODE_AR);
        LcdCursorMove(2,13);
        LcdDispDecWord(0, 4, LCD_DEC_MODE_AR);
        SysTickDelay(1500);

        LcdDispLineClear(1);
        LcdDispString("DecWord AL");
        LcdDispLineClear(2);
        LcdDispDecWord(123, 4, LCD_DEC_MODE_AL);
        LcdCursorMove(2,7);
        LcdDispDecWord(12345, 4, LCD_DEC_MODE_AL);
        LcdCursorMove(2,13);
        LcdDispDecWord(0, 4, LCD_DEC_MODE_AL);
        SysTickDelay(1500);

        LcdDispLineClear(1);
        LcdDispString("Cursor 0,0");
        LcdDispLineClear(2);
        LcdCursorMode(0,0);
        SysTickDelay(1500);

        LcdDispLineClear(1);
        LcdDispString("Cursor 0,1");
        LcdDispLineClear(2);
        LcdCursorMode(0,1);
        SysTickDelay(1500);

        LcdDispLineClear(1);
        LcdDispString("Cursor 1,0");
        LcdDispLineClear(2);
        LcdCursorMode(1,0);
        SysTickDelay(1500);

        LcdDispLineClear(1);
        LcdDispString("Cursor 1,1");
        LcdDispLineClear(2);
        LcdCursorMode(1,1);

        SysTickDelay(5000);
        Count++;
    }
}
