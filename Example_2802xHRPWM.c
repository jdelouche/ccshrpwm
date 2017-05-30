//#############################################################################
//
//  File:   f2802x_examples_ccsv4/hrpwm/Example_2802xHRPWM.c
//
//  Title:  F2802x Device HRPWM example
//
//  Group:          C2000
//  Target Device:  TMS320F2802x
//
//! <h1> Full bridge management</h1>
//! <h2>Start/Stop Button = GPIO12</h2>
//! <h2>Battery detection = GPIO19 pin J2 2</h2>
//! <h2>Wake        LED GPIO0</h2>
//! <h2>Start Hr    LED GPIO1</h2>
//! <h2>Start       LED GPIO2</h2>
//! <h2>Stop        LED GPIO3</h2>
//! <h2>EPWM3A High1A - GPIO4 pin J6 5</h2>
//! <h2>EPWM3B Low1A  - GPIO5 pin J6 6</h2>
//! <h2>EPWM4A High2A - GPIO6 pin J2 8</h2>
//! <h2>EPWM4B Low2B  - GPIO7 pin J2 9</h2>
//!
//#############################################################################
// $TI Release: LaunchPad f2802x Support Library v100 $
// $Release Date: Wed Jul 25 10:45:39 CDT 2012 $
//#############################################################################


#include "DSP28x_Project.h"         // DSP2802x Headerfile

#include "f2802x_common/include/clk.h"
#include "f2802x_common/include/flash.h"
#include "f2802x_common/include/gpio.h"
#include "f2802x_common/include/pie.h"
#include "f2802x_common/include/pll.h"
#include "f2802x_common/include/pwm.h"
#include "f2802x_common/include/pwr.h"
#include "f2802x_common/include/wdog.h"

Uint16 state=0x0002;
const Uint16 STOPPED=0x0002;
const Uint16 STOP=0x0000;
const Uint16 STARTED=0x0003;
const Uint16 START=0x0001;
Uint16 hrInc = 1;
Uint16 hrMin=0xFF;
Uint16 hr;
Uint16 hrMax=0x00;

Uint16 period = 0x190; // 0x48 min
Uint16 widthInc = 1;
Uint16 widthMin;
Uint16 width;
Uint16 deadBand = 0x20; //  765 ns - IRFB4110=258ns
Uint16 widthMax;
Uint16 widthB; // 460ns
Uint16 phase;
Uint16 memPeriod;

void scalePeriods() {
	if (period >= (2*(deadBand + 2)+4)) {
	  widthMin = period - 2;
	  widthMax = period / 2 + deadBand;
	  widthB = period - widthMax;
	  phase = period - 3;
	}
}

Uint32 waitIncWidth=10000; // micro secs
Uint32 waitIncStep=1000000;// micro secs

Uint32 inputCounter=0;

CLK_Handle myClk;
FLASH_Handle myFlash;
GPIO_Handle myGpio;
PIE_Handle myPie;
PWM_Handle myPwm3, myPwm4;

void HRPWM_Config(PWM_Handle,PWM_Number_e, PWM_SyncMode_e, Uint16);

void startStop() ;

extern void DSP28x_usDelay(Uint32 Count);
interrupt void XINT_2_ISR(void);
interrupt void XINT_3_ISR(void);

// init LED
void configGPIO0() {
    GPIO_setMode(myGpio, GPIO_Number_0, GPIO_0_Mode_GeneralPurpose);
    GPIO_setDirection(myGpio, GPIO_Number_0, GPIO_Direction_Output);
    GPIO_setPullUp(myGpio,GPIO_Number_0,GPIO_PullUp_Disable);
    GPIO_setHigh(myGpio, GPIO_Number_0);
}

void configGPIO1() {
    GPIO_setMode(myGpio, GPIO_Number_1, GPIO_1_Mode_GeneralPurpose);
    GPIO_setDirection(myGpio, GPIO_Number_1, GPIO_Direction_Output);
    GPIO_setPullUp(myGpio,GPIO_Number_1,GPIO_PullUp_Disable);
    GPIO_setHigh(myGpio, GPIO_Number_1);
}

void configGPIO2() {
    GPIO_setMode(myGpio, GPIO_Number_2, GPIO_2_Mode_GeneralPurpose);
    GPIO_setDirection(myGpio, GPIO_Number_2, GPIO_Direction_Output);
    GPIO_setPullUp(myGpio,GPIO_Number_2,GPIO_PullUp_Disable);
    GPIO_setHigh(myGpio, GPIO_Number_2);
}

