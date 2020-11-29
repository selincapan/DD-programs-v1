/*--------------------------------------------------------++
||   Operate Right Port in FC trials (Large Reward Port)   ||
++--------------------------------------------------------*/

// Solenoid Reward will only be from the middle port!

static unsigned long FC_delay_start_time = 0; 

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

           // RESET PORT COUNTER to count # of incorrect pokes during response window (when response port lights come on)
           left_port_counter = 0;
           mid_port_counter = 0;   
           right_port_counter = 0;
           
           }   // led is on HIGH


         // (VALID POKE - LIGHT OFF) if correct nose poke during valid trial window -> start reward window (type 1 = yes reward)
         if ((valid_response_window) && (led_state_R == HIGH) && (poke_in_R) && (right_port_counter > 0)) {

           valid_response_window = false;  // valid response window ends and reward window starts

           led_state_R = LOW;           // turn off left LED 
           digitalWrite(port_led_R, led_state_R);

           led_off_time = millis();
           // Serial.print(F("Valid led Off:"));

           Serial.print(F("92170:"));    // Timestamp for FC Valid Light Off
           Serial.println(led_off_time);

           // CHANGED HERE (2/14/20) --> automatically subtracts one from relevant port to account for poke for reward
           // POKEs during the RESPONSE Window (during when response port light is on)           
            Serial.print("72589::");   // 75xx
            Serial.println(left_port_counter);
            Serial.print("82589::");    // 85xx
            Serial.println(mid_port_counter);
            Serial.print("92589::");  // 95xx
            Serial.println(right_port_counter-1);

           // RESET PORT COUNTER to count # of pokes during the reward delay 
           left_port_counter = 0;
           mid_port_counter = 0;   
           right_port_counter = 0; 

           // Serial.print("valid_poke_timer");
           // Serial.println(valid_poke_timer);

           initiate_reward_delay = true;   // start the delay in reward
           FC_delay_start_time = millis(); //record when delay window started  
         
           // Reward Cue ON (reward cue is on both during the reward delay and the reward window)
           // won't be counted toward the reward cue period! (reward cue period begins when the reward is dispensed) 
           led_state_M = HIGH;
           digitalWrite(port_led_M, led_state_M);

           // reward window is NOT true here --> true following the delay! 
                                
         }

         // If mouse pokes in Middle or Left Port during valid response window,
         // then turn off the left LED and and start reward window (type 2 = no reward)
         // * * * * * * * * * *

         // (INVALID_POKE - LIGHT OFF)
        if ((valid_response_window) && (led_state_R == HIGH) && ((mid_port_counter > 0) || (left_port_counter > 0)) && ((poke_in_M) || (poke_in_L))) {

           valid_response_window = false;   // valid response window has ended 

           led_state_R = LOW;           // turn off right LED 
           digitalWrite(port_led_R, led_state_R);

           led_off_time = millis();
           // Serial.print(F("Invalid led Off:"));

           Serial.print(F("92160:"));    // Timestamp for FC Invalid Light Off
           Serial.println(led_off_time);

           // Serial.print("invalid_poke_timer");
           // Serial.println(invalid_poke_timer);

           // CHANGED HERE (2/14/20) --> automatically subtracts one from relevant port to account for poke for reward
           // POKEs during the RESPONSE Window (during when response port light is on)           
           Serial.print("72589::");   // 725xx
           Serial.println(left_port_counter);
           Serial.print("82589::");    // 825xx
           Serial.println(mid_port_counter);
           Serial.print("92589::");  // 925xx
           Serial.println(right_port_counter);

           // RESET PORT COUNTER to count # of pokes during iti
           left_port_counter = 0;
           mid_port_counter = 0;   
           right_port_counter = 0;


           reward_window = true;
           reward_type = 2;

          }

        // OMISSION TRIAL
        // (NO RESPONSE - LIGHT OFF) // Essentially reward_type = 2 --> differentiated only by Event Code!!
        if ((valid_response_window) && (led_state_R == HIGH) && (millis() - led_on_time > led_trial_duration)) {

          valid_response_window = false;  // valid trial window ends and ITI window starts 

          led_state_R = LOW;
          digitalWrite(port_led_R, led_state_R);

          led_off_time = millis();
          // Serial.print(F("Invalid led Off:"));

          Serial.print(F("92540:"));    // FC Omission Trial END Timestamp (LEFT PORT OMISSION)
          Serial.println(led_off_time);

          // start reward_window (type=2 (no reward))
          reward_window = true;
          reward_type = 2;

        }

       }
}

