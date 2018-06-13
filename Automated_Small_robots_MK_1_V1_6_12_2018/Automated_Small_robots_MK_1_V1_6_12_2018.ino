
/*
  PROJECT     :  ROBOT ROOM
  NAME        :  AUTOMATED ROBOT
  BOARD       :  ARDUINO NANO
  PROCESSOR   :  AT Mega 328P [ Old Bootloader ]
  DATE        :  6 / 12 / 2018
  PROGRAMMER  :  SERGEI DINES
*/
#include <Servo.h>
#include <FastLED.h>
#define DATA_PIN 11   // was 2
#define NUM_LED 72
CRGB leds[NUM_LED];
CRGB colors[] = {CRGB::HotPink, CRGB::Green, CRGB::Blue, CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::Purple, CRGB::Cyan, CRGB::Magenta, CRGB::Aqua, CRGB::Lavender};

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

//#define LEDring              11
#define IR_sensor_pin          4   // IR Laser Pin  - Does it need to be digital pin? Was PWM/Digital Pin #13 before 6 12 2018
int IR_state                  = 1 ;
int x                         = 0;   // Jump from loop to null while routine when 1 (one)
int y                         = 0;   // RGB LED RING For Loop Variable Holder
int flip1                     = 0 ;  // State if hit
int count                     = 0;
int Q = 0; // For LED Start Routine
int z = 0;  // Tracks what count is as a more solid bit [ like static ] - can be used to stay in null loop if while loop
int zz = 0; // Main Void Loop if/while statement

void setup() {
  Serial.begin(9600);
  delay(5000); // Give time for power to run in from relay
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LED);
 
  pinMode(12, OUTPUT);        // Data out to main board
  pinMode(IR_sensor_pin,      INPUT_PULLUP);

  //


  // RGB RING INITIAL ROUTINE  :
  //SPECS : Outter Ring = 32, Next =  24, Inner Ring = 16
  CRGB temp = colors[random(0, 10)];
  if (Q == 0) {
    boolean isFull = false;
    if (!isFull) {
      for (float x = 0; x < 32; x++) {
        leds[(int) x] = CRGB::White;
        leds[(int)(32 + (24 * (x / 32)))] = CRGB::Yellow;
        leds[(int)(56 + (16 * (x / 32)))] = CRGB::Red;

        leds[(int) x] /= 16;
        leds[(int)(32 + (24 * (x / 32)))]  /= 16;
        leds[(int)(56 + (16 * (x / 32)))]  /= 16;

        FastLED.show();
        delay(50);
      }
      Q = 1;
    }
  }

  //  if (Q == 1) {
  //    boolean isFull = false;
  //    if (!isFull) { // Shut off All LEDs after LED BOOT - UP SEQUENCE
  //      for (float x = 0; x < 72; x++) {
  //        leds[(int) x] = CRGB::Black;
  //        FastLED.show();
  //        delay(50);
  //      }
  //    }
  //  }

}

void loop() {

  if (zz == 0) {
    // TO AVOID FALSE TRIGGERING while running robot boot up gun routine :

    IR_state                  = 1;
    IR_state  = digitalRead(IR_sensor_pin );
    Serial.print("1st IR State:");
    Serial.print(IR_state);
    // END FALSE TRIGGERING Routine

    if (IR_state == 0) {
      delay(50);
      digitalWrite(5, HIGH);
    }
    if (IR_state == 1) {
      digitalWrite(5, LOW);
    }

    // 1 =
    // 2 =
    // 3 = Gun?
    // 4 =
    // 5 = Run
    // ROBOT START GUN SOUND
    // SEQUENCE TO ACTIVATE PINS : 
    //   "GO" / RUN >> Hack Pin >> "GO" / RUN   *with at least 200 milliseconds of delay between button presses
    delay(5000); // Give time to execute start up sound
    servo1.attach(3);
    servo2.attach(5);
    servo3.attach(6);
    servo4.attach(9);

    servo1.write(45);
    servo2.write(45);
    servo3.write(45);
    servo4.write(45);
 
    zz = 1;
  }

  if (zz == 1) {
   
    // SERVO ROUTINE : 
    delay(50);
    servo1.write(50);
    servo2.write(50);
    servo3.write(50);
    servo4.write(50);
    
    IR_state = 1; // Always until sensor goes off

    IR_state  = digitalRead(IR_sensor_pin );
        Serial.print("IR STATE LOOP : ");
        Serial.println(IR_state);
        delay(100);
    if (IR_state == 0) {
      delay(50);
      digitalWrite(5, HIGH);
    }
    if (IR_state == 1) {
      digitalWrite(5, LOW);
    }
    if (IR_state == 0) {
      count++;
      // DATA TO MAIN BOARD :
      digitalWrite(12, HIGH);
      delay(300);
      digitalWrite(12, LOW);
      delay(300);
    }

    while (count == 1 && z == 0) {

      boolean isFull = false;
      if (!isFull) {
        for (float x = 57; x < 72; x++) {
          leds[(int) x] = CRGB::Black;
          delay(3);
        }
        FastLED.show(); // Instead of in for loop ,  put outside to turn off after all go through
        delay(50);
      }

      for (float x = 57; x < 72; x++) {
        leds[(int) x] = CRGB::Red;
        delay(3);
      }
      FastLED.show(); // Instead of in for loop ,  put outside to turn off after all go through
      delay(50);

      for (float x = 57; x < 72; x++) {
        leds[(int) x] = CRGB::Black;
        delay(3);
      }
      FastLED.show(); // Instead of in for loop ,  put outside to turn off after all go through
      delay(50);
      z = 1;
    }


    while (count == 2 && z == 1) {

      for (float x = 32; x < 57; x++) {
        leds[(int) x] = CRGB::Black;

        delay(3);
      }
      FastLED.show();
      delay(50);


      for (float x = 32; x < 57; x++) {
        leds[(int) x] = CRGB::Red;

        delay(3);
      }
      FastLED.show();
      delay(50);

      for (float x = 32; x < 57; x++) {
        leds[(int) x] = CRGB::Black;

        delay(3);
      }
      FastLED.show();
      delay(50);
      z = 2;
    }

    while(count == 3 && z == 2) {

      for (float x = 0; x < 32; x++) {
        leds[(int) x] = CRGB::Black;

        delay(3);
      }
      FastLED.show();
      delay(50);

      for (float x = 0; x < 32; x++) {
        leds[(int) x] = CRGB::Red;

        delay(3);
      }
      FastLED.show();
      delay(50);


      for (float x = 0; x < 32; x++) {
        leds[(int) x] = CRGB::Black;

        delay(3);
      }
      FastLED.show();
      delay(50);
      z = 3;
    }

    while(z == 3) {
      // NULL LOOP
    }
    //  if (z = 3) {
    //    analogWrite(10, 2);
    //  }

    /*

      OUTTER = 32 , INNER = 24,  MOST INNER = 16

    */

  }
} // END VOID LOOP



// Observations 6 12 2018 with no gun fired at any target but rings were facing sensors : 
//   4th then 2nd Robot turns off around 30seconds - 1 min
//   1st ring turns off at ____ for robot #3
//   1st ring turns off at 4.5 min for robot #1

//   Possible Issues : LEDs giving off some IR?
// TEST #2  :
//   Without rings / arduino facing sensors :
//     No rings turned off
//     Therefore, very little IR from LEDs/ arduino blinking were responsible


