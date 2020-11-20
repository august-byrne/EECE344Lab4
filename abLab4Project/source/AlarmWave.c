/*
 * AlarmWave.c
 *
 *  Created on: Nov 18, 2020
 *      Author: August
 */

#include "MCUType.h"
#include "AlarmWave.h"
#include "K65TWR_GPIO.h"

void AlarmWaveInit(void);
void AlarmWaveControlTask(void);
void AlarmWaveSetMode(void);
void PIT0_IRQHandler(void);

static const INT16U sineSamples[64] = {0x8000,0x8C8B,0x98F8,0xA528,0xB0FB,0xBC56,0xC71C,0xD133,0xDA82,0xE2F2,0xEA6D,0xF0E2,0xF641,0xFA7D,0xFD8A,0xFF62,0xFFFF,0xFF62,0xFD8A,0xFA7D,0xF641,0xF0E2,0xEA6D,0xE2F2,0xDA82,0xD133,0xC71C,0xBC56,0xB0FB,0xA528,0x98F8,0x8C8B,0x8000,0x7374,0x6707,0x5AD7,0x4F04,0x43A9,0x38E3,0x2ECC,0x257D,0x1D0D,0x1592,0xF1D,0x9BE,0x582,0x275,0x9D,0x0,0x9D,0x275,0x582,0x9BE,0xF1D,0x1592,0x1D0D,0x257D,0x2ECC,0x38E3,0x43A9,0x4F04,0x5AD7,0x6707,0x7374};
//static INT8U PitEventFlag = 0;
static INT8U oneSec = 0;
static INT8U toggleMode = 0;

void AlarmWaveInit(void){
	SIM->SCGC6 |= SIM_SCGC6_PIT(1);
	PIT->MCR = PIT_MCR_MDIS(0);
	PIT->CHANNEL[0].LDVAL = 19199;
	//INITIALIZE DAC (set these three bits to enable software dac usage with ref. voltage 2 (Vcc))
	DAC0->C0 = (DAC_C0_DACRFS(1) | DAC_C0_DACTRGSEL(1) | DAC_C0_DACEN(1));

	// enable ints and start Timer 0
	PIT->CHANNEL[0].TCTRL = (PIT_TCTRL_TIE(1)|PIT_TCTRL_TEN(1));
	//Enable PIT interrupt
	NVIC_EnableIRQ(PIT0_IRQn);
}

void AlarmWaveControlTask(void){
	while(PitEventFlag == 0x00){} // wait for the timer flag
	PitEventFlag = 0;

}

void AlarmWaveSetMode(void){
	if (toggleMode == 1){
		//WRITE SINE WAVE TO DAC
		DAC0->DAT[0].DATL = (INT8U)((sineSamples[PitEventFlag]>>4) & 0xff);
		DAC0->DAT[0].DATH = (INT8U)(sineSamples[PitEventFlag]>>12);
	}else if(toggleMode == 0){
		//WRITE 1.6V TO DAC
		DAC0->DAT[0].DATL = (INT8U)((0x8000>>4) & 0xff);
		DAC0->DAT[0].DATH = (INT8U)(0x8000>>12);
	}
}

void PIT0_IRQHandler(void){
	DB4_TURN_ON();
	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF(1);
	PitEventFlag++;

	if(PitEventFlag >= 63){
		PitEventFlag = 0;
		oneSec++;
	}
	if(oneSec >= 64){
		toggleMode = !toggleMode;
		oneSec = 0;
	}

	AlarmWaveSetMode();

	DB4_TURN_OFF();
}
