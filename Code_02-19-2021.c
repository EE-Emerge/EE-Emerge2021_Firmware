/*
 * EE-Emerge W2021 
 * Software team 
 *
 *
 */

/************************* INCLUDE ***********************/
#include<stdio.h> 
#include "msp430g2231.h"
#include<boolean.h> // I don't think this is needed maybe 

/*********************** END INCLUDE ********************/


/********************************* GLOBAL VAR + MACRO  *******************/
/************* Please use these variables names when refering to components *****/
#define HIGHLED P1_1/* Blink if Battery is High Pin*/
#define MIDLED P1_3/* Blink if Battery is Mid Percent Pin*/
#define LOWLED P1_4 /* Blink if Battery is Low Pin*/
#define BUZZER /* INSERT BUZZPIN */
#define BATTERY /* Insert Battery Pin */
#define SPEEDUP /* Pin Number  */
#define SPEEDDOWN /* Insert Pin Number */
#define CHECK_BATTERY /* Same as above */

#define 1MHZ = 1000000
/* Cycle Through this Tone Array and Play the Notes. Notes and Beat Number are arbitrary */
int NUMBEATS = 5;
/* TODO */
int beats[NUMBEATS] = {/* Insert R2D2 Noises Here */}
int duration[NUMBEATS] = {/* Insert Duration of Notes here i.e. quarter note, half note */} 

/*************************** GLOBAL VAR + MACRO END *************************/

/****************************** FUNCTION PROTOTYPES END **************/
/* Set up initial state */
void setup() {
	// this code will run once

	/* Handle Interrupts */
	WDTCTL = WDTPW + WDTHOLD; // Reset Timer
	CTL0 = CCIE;                             // Enable Timer Interrupt 
	TACTL = TASSEL_2 + MC_1 + ID_3;           /* MC_1 Means count up to CCR0, Tassel_2 means count 1Mhz a second, ID_3 means divide clock by 8 
						   Frequency is really 1000000/8 = 125000*/

	CCR0 =  timer(10); /* Currently Set at 10 Minutes */
	/* If you want a second timer, use CCR1 */


	/* PinMode for LEDS */
	pinMode(HIGHLED, OUTPUT);
	pinMode(MIDLED, OUTPUT);
	pinMode(LOWLED, OUTPUT);

	/* PinMode for Buttons */

	/* If you want to reverse trigger buttons 
	P1REN |= BUTTON;             // Enable internal pull-up/down resistors
 	P1OUT |= BUTTON;                 //Select pull-up mode for P1.3
	*/

	pinMode(SPEEDUP, INPUT);
	pinMode(SPEEDDOWN, INPUT);
	pinMode(CHECK_BATTERY, INPUT);

	/* TODO: Set up DC motors and Piezzo */

	/* Interrupts P2 */ 
	P1DIR = 0x00; /* Turn off all the GPIO Pins in P1 */
	P1IE = SPEEDUP | 1; /* Turn on Interupts for SpeedUp Botton */
	P1IE = SPEEDDOWN | 1; /* Turn on Interupts for SpeedDown Button */
	P1IE = CHECK_BATTERY | 1; /* Turn on Interupts for CheckBattery */


	P1IFG ~= SPEEDUP; /* Lower Interrupt Flag . I can just set this to zero ???? */
	P1IFG ~= SPEEDDOWN; /* Lower Interrupt Flag . I can just set this to zero ???? */
	P1IFG ~= BATTERY_PRESSED; /* Lower Interrupt Flag . I can just set this to zero ???? */

	__bis_SR_register(GIE+CPUOFF); /* GIE stands for Global Interrupt Enable. Enables all Interrupts. Idk if this is necessary */

} //end set up 

/* Loop forever */
void loop() {

	/* 


	   If Interrupts Work, while{1} should be enough for this function



	 */
	// this code will repeatedly loop
	// check if speed up button is pressed
	if(buttonSpeedChangePressed()) {
		// do fan speed stuff
	}


	if(buttonPowerPressed()) {
		// do some battery level stuff
	}

	if (millis() % 5000 == 0) {
		batteryWarning(batteryPin)	//<- to be filled in w battery pin
	}

}

/************************************ CONTROL FAN LOGIC ****************************/
/* Check the state of speed up button */
bool buttonSpeedUpPressed() {
	/* TODO */
	// assuming the button pin is active-low,
	int buttonState = digitalRead(SPEED_BUTTON);
	buttonState = HIGH
		return false;
}

