#include<stdio.h> 

#define REDLED /* RED LED Pin*/
#define MIDLED /* Green Middle LED Pin*/
#define TOPLED /* Green TOP LED Pin*/
#define BUZZPIN

void setup() {
	// this code will run once
	pinMode(13, OUTPUT);
	pinMode(13, OUTPUT);
	pinMode(13, OUTPUT);
}

boolean ledON
void loop() {
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

boolean buttonSpeedUpPressed(int buttonPin) {
	// poll the speed up button pin

	// assuming the button pin is active-low,
	int buttonState = digitalRead(buttonPin);

	return false;
}

boolean buttonSlowDownPressed(int buttonPin) {
	// poll the slow down button pin
	return false;
}

/* Checks the battery level and displays on the LEDs
 */
void buttonPowerPressed (int buttonPin, int batteryPin){
	int battery = //battery percentage
		if (battery  > 89) {
			writeToLed(HIGH, HIGH, HIGH);	// light up LEDs 1, 2, 3
		} else if (battery > 78) {
			writeToLed(HIGH, HIGH, LOW);
			blinkLed(LED3); 
		} else if (battery > 67) {	// light up LEDs 1, 2
			writeToLed(HIGH, HIGH, LOW);
		} else if (battery > 50) { 
			writeToLed(HIGH, LOW, LOW);
			blinkLed(LED2);
		} else if (battery > 33) {
			writeToLed(HIGH, LOW, LOW);	// light up LED 1
		} else
			blinkLed(LED1);	// blink LED1 - RED LED
}
// wait 2 seconds (or however long we decide)
writeToLED(LOW, LOW, LOW, LOW);	// turn off LEDs after giving battery indication
}


/* Write to LED for battery. Takes 3 Args 
 * pin1 is LSB, pin3 is MSB 
 * Arg1 is high or LOW for pin1 
 * Arg2 is high or LOW for pin2 
 * Arg3 is high or LOW for pin3 
 */
void writeToLed(int pin1, int pin2, int pin3){
	digitalWrite(REDLED, pin1);
	digitalWrite(MIDLED, pin2);
	digitalWrite(TOPLED, pin3);
}

/* If battery percent < 15, this function is called:
 *	Flash LED1 twice as long as usual
 *	Buzz piezo buzzer
 */
void batteryWarning(int batteryPin) {
	int battery = //battery percentage
		if (battery < 15) {
			blinkLed(LED1);
			blinkLed(LED1);
			// buzz piezo
		}
}

/* Internal Freq is 16Mhz */
void SysTick_Init(void){


}