void configGPIO3() {
    GPIO_setMode(myGpio, GPIO_Number_3, GPIO_3_Mode_GeneralPurpose);
    GPIO_setDirection(myGpio, GPIO_Number_3, GPIO_Direction_Output);
    GPIO_setPullUp(myGpio,GPIO_Number_3,GPIO_PullUp_Disable);
    GPIO_setHigh(myGpio, GPIO_Number_3);
}

// PWM 3 & 4
void configPWM() {
	scalePeriods();
    memPeriod = period;

	HRPWM_Config(myPwm3, PWM_Number_3, PWM_SyncMode_CounterEqualZero, 0);
	HRPWM_Config(myPwm4, PWM_Number_4, PWM_SyncMode_EPWMxSYNC, phase);

	CLK_enableTbClockSync(myClk);

	GPIO_setMode(myGpio, GPIO_Number_4, GPIO_4_Mode_EPWM3A);
	GPIO_setMode(myGpio, GPIO_Number_5, GPIO_5_Mode_EPWM3B);
	GPIO_setMode(myGpio, GPIO_Number_6, GPIO_6_Mode_EPWM4A);
	GPIO_setMode(myGpio, GPIO_Number_7, GPIO_7_Mode_EPWM4B);

	GPIO_setLow(myGpio, GPIO_Number_0);
}

// Button
void configGPIO12(CPU_Handle cpu) {
	GPIO_setMode(myGpio, GPIO_Number_12, GPIO_12_Mode_GeneralPurpose);
	GPIO_setDirection(myGpio, GPIO_Number_12, GPIO_Direction_Input);
    GPIO_setPullUp(myGpio,GPIO_Number_12,GPIO_PullUp_Disable);
	GPIO_setQualification(myGpio, GPIO_Number_12, GPIO_Qual_Sample_6);
	GPIO_setQualificationPeriod(myGpio, GPIO_Number_12, 0xFF);
	GPIO_setExtInt(myGpio, GPIO_Number_12, CPU_ExtIntNumber_3);
	// Enable XINT3 pin
	PIE_enableExtInt(myPie, CPU_ExtIntNumber_3);
	// Interrupt triggers on falling edge
	PIE_setExtIntPolarity(myPie, CPU_ExtIntNumber_3, PIE_ExtIntPolarity_FallingEdge);
	// Register interrupt handlers in the PIE vector table
	PIE_registerPieIntHandler(myPie, PIE_GroupNumber_12, PIE_SubGroupNumber_1, (intVec_t)&XINT_3_ISR);
	// Enable XINT3 in the PIE: Group 12
	PIE_enableInt(myPie, PIE_GroupNumber_12, PIE_InterruptSource_XINT_3);
	// Enable CPU INT12
	CPU_enableInt(cpu, CPU_IntNumber_12);
}

// Battery
void configGPIO19(CPU_Handle cpu) {
    GPIO_setMode(myGpio, GPIO_Number_19, GPIO_19_Mode_GeneralPurpose);
    GPIO_setDirection(myGpio, GPIO_Number_19, GPIO_Direction_Input);
    GPIO_setPullUp(myGpio,GPIO_Number_19,GPIO_PullUp_Disable);
    GPIO_setQualification(myGpio, GPIO_Number_19, GPIO_Qual_Sample_6);
    GPIO_setQualificationPeriod(myGpio, GPIO_Number_19, 0xFF);
    GPIO_setExtInt(myGpio, GPIO_Number_19, CPU_ExtIntNumber_2);
    PIE_enableExtInt(myPie, CPU_ExtIntNumber_2);
    PIE_setExtIntPolarity(myPie, CPU_ExtIntNumber_2, PIE_ExtIntPolarity_RisingAndFallingEdge);
    PIE_registerPieIntHandler(myPie, PIE_GroupNumber_1, PIE_SubGroupNumber_5, (intVec_t)&XINT_2_ISR);
    PIE_enableInt(myPie, PIE_GroupNumber_1, PIE_InterruptSource_XINT_2);
    CPU_enableInt(cpu, CPU_IntNumber_1);
}