bool buttonSlowDownPressed(int buttonPin) {
	// poll the slow down button pin
	/* TODO */
	return false;
}
/************************ END CONTROL FAN LOGIC **************************************/

/************************ HELPER FUNCTIONS FOR BATTERY ******************************/

/* Read Battery Charge of Battery */
int readBatteryCharge(){
	/* TODO: Analog Mapping */

	batteryPercent = analogRead(BATTERY);
	/* Perform Mapping Here */
	return batteryPercent;	
}

/* Checks the battery level and displays on the LEDs */
void buttonPowerPressed(){
	int battery = readBatteryCharge();

	/* Handle Edge Case if Error in Reading Battery Level */
	if (battery > 100){
		battery = 100;
	} else if (battery < 0){
		battery = 0;
	}

	/*Handle Logic */
	if (battery > 89) {
		writeToLed(HIGH, HIGH, HIGH);					// GREEN GREEN RED ON 

	} else if (battery > 78) {
		writeToLed(LOW, HIGH, HIGH); 					//MEDLED and LOWLED ON 
		blinkLed(HIGHLED);  						// HIGHLED BLINKING 

	} else if (battery > 67) {						/* MEDLED AND LOWLED ON */
		writeToLed(LOW, HIGH, HIGH);

	} else if (battery > 50) { 
		writeToLed(LOW, LOW, HIGH); 					/* LOWLED ON */
		blinkLed(MEDLED); 						/*MEDLED BLINKING */

	} else if (battery > 33) {
		writeToLed(LOW, LOW, HIGH);					// LOWLED ON

	} else
		blinkLed(LOWLED);						// blink LED1 - RED LED
	}

	/* Wait X Seconds. Then turn off LEDS */
	int X = 5;
	delay(X*1000);
	writeToLED(LOW, LOW, LOW) ;						

}  

/* Poll Battery and Check if it is low */
void batteryWarning(int batteryPin) {
	int battery = readBatteryCharge();
	if (battery < 15) {
		blinkLed(LED1);
		blinkLed(LED1);

	}
}

/******************************** END BATTERY HELPER FUNCTIONS *****************/

/******************************** PIEZO FUNCTIONS ******************************/
void playPiezzo(){
	for(int i = 0; i < NUMBEATS; i++){
		/* Is tones() supported by our microcontroller */
		/* TODO: https://www.arduino.cc/en/Tutorial/BuiltInExamples/toneMelody */
	}
}
/************************** LED HELPER FUNCTIONS *******************************/
/* Write to LED for battery. Takes 3 Args 
 * pin1 is MSB, pin3 is LSB
 HIGH     MED            LOW  */
void writeToLed(int pin1, int pin2, int pin3){
	digitalWrite(REDLED, pin3);
	digitalWrite(MIDLED, pin2);
	digitalWrite(TOPLED, pin1);
}

/* Blink LEDs */
void blinkLed(int LEDToBlink){
	for(int i = 0; i < 5; i++){
		digitalWrite(LEDToBlink, HIGH);
		delay(250);
		digitalWrite(LEDToBlink, LOW);
		delay(250);
	}

}

/************************** END LED HELPER FUNCTIONS *************************/

/**************************** HANDLE INTERUPTS *******************************/

/* Interval for Timer to Trigger */
long int timer(int minutes){
	minutes = minutes * 60;
	return (1MHZ / 8) * minutes;
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1 (void){
	if (buttonPowerPressed()){
		P1IFG ~= BATTERY_PRESSED; /* Lower Interrupt Flag . I can just set this to zero ???? */
	}
	if(buttonSpeedUpPressed()){
		P1IFG ~= SPEEDUP; /* Same as above */
	if(buttonSlowDownPressed()){
		P1IFG ~= SPEEDDOWN; /* Same as above */
	}

}
/* I don't think we have any IO on PORT2 */
#pragma vector=PORT2_VECTOR
__interrupt void PORT2 (void)
{
	
}
#pragma vector=TIMERA0_VECTOR
__interrupt void BatteryStatusTimer(void){
	checkBatteryStatus();
}

#pragma vector=TIMERA1_VECTOR
__interrupt void misc(void){

	/* Idk do we need this */
}


/* Leaving these here in case it doesn't compile */
/**************************** FUNCTION PROTOTYPES  ********************/
void pinSetUp();
	bool buttonSpeedUpPressed(int) 
	bool buttonSlowDownPressed(int) 
	void buttonPowerPressed (int, int)
	void writeToLed(int, int, int)
	void batteryWarning(int) 
int readBatteryCharge()

	/****************************** FUNCTION PROTOTYPES END **************/
