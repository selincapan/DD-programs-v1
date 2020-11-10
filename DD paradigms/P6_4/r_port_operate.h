/*--------------------------------------------------------++
||   Operate Right Port in FC trials (Small Reward Port)  ||
++--------------------------------------------------------*/

// Solenoid Reward will only be from the middle port!

void activate_right_led() {

     static unsigned long led_on_time = 0;
     static unsigned long led_off_time = 0;

     // static unsigned long valid_poke_timer = 0;
     // static unsigned long invalid_poke_timer = 0;

      // solenoid_active_M becomes false only after ITI time expires
      // Valid trial has started: Turn on left LED

     if (solenoid_active_M == false) {

       // (PORT LIGHT ON) Turn on Left LED if it is a valid trial
         if ((valid_trial_window) && (led_state_R == LOW)){

           led_state_R = HIGH;
           digitalWrite(port_led_R, led_state_R);

           led_on_time = millis();
           // Serial.print(F("Valid led On:"));

           Serial.print(F("9171:")); // Timestamp for Valid Light On
           Serial.println(led_on_time);

           // RESET PORT COUNTER to count # of pokes during trial window 
           left_port_counter = 0;
           mid_port_counter = 0;  
           right_port_counter = 0;
           
          }   // led is on HIGH

         // (VALID POKE - LIGHT OFF) if correct nose poke during valid trial window -> start reward window (type 1 = yes reward)
         if ((valid_trial_window) && (led_state_R == HIGH) && (poke_in_R) && (right_port_counter > 0)) {

           valid_trial_window = false;  // valid trial window ends and response window starts

           led_state_R = LOW;           // right LED is turned off 
           digitalWrite(port_led_R, led_state_R);

           led_off_time = millis();
           // Serial.print(F("Valid led Off:"));

           Serial.print(F("9170:"));    // Timestamp for Valid Light Off
           Serial.println(led_off_time);
           
           // POKEs during the TRIAL Window (during when response ports light on)
           Serial.print("7589::");   // 75xx
           Serial.println(left_port_counter);
           Serial.print("8589::");    // 85xx
           Serial.println(mid_port_counter);
           Serial.print("9589::");  // 95xx
           Serial.println(right_port_counter-1);

           // RESET PORT COUNTER to count # of pokes during the reward delay (0 ms for right port) 
           left_port_counter = 0;
           mid_port_counter = 0;   
           right_port_counter = 0;


           // Serial.print("valid_poke_timer");
           // Serial.println(valid_poke_timer);

           reward_window = true;
           reward_type = 1;     // reward_type = 1 --> dispense reward
                                // reward_type = 2 --> no reward (start iti)
         }

         // If mouse pokes in Middle or Left Port during valid trial window,
         // then turn off the right LED and and start reward window (type 2 = no reward)

         // (INVALID_POKE - LIGHT OFF)
        if ((valid_trial_window) && (led_state_R == HIGH) && ((mid_port_counter > 0) || (left_port_counter > 0)) && ((poke_in_M) || (poke_in_L))) {

           valid_trial_window = false;

           led_state_R = LOW;           // turn off right LED  
           digitalWrite(port_led_R, led_state_R);

           led_off_time = millis();
           // Serial.print(F("Invalid led Off:"));

           Serial.print(F("9160:"));    // Timestamp for Invalid Light Off
           Serial.println(led_off_time);

           // POKEs during the TRIAL Window (during when response ports light on)
           Serial.print("7589::");   // 75xx
           Serial.println(left_port_counter);
           Serial.print("8589::");    // 85xx
           Serial.println(mid_port_counter);
           Serial.print("9589::");  // 95xx
           Serial.println(right_port_counter);

           // RESET PORT COUNTER to count # of pokes during the reward delay (0 ms for right port) 
           left_port_counter = 0;
           mid_port_counter = 0;   
           right_port_counter = 0;

           // Serial.print("invalid_poke_timer");
           // Serial.println(invalid_poke_timer);

           reward_window = true;
           reward_type = 2;

          }

          // (OMISSION - LIGHT OFF) 
          // Essentially reward_type = 2 --> differentiated only by Event Code!!
          if ((valid_trial_window) && (led_state_R == HIGH) && (millis() - led_on_time > led_trial_duration)) {

            valid_trial_window = false;  // valid trial window ends and response window starts

            led_state_R = LOW;
            digitalWrite(port_led_R, led_state_R);

            led_off_time = millis();
            // Serial.print(F("Invalid led Off:"));

            Serial.print(F("9540:"));    // Omission Trial END Timestamp (RIGHT PORT OMISSION)
            Serial.println(led_off_time);

            // start reward_window (type=2 (no reward))
            reward_window = true;
            reward_type = 2;

          }


       }
}

