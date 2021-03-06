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
         if ((valid_response_window) && (led_state_R == LOW)){

           led_state_R = HIGH;
           digitalWrite(port_led_R, led_state_R);

           led_on_time = millis();
           // Serial.print(F("Valid led On:"));

           Serial.print(F("92171:")); // Timestamp for FC Valid Light On
           Serial.println(led_on_time);

           // RESET PORT COUNTER to count # of pokes during response window 
           left_port_counter = 0;
           mid_port_counter = 0;  
           right_port_counter = 0;

           }   // led is on HIGH

         // (VALID POKE - LIGHT OFF) if correct nose poke during valid trial window -> start reward window (type 1 = yes reward)
         if ((valid_response_window) && (led_state_R == HIGH) && (poke_in_R) && (right_port_counter > 0)) {

           valid_response_window = false;  // valid response window ends and reward window starts

           led_state_R = LOW;
           digitalWrite(port_led_R, led_state_R);

           led_off_time = millis();
           // Serial.print(F("Valid led Off:"));

           Serial.print(F("92170:"));    // Timestamp for FC Valid Light Off
           Serial.println(led_off_time);

           // POKEs during the RESPONSE Window (during when response ports light on)
           Serial.print("72589::");   // 725xx
           Serial.println(left_port_counter);
           Serial.print("82589::");    // 825xx
           Serial.println(mid_port_counter);
           Serial.print("92589::");  // 925xx
           Serial.println(right_port_counter-1);

           // Serial.print("valid_poke_timer");
           // Serial.println(valid_poke_timer);

           reward_window = true;
           reward_type = 1;     // reward_type = 1 --> dispense reward
                                // reward_type = 2 --> no reward (start iti)
         }

         // If mouse pokes in Middle or Left Port during valid trial window,
         // then turn off the right LED and and start reward window (type 2 = no reward)
         // the mid port counter must be greater than 1
         // (since we don't want the first poke (trial initiation poke) to trigger the below if statement)

         // (INVALID_POKE - LIGHT OFF)
        if ((valid_response_window) && (led_state_R == HIGH) && ((mid_port_counter > 0) || (left_port_counter > 0)) && ((poke_in_M) || (poke_in_L))) {

           valid_response_window = false;

           led_state_R = LOW;
           digitalWrite(port_led_R, led_state_R);

           led_off_time = millis();
           // Serial.print(F("Invalid led Off:"));

           Serial.print(F("92160:"));    // Timestamp for FC Invalid Light Off
           Serial.println(led_off_time);

           // POKEs during the RESPONSE Window (during when response ports light on)
           Serial.print("72589::");   // 725xx
           Serial.println(left_port_counter);
           Serial.print("82589::");    // 825xx
           Serial.println(mid_port_counter);
           Serial.print("92589::");  // 925xx
           Serial.println(right_port_counter);

           // RESET PORT COUNTER to count # of pokes during the ITI window 
           left_port_counter = 0;
           mid_port_counter = 0;   
           right_port_counter = 0;

           // Serial.print("invalid_poke_timer");
           // Serial.println(invalid_poke_timer);

           reward_window = true;
           reward_type = 2;

          }

          // (OMISSION - LIGHT OFF) // Essentially reward_type = 2 --> differentiated only by Event Code!!
          if ((valid_response_window) && (led_state_R == HIGH) && (millis() - led_on_time > led_trial_duration)) {

            valid_response_window = false;  // valid response window ends and ITI window starts

            led_state_R = LOW;
            digitalWrite(port_led_R, led_state_R);

            led_off_time = millis();
            // Serial.print(F("Invalid led Off:"));

            Serial.print(F("92540:"));    // FC Omission Trial END Timestamp (RIGHT PORT OMISSION)
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

            Serial.print(F("82271:"));     // MIDDLE SOLENOID ON Timestamp for FC Valid Solenoid On
            Serial.println(solenoid_on_time);

            // Reward Cue ON (Reward Cue Window ON)
            reward_cue_window = true;
            led_state_M = HIGH;
            digitalWrite(port_led_M, led_state_M);

            Serial.print(F("82171:"));    // Timestamp for FC Valid Light On
            Serial.println(solenoid_on_time);

            // reset port counters for pokes during REWARD window 
            left_port_counter = 0;   
            mid_port_counter = 0;
            right_port_counter = 0;
          }

          // (SOL_OFF) reward_type = 1; --> After time expires, turn off solenoid
          if ((sol_state_M == HIGH) && (millis() - solenoid_on_time > solenoid_on_duration)) {

             solenoid_active_M = false;   // can now activate the leds
             sol_state_M = LOW;
             digitalWrite(port_solenoid_M, sol_state_M);

             solenoid_off_time = millis();
             // Serial.print(F("valid_solenoid_off:"));

             Serial.print(F("82270:"));   // MIDDLE SOLENOID Timestamp for FC Valid Solenoid Off
             Serial.println(solenoid_off_time);

           }

           // REWARD CUE OFF (solenoid_on_time = reward_cue_start_time)
           if ((reward_cue_window) && (led_state_M == HIGH) && (millis() - solenoid_on_time > reward_cue_duration) ) {
             reward_cue_window = false;
             led_state_M = LOW;
             digitalWrite(port_led_M, led_state_M);

             reward_cue_off_time = millis();
             // Serial.print(F("Invalid led Off:"));

             Serial.print(F("82170:"));    // FC Valid LED OFF
             Serial.println(reward_cue_off_time);

             // start ITI here
             start_iti_window = true;
             iti_start_time = millis();
             count = count + 1;   // keeps track of the number of VALID FC trials

             // POKEs during the REWARD Window
             Serial.print("72549::");   // 725xx
             Serial.println(left_port_counter);
             Serial.print("82549::");    // 825xx
             Serial.println(mid_port_counter);
             Serial.print("92549::");  // 925xx
             Serial.println(right_port_counter);


             left_port_counter = 0;  // reset any port counters before ITI WINDOW starts 
             mid_port_counter = 0;   
             right_port_counter = 0;


           }

           // (NO REWARD) If reward_type = 2; --> (don't dispense reward but start iti timer)
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


           if (start_iti_window) {

              // Initiate Next trial after ITI expires
              if ((millis() - iti_start_time) >= iti_interval[random_idx]) {

                // # of poke counts during iti window
                Serial.print("72519::");   // 725xx
                Serial.println(left_port_counter);
                Serial.print("82519::");    // 825xx
                Serial.println(mid_port_counter);
                Serial.print("92519::");  // 925xx
                Serial.println(right_port_counter);

                // for next ITI
                get_random_iti = true;
                random_idx = random_iti();

                // prints out the next iti (the next iti duration to be used after it loops around)
                // Serial.print("idx: ");
                // Serial.println(random_idx);
                // Serial.println(iti_interval[random_idx]);

                left_port_counter = 0;  // reset any port counters before new trial starts 
                mid_port_counter = 0;   
                right_port_counter = 0;

                // RESET current_port so that leds and solenoids don't get activated again
                current_port = 0; // RESET NEEDED!!

                start_iti_window = false;
                initiate_new_trial = true;

              }
        }
    }
}
