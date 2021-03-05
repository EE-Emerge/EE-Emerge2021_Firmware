#include <Wire.h>

/* LED DEF’s */
#define LED1 P1_0
#define LED2 P1_6
#define LED3 P2_1
#define LED4 P2_3
#define LED5 P2_5
#define LED1_BUTTON P1_7
#define LED2_BUTTON P1_3

/* BUTTON SPEED */
#define PWM P1_6
#define SLOW_BUTTON P1_7
#define SPEED_BUTTON P1_3


/************ GET BATTERY LOGIC **********/
#define NOMINAL_CAPACITY 0x08
#define FULL_AVAILABILITY_CAPACITY 0x0A
#define REMAINING_CAPACITY 0x0C
#define FULL_CHARGE_CAPACITY 0x0E
#define AVERAGE_CURRENT 0x10
#define STANDBY_CURRENT 0x12
#define MAXLOAD_CURRENT 0x14
#define AVERAGE_POWER 0x18
#define STATE_OF_HEALTH 0x20
#define STATE_OF_CHARGE 0x1C
#define INTERNAL_TEMPERATURE 0x1E
#define REMAINING_CAPACITY_UNFILTERED 0x28
#define REMAINING_CAPACITY_FILTERED 0x2A
#define FULL_CHARGE_CAPACITY_UNFILTERED 0x2C
#define FULL_CHARGE_CAPACITY_FILTERED 0x2E
#define STATE_OF_CHARGE_UNFILTERED 0x30 
#define TRUE_REMAINING_CAPACITY 0x6A
/************************** GLOBAL VAR START ********************/
/**************************************GLOBAL VAR  LED’s *****************************************/
bool lastStateLedOneBtn = 0;
bool lastStateLedTwoBtn = 0;
/* Check if LEDs are on or off*/
bool ledOne = 0;    // 0 = LED off, 1 = LED on
bool ledTwo = 0;
/******************************** GLOBAL VAR SPEED BUTTONS *******************/
bool lastStateSpeedBtn = 0;
bool lastStateSlowBtn = 0;
int pwmVal = 0;
/************************** GLOBAL VAR END ********************************/


/********************************* HELPER FUNCTIONS SPEED UP*******************/
/*The next 2 functions change the PWM speed by 5*/
void buttonSpeedUpPressed() 
{
	bool currentBtnState = digitalRead(SPEED_BUTTON);

	if( !lastStateSpeedBtn && currentBtnState)
	{
		pwmVal = pwmVal + 5;
		if ( pwmVal>= 1023) {    // if PWM is at max, no change
			pwmVal = 1023;
		} else {    // if PWM is not at max, write new value
			analogWrite(PWM, pwmVal);
		}
	}
	lastStateSpeedBtn = currentBtnState;
	return;
}
/********************** HELPER FUNCTIONS LED *******************/
/*The next 2 functions turn the specified LED on or off when the button is pressed, depending on the state of the LED*/
void buttonLedOnePressed() 
{
	bool currentBtnState = digitalRead(LED1_BUTTON);
	bool btnPressed = 0;

	if( !lastStateLedOneBtn && currentBtnState)
	{
		btnPressed = 1;
		if (ledOne == 0) {    // if off, turn it on
			digitalWrite(LED1, HIGH);
			ledOne = 1;
		} else {    // if on, turn it off
			digitalWrite(LED1, LOW);
			ledOne = 0;
		}
	}
	lastStateLedOneBtn = currentBtnState;
	return;
}

void buttonLedTwoPressed() {
	bool currentBtnState = digitalRead(LED2_BUTTON);
	bool btnPressed = 0;

	if( !lastStateLedTwoBtn && currentBtnState)
	{
		btnPressed = 1;
		if (ledTwo == 0) {    // if off, turn it on
			digitalWrite(LED2, HIGH);
			ledTwo = 1;
		} else {    // if on, turn it off
			digitalWrite(LED2, LOW);
			ledTwo = 0;
		}
	}
	lastStateLedTwoBtn = currentBtnState;
	return;
}

/**************************** HELPER FUNCTIONS BATTERY *************************/

void buttonSlowDownPressed(int buttonPin) 
{
	bool currentBtnState = digitalRead(SLOW_BUTTON);

	if( !lastStateSlowBtn && currentBtnState)
	{
		pwmVal = pwmVal - 5;
		if ( pwmVal<= 0) {    // if PWM is at min, no change
			pwmVal = 0;
		} else {    // if PWM is not at min, write new value
			analogWrite(PWM, pwmVal);
		}
	}
	lastStateSlowBtn = currentBtnState;
	return;
}
/* Get Register from Fuel Gauge */
uint16_t Get_BatteryValue(uint8_t fuelregister)
{
	return Get_BatteryRegister(fuelregister);
}

uint16_t Get_BatteryRegister(uint8_t commandAddress)
{
	Wire.beginTransmission(0xAA); // transmit to device #44 (0x2c) Send Start Address
	Wire.write(byte(commandAddress));            // sends instruction byte
	Wire.write(0xAB);             // sends potentiometer value byte
	uint8_t lsb = Wire.read();
	uint8_t msb = Wire.read();
	uint16_t combined = msb << 8 | lsb;
	Wire.endTransmission();     // stop transmitting

	return combined;
}


/******************** VOID MAIN **************************/
void setup() 
{
	// initialize the digital pin as an output.

	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);
	pinMode(LED4, OUTPUT);
	pinMode(LED5, OUTPUT);
	pinMode(LED1_BUTTON, INPUT);
	pinMode(LED2_BUTTON, INPUT);
	digitalWrite(LED1, 0);
	digitalWrite(LED2, 0);
	digitalWrite(LED3, 0);
	digitalWrite(LED4, 0);
	digitalWrite(LED5, 0);


	pinMode(SPEED_BUTTON, INPUT);
	pinMode(SLOW_BUTTON, INPUT);
	pinMode(PWM, OUTPUT);
	analogWrite(PWM, 0);


}
void loop()
{
	buttonLedOnePressed();
	buttonLedTwoPressed();
	buttonSpeedUpPressed();
}

