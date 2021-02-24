/*--------------------------------------------------------------------------------------------*/
/* Truck led & motor simulation.                                                              */
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
#define HAZARDLIGHTS   24
#define REVLIGHTS      25
#define BREAKLIGHTS    26 
#define CONSTLIGHTS    27
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
#define TIM_STARTCONST        3*RUN_FREQ
#define TIM_STOPCONST         57*RUN_FREQ
#define TIM_BREAK1_ON         14*RUN_FREQ
#define TIM_BREAK1_OFF        15*RUN_FREQ
#define TIM_BREAK2_ON         30*RUN_FREQ
#define TIM_BREAK2_OFF        31*RUN_FREQ
#define TIM_BREAK3_ON         39*RUN_FREQ
#define TIM_BREAK3_OFF        40*RUN_FREQ
#define TIM_BREAK4_ON         43*RUN_FREQ
#define TIM_BREAK4_OFF        44*RUN_FREQ
#define TIM_BREAK5_ON         50*RUN_FREQ
#define TIM_BREAK5_OFF        51*RUN_FREQ
#define TIM_REV_START         51*RUN_FREQ
#define TIM_REV_STOP          66*RUN_FREQ
#define TIM_REPEAT            90*RUN_FREQ       // Rewind at 90 seconds (max. value is 655535 = 327 seconds = a little over 5 minutes)

#define FLASHSPEED            50                // 50 = 0,5 sec     100 = 1 sec      25 = 0,25 sec etc 

// This runs only once when powering on
void setup()
{
        digitalWrite(HAZARDLIGHTS, LOW);     // Hazard lights led group starts OFF
        digitalWrite(REVLIGHTS,    LOW);     // Reverse lights led group starts OFF
        digitalWrite(BREAKLIGHTS,  LOW);     // Brake light led group starts OFF
        digitalWrite(CONSTLIGHTS,  LOW);     // Contant-on led group starts OFF
        
        // All led groups as OUTPUT
        pinMode (HAZARDLIGHTS,   OUTPUT);
        pinMode (REVLIGHTS,   OUTPUT);
        pinMode (BREAKLIGHTS,   OUTPUT);
        pinMode (CONSTLIGHTS,   OUTPUT);
        
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
        if ( (TickCounter == TIM_BREAK1_ON) ||
             (TickCounter == TIM_BREAK2_ON) ||
             (TickCounter == TIM_BREAK3_ON) ||
             (TickCounter == TIM_BREAK4_ON) ||
             (TickCounter == TIM_BREAK5_ON)    )
        {
                digitalWrite(BREAKLIGHTS, HIGH);
        }
        if ( (TickCounter == TIM_BREAK1_OFF) ||
             (TickCounter == TIM_BREAK2_OFF) ||
             (TickCounter == TIM_BREAK3_OFF) ||
             (TickCounter == TIM_BREAK4_OFF) ||
             (TickCounter == TIM_BREAK5_OFF)    )
        {
                digitalWrite(BREAKLIGHTS, LOW);
        }
        
        // Time the reverse lights
        if ( TickCounter == TIM_REV_START )
        {
             digitalWrite(REVLIGHTS, HIGH);   
        }
        if ( TickCounter == TIM_REV_STOP )
        {
             digitalWrite(REVLIGHTS, LOW);   
        }

        // Time the constant lights
        if ( TickCounter == TIM_STARTCONST )
        {
             digitalWrite(CONSTLIGHTS, HIGH);   
        }
        if ( TickCounter == TIM_STOPCONST )
        {
             digitalWrite(CONSTLIGHTS, LOW);   
        }
        
        // Time the hazard lights
        if (TickCounter % FLASHSPEED == 0)             // Every half second we invert the hazard lights
        {
              digitalWrite(HAZARDLIGHTS, !digitalRead(HAZARDLIGHTS));
        }

        delay(1); // Added this dummy delay() to make sure the code takes more than 1 ms to execute.
}