void main(void)
{
    CPU_Handle myCpu;
    PLL_Handle myPll;
    PWR_Handle myPwr;
    WDOG_Handle myWDog;
    
    // Initialize all the handles needed for this application    
    myClk = CLK_init((void *)CLK_BASE_ADDR, sizeof(CLK_Obj));
    myCpu = CPU_init((void *)NULL, sizeof(CPU_Obj));
    myFlash = FLASH_init((void *)FLASH_BASE_ADDR, sizeof(FLASH_Obj));
    myGpio = GPIO_init((void *)GPIO_BASE_ADDR, sizeof(GPIO_Obj));
    myPie = PIE_init((void *)PIE_BASE_ADDR, sizeof(PIE_Obj));
    myPll = PLL_init((void *)PLL_BASE_ADDR, sizeof(PLL_Obj));
    myPwr = PWR_init((void *)PWR_BASE_ADDR, sizeof(PWR_Obj));
    myPwm3 = PWM_init((void *)PWM_ePWM3_BASE_ADDR, sizeof(PWM_Obj));
    myPwm4 = PWM_init((void *)PWM_ePWM4_BASE_ADDR, sizeof(PWM_Obj));
    myWDog = WDOG_init((void *)WDOG_BASE_ADDR, sizeof(WDOG_Obj));
    
    // Perform basic system initialization    
    WDOG_disable(myWDog);
    CLK_enableAdcClock(myClk);
    (*Device_cal)();
    CLK_disableAdcClock(myClk);
    
    //Select the internal oscillator 1 as the clock source
    CLK_setOscSrc(myClk, CLK_OscSrc_Internal);
    
    // Setup the PLL for x10 /2 which will yield 50Mhz = 10Mhz * 10 / 2
    PLL_setup(myPll, PLL_Multiplier_10, PLL_DivideSelect_ClkIn_by_2);
    
    // Disable the PIE and all interrupts
    PIE_disable(myPie);
    PIE_disableAllInts(myPie);
    CPU_disableGlobalInts(myCpu);
    CPU_clearIntFlags(myCpu);

    configGPIO0();
    configGPIO1();
    configGPIO2();
    configGPIO3();

    // If running from flash copy RAM only functions to RAM   
#ifdef _FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
#endif   

    // Setup a debug vector table and enable the PIE
    PIE_setDebugIntVectorTable(myPie);
    PIE_enable(myPie);

    CPU_enableGlobalInts(myCpu);
    CLK_disableTbClockSync(myClk);

    configPWM();

    // Button
    configGPIO12(myCpu);

    // Battery
    configGPIO19(myCpu);

    startStop(myPll,myPwr);
}

void waitInc() {
	DELAY_US(waitIncWidth);
}

void waitStep() {
	DELAY_US(waitIncStep);
}

void incrPulseHr() {
	for (hr=hrMin; hr != hrMax-1; hr-=hrInc) {
		if (state==STOP) {
			asm(" NOP");
			return;
		}
		PWM_setCmpAHr(myPwm3, hr << 8);
		PWM_setCmpAHr(myPwm4, hr << 8);
		waitInc();
	}
}

void incrPulse() {
	for (width=widthMin; width != widthMax-1;width-=widthInc) {
		if (state==STOP) {
			asm(" NOP");
			return;
		}
		PWM_setCmpA(myPwm3, width);
		PWM_setCmpA(myPwm4, width);
		waitInc();
	}
	state = STARTED;
}

void decrPulseHr() {
	for (hr=hrMax; hr != hrMin+1; hr+=hrInc) {
		PWM_setCmpAHr(myPwm3, hr << 8);
		PWM_setCmpAHr(myPwm4, hr << 8);
	}
}

void decrPulse() {
    for (width=widthMax; width != widthMin+1;width+=widthInc) {
		PWM_setCmpA(myPwm3, width);
		PWM_setCmpA(myPwm4, width);
	}
}

void changeFrequency() {
	PWM_setPeriod(myPwm3,period-1);
	PWM_setPeriod(myPwm4,period-1);
	PWM_setCmpB(myPwm3,widthB);
	PWM_setCmpB(myPwm4,widthB);
    PWM_setPhase(myPwm4, phase);
}

