 /*--------------------------------------------------------++
|| DD_P4 (both lights activation + large reward + with FC) ||
++--------------------------------------------------------*/



/*
 * 10/8/2020 Update: added large reward port + forced choice (FC) + free choice trials (where both L and R port turn on during the trial window)
 * 
 * 1/15/2020 Update: Included # of incorrect poke counts during trial window
 * (when reponse ports light up)
 *
 * PARADIGM:
 *
 * DESCRIPTION: Refer to Logic Flow Diagram for more detailed information
 *
 *              1.
 *              2.
 *              3.
 *              4.
 *              5.
 *              6.
 *              7.
 *
 *
 * KEY CHARACTERISTICS:
 *
 *
 *
 *
 * PUSH BUTTON BEHAVIOR: single short push toggles solenoid valve operation
 *                       push and hold (for 2s) toggles LED operation
 *
 * EVENT CODE STATUS: Compatible
 *
 * EDIT HISTORY:
 *
 *
 * COPYRIGHT: SC/JHL/ Nautiyal Lab
 *            + Open Source Arduino Forum
 *
 *  ## CHANGED IN G5 (group 5)
 * NOTES::: automatically subtracts one from the final count for relevant windows!!
 *     1) incorrect pokes before trial initiation
 *     2) Pokes during the trial window
 * (CODE: xx29)
 * (CODE: xx89) --> currently (2/14/20) not included in analysis
 */


#include "globals_main.h"
#include "globals_button.h"
#include "globals_time.h"
#include "random_generator.h"
#include "IR.h"
#include "begin_trial.h"
#include "push_button.h"
#include "m_port_operate.h"
#include "l_port_operate.h" //used in FC trials
#include "r_port_operate.h" //used in FC trials 

String paradigm = "G5_P3_v3";

// * * * * * * * * * * * S E T U P * * * * * * * * * * * * * * *

void setup(){

    // randomize the start of the random generator
    randomSeed(analogRead(0));

    pinMode (button_pin, INPUT);

    // solenoids initialization
    for (byte i = 4; i <=6; i++) { pinMode(i, OUTPUT);}

    // LED initialization
    for (byte i = 8; i <=10; i++) { pinMode(i, OUTPUT);}

    // IRs initialization
    for (byte i = 11; i <=13; i++) { pinMode(i, INPUT);}
    // pinMode (9, INPUT);  // modified pin #s
    // pinMode (11, INPUT); // modified pin #s
    // pinMode (13, INPUT); // modified pin #s

    Serial.begin(115200);
    Serial.print("Paradigm:");
    Serial.println(paradigm);

    // initialize initiate_trial_window to true to start program
    initiate_new_trial = true;

    // initiate random_idx to pick the first idx for Inter-Trial Interval
    random_idx = random_iti();

    // Serial.print("idx: ");
    // Serial.println(random_idx);
    // Serial.println(iti_interval[random_idx]);
}

// * * * * * * * * * * * S E T U P * * * * * * * * * * * * * * *

// * * * * * * * * * * * L O O P * * * * * * * * * * * * * * * *

void loop() {

    // activate_button();
    record_IR();
    begin_new_trial();

    // Once begin_new_trial finishes running, should be right HERE!
    // Next Step is to pick the activation port!!!

    // NEED TO RESET current_port AT ONE POINT IN CODE,
    // SO THAT below if statements don't get evaluated!!!
    // (otherwise, current_port will always be either 1 or 2 --> leading to unexpected results!)

    //count is incremented here to ensure the first 10 trials of the paradigm are forced choice 

    if (current_port == 1) { //a FC trial with left port on (large reward) 
      // delay(1);
      activate_left_led();
      activate_left_sol();
      //count = count + 1; 

      }

    else if (current_port == 2) { //a FC trial with right port on (small reward) 
      // delay(1);
      activate_right_led();
      activate_right_sol();
      //count = count + 1; 
      //Serial.println(count); 

      }
    else if (current_port > 2) { //a standard P4 trial with both ports on --> left port is large reward 
      // delay(1);
      activate_middle_led();
      activate_middle_sol();
      //count = count + 1; 
      //Serial.println(count); 
      }
}

// * * * * * * * * * * * L O O P * * * * * * * * * * * * * * * *
