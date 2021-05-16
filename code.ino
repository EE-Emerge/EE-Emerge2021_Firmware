#include <Wire.h>
#include "msp430g2553.h"

#define MHZ 1000000
#define G_LED1 P1_3
#define G_LED2 P1_4
#define R_LED P1_1

#define BUZZER P2_6
#define CTL_FAN P1_0

#define FAST_BUTTON P1_5
#define SLOW_BUTTON P2_7
#define BATTERY_BUTTON P2_3


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

int speed = 0;
bool reached_15 = False; // checks if the battery has reached 15% yet (user is alerted at 15% battery)
int battery_charge = 0;
Int new_battery_charge = 0;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(G_LED1, OUTPUT);
  pinMode(G_LED2, OUTPUT);
  pinMode(R_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(CTL_FAN, OUTPUT);

  pinMode(FAST_BUTTON, INPUT);
  pinMode(SLOW_BUTTON, INPUT);
  pinMode(BATTERY_BUTTON, INPUT);

  attachInterrupt(FAST_BUTTON, speedUpInterrupt, RISING);
  attachInterrupt(SLOW_BUTTON, slowDownInterrupt, RISING);
  attachInterrupt(BATTERY_BUTTON, Battery_Interrupt_Response, RISING);

  WDTCTL = WDTPW + WDTHOLD;
  CCTL0 = CCIE;                             // CCR0 interrupt
  TACTL = TASSEL_2 + MC_1 + ID_3;           // SMCLK/8, upmode  
  CCR0 =  10000;
  P1IE = BATTERY_BUTTON | 1; /* Turn on Interrupts for CheckBattery */
  // P1IFG ~= BATTERY_BUTTON; /* Lower Interrupt Flag . I can just set this to zero ???? */P1IES |= BIT3;                     // P1.3 Hi/lo edge
  P1IFG &= ~BIT3;                  // P1.3 IFG cleared
  __bis_SR_register(GIE+CPUOFF); /* GIE stands for Global Interrupt Enable. Enables all Interrupts. Idk if this is necessary */
}

// the loop routine runs over and over again forever:
void loop() {

  if (FAST_BUTTON == 1){
    digitalWrite(G_LED1, LOW);
  }
  if (FAST_BUTTON == 0){
    digitalWrite(G_LED1, HIGH);
  }

  digitalWrite(G_LED2, LOW);
  delay(500);
  digitalWrite(G_LED2, HIGH);
  delay(500);
  
  new_battery_charge = check_Battery();
  if (new_battery_charge > battery_charge) {
    /* Now Charging */
    reached_15 = False;
  }  
  battery_charge = new_battery_charge;
  if (reached_15) {
    Beep();
    delay(1000);
    analogWrite(CTL_FAN, 50);
// turn the LED off by making the  voltage LOW
    digitalWrite(LED, LOW);      
}
  if (BATTERY_BUTTON == 1){
    // call fun for LED  
  }
}

/* Get Register from Fuel Gauge */
uint16_t Get_BatteryValue(uint8_t fuelregister)
{
  Get_BatteryRegister(fuelregister);
}

void Beep(){
  analogWrite(BUZZER,500);
  delay(1000);
  analogWrite(BUZZER, 0);
  delay(1000);
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

void speedUpInterrupt(){
  Beep();
  digitalWrite(G_LED1, 0);

  if (speed > 255){
    speed = 255;
  } else {
    speed = speed + 10;
  }
  analogWrite(BUZZER, speed);
}    

int state = 0;
void slowDownInterrupt(){
  Beep();
  state = !state;
  digitalWrite(P1_3, state);
  if (speed < 0){
    speed = 0;
  } else {
    speed = speed - 10;
  }
  analogWrite(BUZZER, speed);
}

void LED_Display(int GREEN, int GREEN2, int RED){
  digitalWrite(G_LED1, GREEN);
  digitalWrite(G_LED2, GREEN2);
  digitalWrite(R_LED, RED);
}

int checkBattery(){
  int battery_level = Get_BatteryValue();
  if (battery_level <= 15) {
    reached_15 = True;
  }
  return battery_level;
}

int Battery_Interrupt_Response(){
  if (battery_charge <= 100 && battery_charge > 85){
    LED_Display(HIGH, HIGH, HIGH);
    delay(5000);
    LED_Display(LOW, LOW, LOW);
  }
  else if  (battery_charge <= 85 && battery_charge > 66){
    for (int i = 0; i < 5; i++){
          LED_Display(LOW, HIGH, HIGH);
          delay(500);
      LED_Display(HIGH, HIGH, HIGH);
          delay(500);
    }
    LED_Display(LOW, LOW, LOW);
  }
  else if  (battery_charge <= 66 && battery_charge > 50){
      LED_Display(LOW, HIGH, HIGH);
    delay(5000);
    LED_Display(LOW, LOW, LOW);
  }
  else if  (battery_charge <= 50 && battery_charge > 35){
    for (int i = 0; i < 5; i++){
LED_Display(LOW, LOW, HIGH);
          delay(500);
          LED_Display(LOW, HIGH, HIGH);
          delay(500);
    }
    LED_Display(LOW, LOW, LOW);
  }
  else if  (battery_charge <= 35 && battery_charge > 25){
LED_Display(LOW, LOW, HIGH);
    delay(5000);
    LED_Display(LOW, LOW, LOW);
  }
  else if  (battery_charge <= 15 && battery_charge > 5){
    for (int i = 0; i < 5; i++){

      LED_Display(LOW, LOW, LOW);
      delay(500);
      LED_Display(LOW, LOW, HIGH);
      delay(500);
    }
    LED_Display(LOW, LOW, LOW);
  }
  else if  (battery_charge < 5)
    LED_Display(LOW, LOW, LOW);
      delay(5000);
  }
  LED_Display(LOW, LOW, LOW);
}

long int timer(int minutes){
  minutes = minutes * 1;
  return (1*MHZ / 8) * minutes;
}

#pragma vector=TIMERA0_VECTOR
__interrupt void BatteryStatusTimer(void){
  P1OUT ^= BIT4;                          // Toggle P1.4
}