void startStop(PLL_Handle pll, PWR_Handle pwr) {
	state = START;
    while (true) {
    	    if (state == STOPPED) {
    	        if ( PLL_getClkStatus(pll) != PLL_PLLSTS_MCLKSTS_BITS)
    	        {
    	            // LPM mode = Standby
    	            PWR_setLowPowerMode(pwr, PWR_LowPowerMode_Idle);
    	        }
    			GPIO_setHigh(myGpio, GPIO_Number_0);
    	    		IDLE;
    			GPIO_setLow(myGpio, GPIO_Number_0);
    	    }
        if (memPeriod != period) {
      	  scalePeriods();
        	  memPeriod = period;
        	  switch (state) {
        	    case STARTED :
    					changeFrequency();
    					PWM_setCmpA(myPwm3,widthMax);
    					PWM_setCmpA(myPwm4,widthMax);
    					break;
        	    }
        }
		if (state == START) {
			changeFrequency();
			PWM_setCmpA(myPwm3,widthMin);
			PWM_setCmpA(myPwm4,widthMin);
			GPIO_setLow(myGpio, GPIO_Number_1);
			incrPulseHr();
			GPIO_setLow(myGpio, GPIO_Number_2);
			incrPulse();
			GPIO_setHigh(myGpio, GPIO_Number_1);
			GPIO_setHigh(myGpio, GPIO_Number_2);
		}
		if (state == STOP) {
			GPIO_setLow(myGpio, GPIO_Number_3);
			decrPulse();
			decrPulseHr();
			state = STOPPED;
			changeFrequency();
			GPIO_setHigh(myGpio, GPIO_Number_3);
		}
    }
}

// Battery
interrupt void XINT_2_ISR(void)
{
	if (GPIO_getData(myGpio, GPIO_Number_19) == 1) {
		state = START;
	} else {
		state = STOP;
	}
	PIE_clearInt(myPie, PIE_GroupNumber_1);
}

// Button
interrupt void XINT_3_ISR(void)
{
	inputCounter++;
    if (state==STOPPED) {
    	  state = START;
    } else {
    	  state = STOP;
    }
	PIE_clearInt(myPie, PIE_GroupNumber_12);
}

void HRPWM_Config(PWM_Handle pwm, PWM_Number_e nb, PWM_SyncMode_e sync, Uint16 phase)
{
    
    CLK_enablePwmClock(myClk, nb);

//    PWM_setPeriodLoad(pwm, PWM_PeriodLoad_Immediate);
    PWM_setCmpAHr(pwm, (hrMin << 8));
    PWM_setPeriod(pwm, period-1);
    PWM_setCmpA(pwm, widthMin);
    PWM_setCmpB(pwm, widthB);
    PWM_setPhase(pwm, phase);
    PWM_setCount(pwm, 0x0001);
    
    PWM_setCounterMode(pwm, PWM_CounterMode_UpDown);
    if (sync == PWM_SyncMode_CounterEqualZero ) {
      PWM_disableCounterLoad(pwm);
    } else {
      PWM_enableCounterLoad(pwm);
    }
    PWM_setSyncMode(pwm, sync);
    PWM_setHighSpeedClkDiv(pwm, PWM_HspClkDiv_by_1);
    PWM_setClkDiv(pwm, PWM_ClkDiv_by_1);
    
//    PWM_setShadowMode_CmpA(pwm, PWM_ShadowMode_Shadow);
//    PWM_setShadowMode_CmpB(pwm, PWM_ShadowMode_Shadow);
    PWM_setLoadMode_CmpA(pwm, PWM_LoadMode_Zero);
    PWM_setLoadMode_CmpB(pwm, PWM_LoadMode_Zero);
    
    PWM_setActionQual_Zero_PwmA(pwm, PWM_ActionQual_Clear);
    PWM_setActionQual_CntUp_CmpA_PwmA(pwm, PWM_ActionQual_Set);
    PWM_setActionQual_CntDown_CmpA_PwmA(pwm, PWM_ActionQual_Clear);
    PWM_setActionQual_Zero_PwmB(pwm, PWM_ActionQual_Set);
    PWM_setActionQual_CntUp_CmpB_PwmB(pwm, PWM_ActionQual_Clear);
    PWM_setActionQual_CntDown_CmpB_PwmB(pwm, PWM_ActionQual_Set);

    //MEP control on Rising edge
    PWM_setHrEdgeMode(pwm, PWM_HrEdgeMode_Rising);
    PWM_setHrControlMode(pwm, PWM_HrControlMode_Duty);
    PWM_setHrShadowMode(pwm, PWM_HrShadowMode_CTR_EQ_0);

}


