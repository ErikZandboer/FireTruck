/*--------------------------------------------------------------------------------------------*/
/* FireTruck led & audio simulation.                                                          */
/*                                                                                            */
/* Controls various leds and an mp3 player to simulate a model truck                          */
/*                                                                                            */
/* Versioning:                                                                                */
/* 1.00            Creation. First go at timing the leds and mp3 player.                      */
/*--------------------------------------------------------------------------------------------*/

// Includes
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "RedMP3.h"

// Define the physical pinout
#define FLASH1         17
#define FLASH2         18
#define FLASH3         19
#define FLASH4         20
#define FRONTFLASH1    24
#define FRONTFLASH2    25
#define HAZARDLIGHTS   26 
#define MP3_RX         23
#define MP3_TX         22   // connect to RX of the MP3 player module

// Setup serial comms to the MP3 hardware
MP3 mp3(MP3_RX, MP3_TX);

// Get some base counters in for the runtime in seconds.
unsigned int    TickCounter=0;

#define RUN_FREQ 100

// Timings and things to change
#define TIM_SAMPLESTART       5*RUN_FREQ        // 5 seconds
#define TIM_SAMPLELENGTH      61*RUN_FREQ       // 61 seconds

#define FLASH1SPEED           45
#define FLASH2SPEED           48
#define FLASH3SPEED           52
#define FLASH4SPEED           55
#define FLASHHAZARDSPEED      50                // 50 = 0,5 sec     100 = 1 sec      25 = 0,25 sec etc 

// This runs only once when powering on
void setup()
{
        digitalWrite(FLASH1, HIGH);          // FLASH light start OFF
        digitalWrite(FLASH2, HIGH);          // FLASH light start OFF
        digitalWrite(FLASH3, HIGH);          // FLASH light start OFF
        digitalWrite(FLASH4, HIGH);          // FLASH light start OFF
        digitalWrite(HAZARDLIGHTS, LOW);     // Hazard lights led group starts OFF
        digitalWrite(FRONTFLASH1,  HIGH);    // half of FRONTFLASH lights led group starts ON...
        digitalWrite(FRONTFLASH2,  LOW);     // ...the other half starts OFF
        
        // All led groups as OUTPUT
        pinMode (FLASH1, OUTPUT);
        pinMode (FLASH2, OUTPUT);
        pinMode (FLASH3, OUTPUT);
        pinMode (FLASH4, OUTPUT);
        pinMode (HAZARDLIGHTS,   OUTPUT);
        pinMode (FRONTFLASH1,   OUTPUT);
        pinMode (FRONTFLASH2,   OUTPUT);
        
        TickCounter=0;
}

// This loops forever.
void loop()
{
        while ( (millis() % 10) != 0L ) //Just do nothing until millis()/10 has no remainder --> Run at 100Hz
        {
                asm("nop \n"); // Just do NOP to make sure the compiler doesn't optimize the while() away
        }

        // This code executes 100 times a second
        TickCounter++;
        if (TickCounter > TIM_REPEAT)       
        {
                TickCounter = 0;   // Counts up to a single second, then increase the RunTime
        }
        
        // Time the AUDIO
        if (TickCounter == TIM_SAMPLESTART)  mp3.playWithVolume(1,26);  // Play the first mp3 on the card at volume 26 (max is 30)
        if (TickCounter == TIM_SAMPLESTART+TIM_SAMPLELENGTH) mp3.stopPlay(); // Stop playing after the show is over

        // Time the break lights
                // Time the hazard lights

        if (TickCounter % FLASH1SPEED == 0)             // Every half second we invert the hazard lights
        {
              digitalWrite(FLASH1, !digitalRead(FLASH1));
        }
        if (TickCounter % FLASH2SPEED == 0)             // Every half second we invert the hazard lights
        {
              digitalWrite(FLASH2, !digitalRead(FLASH2));
        }
        if (TickCounter % FLASH3SPEED == 0)             // Every half second we invert the hazard lights
        {
              digitalWrite(FLASH3, !digitalRead(FLASH3));
        }
        if (TickCounter % FLASH4SPEED == 0)             // Every half second we invert the hazard lights
        {
              digitalWrite(FLASH4, !digitalRead(FLASH4));
        }
        if (TickCounter % FLASHHAZARDSPEED == 0)             // Every half second we invert the hazard lights
        {
              digitalWrite(FLASHLIGHTS1, !digitalRead(FLASHLIGHTS1));
              digitalWrite(FLASHLIGHTS2, !digitalRead(FLASHLIGHTS2));
        }

        delay(1); // Added this dummy delay() to make sure the code takes more than 1 ms to execute.
}