void activate_right_sol () {

    static unsigned long solenoid_on_time = 0;
    static unsigned long solenoid_off_time = 0;

    static unsigned long iti_start_time = 0;
    static unsigned long reward_cue_off_time = 0;
    static unsigned long iti_delay_start_time = 0;

    // solenoid only operates once led is turned off (have to keep solenoid component OFF normally)
    if (led_state_R == LOW) {

        // (SOL_ON) reward_type = 1;  --> dispense reward (need to go into this loop only once! (bc of timer))
        if ((reward_window) && (reward_type == 1)) {
            reward_window = false;
            reward_type = 0; // toggle back to default value

            solenoid_active_M = true;  // to prevent from going into the led loop
            sol_state_M = HIGH;
            digitalWrite(port_solenoid_M, sol_state_M);

            solenoid_on_time = millis();
            // Serial.print(F("valid_solenoid:"));

            Serial.print(F("8271:"));     // MIDDLE SOLENOID ON Timestamp for Valid Solenoid On
            Serial.println(solenoid_on_time);

            // RESET RIGHT PORT COUNTER! 
            left_port_counter = 0;   
            mid_port_counter = 0;
            right_port_counter = 0;

            // Reward Cue ON (Reward Cue Window ON)
            reward_cue_window = true;
            led_state_M = HIGH;
            digitalWrite(port_led_M, led_state_M);
          }

          // (SOL_OFF) reward_type = 1; --> After time expires, turn off solenoid
          if ((sol_state_M == HIGH) && (millis() - solenoid_on_time > solenoid_on_duration)) {

             solenoid_active_M = false;   // can now activate the solenoid 
             sol_state_M = LOW;
             digitalWrite(port_solenoid_M, sol_state_M);

             solenoid_off_time = millis();
             // Serial.print(F("valid_solenoid_off:"));

             Serial.print(F("8270:"));   // MIDDLE SOLENOID Timestamp for Valid Solenoid Off
             Serial.println(solenoid_off_time);

           }

           // REWARD CUE OFF (solenoid_on_time = reward_cue_start_time)
           if ((reward_cue_window) && (led_state_M == HIGH) && (millis() - solenoid_on_time > reward_cue_duration) ) {
             reward_cue_window = false;
             led_state_M = LOW;
             digitalWrite(port_led_M, led_state_M);

             reward_cue_off_time = millis();
             // Serial.print(F("Invalid led Off:"));

             Serial.print(F("8170:"));    // Omission Trial END Timestamp (LEFT PORT OMISSION)
             Serial.println(reward_cue_off_time);

             // start ITI delay here
             initiate_iti_delay = true; // allows large and small reward trials to be of the same duration
             iti_delay_start_time = millis();
             count = count + 1;        // count counts the number of FC trials 


             // POKEs during the REWARD Window
             Serial.print("7549::");   // 75xx
             Serial.println(left_port_counter);
             Serial.print("8549::");    // 85xx
             Serial.println(mid_port_counter);
             Serial.print("9549::");  // 95xx
             Serial.println(right_port_counter);


             left_port_counter = 0;  // reset any port counters before TRIAL WINDOW starts (so that any invalid pokes get resetted during iti window)
             mid_port_counter = 0;   //  (could also be a measure of incorrect pokes during a certain TIMEFRAME)
             right_port_counter = 0;


           }

           // (NO REWARD) If reward_type = 2; --> (don't dispense reward but start iti timer)
           // don't need an ITI delay 
           if ((reward_window) && (reward_type == 2)) {
             reward_window = false;
             reward_type = 0;

             solenoid_active_M = false;

             start_iti_window = true;
             iti_start_time = millis();

             // NO NEED TO PRINT OUT invalid_solenoid (no reward) OR iti_start_time
             // Serial.print(F("invalid_solenoid: "));
             // Serial.print(F("7270:"));   // Timestamp for Inalid Solenoid Off
             // Serial.println(iti_start_time);

             // RESET counter for poke counts during iti window
             left_port_counter = 0;  // reset any port counters before TRIAL WINDOW starts (so that any invalid pokes get resetted during iti window)
             mid_port_counter = 0;   //  (could also be a measure of incorrect pokes during a certain TIMEFRAME)
             right_port_counter = 0;

           }

           if (initiate_iti_delay) { 
            
              //initiate ITI window after reward_delay_duration has passed 
              //ensures the large and small reward trials take the same amount of time to complete (choosing small reward != ability to do more trials)
              if ((millis() - iti_delay_start_time) >= reward_delay_duration) {  

                  initiate_iti_delay = false; 
                    
                  // start ITI here
                  start_iti_window = true;
                  iti_start_time = millis(); 

                  // don't reset poke counts here! These are still ITI pokes 

              }
           }

          
           if (start_iti_window) {

              // Initiate Next trial after ITI expires
              if ((millis() - iti_start_time) >= iti_interval[random_idx]) {

                // # of poke counts during iti window
                Serial.print("7519::");   // 75xx
                Serial.println(left_port_counter);
                Serial.print("8519::");    // 85xx
                Serial.println(mid_port_counter);
                Serial.print("9519::");  // 95xx
                Serial.println(right_port_counter);

                // for next ITI
                get_random_iti = true;
                random_idx = random_iti();

                // prints out the next iti (the next iti duration to be used after it loops around)
                // Serial.print("idx: ");
                // Serial.println(random_idx);
                // Serial.println(iti_interval[random_idx]);

                left_port_counter = 0;  // reset any port counters before TRIAL WINDOW starts (so that any invalid pokes get resetted during iti window)
                mid_port_counter = 0;   //  (could also be a measure of incorrect pokes during a certain TIMEFRAME)
                right_port_counter = 0;

                // RESET current_port so that leds and solenoids don't get activated again
                current_port = 0; // RESET NEEDED!!

                start_iti_window = false;
                initiate_new_trial = true;

              }
        }
    }
}
