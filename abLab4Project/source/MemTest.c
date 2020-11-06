/*
 * MemTest.c
 *	This module takes in a start and end address, and calculates
 *	the checksum of all of the values between the two supplied addresses.
 *	It also accounts for the terminal count bug.
 *  Created on: Oct 18, 2020
 *      Author: August
 */

#include "MCUType.h"               /* Include header files                    */
#include "MemTest.h"

INT16U CalcChkSum(INT8U *startaddr, INT8U *endaddr);

INT16U CalcChkSum(INT8U *startaddr, INT8U *endaddr){

	INT16U check_sum = 0;
	INT32U terminal_check = (INT32U) startaddr;
	if (terminal_check != 0xFFFFFFFF){
	while (startaddr <= endaddr){
		check_sum += *startaddr;	//add all of the data in the addresses listed
		startaddr ++;				//make the pointer point to the next address
	}
	}else{
		check_sum = *startaddr;
	}
	return check_sum;
}