void activate_right_sol () {

    // SOLENOID ON time will be used to time REWARD_CUE (since they happen concurrently)
    static unsigned long solenoid_on_time = 0;
    static unsigned long solenoid_off_time = 0;

    static unsigned long iti_start_time = 0;
    static unsigned long reward_cue_off_time = 0; 

  
    // solenoid only operates once led is turned off (have to keep solenoid component OFF normally)
    if (led_state_R == LOW) {
       
        if (initiate_reward_delay == true && ((millis() - FC_delay_start_time) >= reward_delay_duration)) {
          
          initiate_reward_delay = false; 

          // # of poke counts during delay window (between trial window and reward dispensing)
          Serial.print(F("72559::"));   // 725xx  (left counter during FC delay window)
          Serial.println(left_port_counter); 
          Serial.print(F("82559::"));    // 825xx (middle counter during FC delay window)
          Serial.println(mid_port_counter);
          Serial.print(F("92559::"));  // 925xx    (right counter during FC delay window)
          Serial.println(right_port_counter);

          // RESET poke_counter for reward window 
          left_port_counter = 0;
          mid_port_counter = 0;
          right_port_counter = 0;

          // DISPENSE REWARD (now that the delay is over)
          reward_window = true; 
          reward_type = 1;

        }

        // (SOL_ON) reward_type = 1;  --> dispense reward (need to go into this loop only once! (bc of timer))
        // REWARD_CUE_ON
        // only go into this statement if the reward delay has ended! (reward_window is false until delay ends)
        if ((reward_window) && (reward_type == 1)) {
            reward_window = false;
            reward_type = 0; // toggle back to default value
            i = 1;           // will count the number of drops given 

            //* solenoid on time = reward cue start time! *//
            solenoid_on_time = millis();   // will only be recorded at the begining; not for each drop 
            Serial.print(F("82271:"));     // MIDDLE SOLENOID Timestamp for FC Valid Solenoid On 
            Serial.println(solenoid_on_time);

            // Reward Cue ON (Reward Cue Window ON)
            reward_cue_window = true;
            led_state_M = HIGH;
            digitalWrite(port_led_M, led_state_M);

            Serial.print(F("82171:"));    // Timestamp for FC Valid Light On
            Serial.println(solenoid_on_time);
            
            // REWARD WINDOW COUNTS (reset to count the number of reward window pokes)             
            left_port_counter = 0;
            mid_port_counter = 0;   
            right_port_counter = 0;


            while (i <= 3) {  // changing 3 here can be used to change the number of reward drops dispensed 
            	solenoid_active_M = true;  // to prevent from going into the led loop
            	sol_state_M = HIGH;
            	digitalWrite(port_solenoid_M, sol_state_M);

              // (SOL_OFF) reward_type = 1; --> After time for 1 drop expires, turn off solenoid
              // will be executed 3 times  
          		if ((sol_state_M == HIGH) && (millis() - solenoid_on_time > (i * solenoid_on_duration))) {

             		solenoid_active_M = false;   
             		sol_state_M = LOW;
             		digitalWrite(port_solenoid_M, sol_state_M);

                i = i + 1; 

                if (i != 3) {
                  Serial.print(F("82270:")); // FC M_sol_Valid_OFF 
                  Serial.println(millis()); 
                }
                
                delay(5);   //puts a 5 ms delay between each drop to ensure the "clicks" are distinguishable 
                
             	 }
            }

             solenoid_off_time = millis();  // record when the solenoid turns off for the last time 
             // Serial.print(F("valid_solenoid_off:"));

             Serial.print(F("82270:"));   // Timestamp for Valid Solenoid Off --> can be used to count the number of drops given
             Serial.println(solenoid_off_time);

        }

           // REWARD CUE OFF (solenoid_on_time = reward_cue_start_time)
           if ((reward_cue_window) && (led_state_M == HIGH) && (millis() - solenoid_on_time > reward_cue_duration) ) {
             reward_cue_window = false; 
             led_state_M = LOW;         // turn off reward cue 
             digitalWrite(port_led_M, led_state_M);

             reward_cue_off_time = millis();
             // Serial.print(F("Invalid led Off:"));

             Serial.print(F("82170:"));    // FC Valid LED off 
             Serial.println(reward_cue_off_time);

             // start ITI here
             start_iti_window = true;
             iti_start_time = millis();
             count = count + 1;         // count counts the number of FC trials 

             // POKEs during the REWARD Window (pokes between when the reward has been dispensed ("click heard") and reward cue turns off)  
             Serial.print("72549::");   // 725xx
             Serial.println(left_port_counter);
             Serial.print("82549::");    // 825xx
             Serial.println(mid_port_counter);
             Serial.print("92549::");  // 925xx
             Serial.println(right_port_counter);

             // RESET counter for poke counts during iti window
             left_port_counter = 0;  
             mid_port_counter = 0;   
             right_port_counter = 0;


           }

           // (NO REWARD) If reward_type = 2; --> (don't dispense reward but start iti timer)
           if ((reward_window) && (reward_type == 2)) {
             reward_window = false;
             reward_type = 0;   // reset reward_type

             solenoid_active_M = false;  

             start_iti_window = true;
             iti_start_time = millis();

             // NO NEED TO PRINT OUT invalid_solenoid (no reward) OR iti_start_time
             // Serial.print(F("invalid_solenoid: "));
             // Serial.print(F("7270:"));   // Timestamp for Invalid Solenoid Off
             // Serial.println(iti_start_time);

             // RESET counter for poke counts during iti window
             left_port_counter = 0;  
             mid_port_counter = 0;   
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

                get_random_iti = true;
                random_idx = random_iti();

                // prints out the next iti (the next iti duration to be used after it loops around)
                // Serial.print("idx: ");
                // Serial.println(random_idx);
                // Serial.println(iti_interval[random_idx]);


                // RESETTING counters before new trial starts 
                left_port_counter = 0;  
                mid_port_counter = 0;   
                right_port_counter = 0;

                // RESET current_port so that leds and solenoids don't get activated again
                current_port = 0;  // RESET NEEDED!!

                start_iti_window = false;
                initiate_new_trial = true;

              }
        }
    }
}
