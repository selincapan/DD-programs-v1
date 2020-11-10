/*--------------------------------------------------------++
||    Operate Middle Port (for free choice trials)        ||
++--------------------------------------------------------*/

// Solenoid Reward will only be from the middle port!
// left port is the large reward port! 

void activate_middle_led() {

     static unsigned long led_on_time = 0;
     static unsigned long led_off_time = 0;
	
     // static unsigned long valid_poke_timer = 0;
     // static unsigned long invalid_poke_timer = 0;

      // solenoid_active_L becomes false only after ITI time expires
      // Valid trial has started: Turn on left LED

     if (solenoid_active_M == false) {

       // (PORT LIGHT ON) Turn on both LEDs if it is a valid trial
         if ((valid_trial_window) && (led_state_L == LOW) && (led_state_R == LOW)){

           led_state_L = HIGH;
           led_state_R = HIGH;
           digitalWrite(port_led_L, led_state_L);
           digitalWrite(port_led_R, led_state_R);

           led_on_time = millis();
           // Serial.print(F("Valid led On:"));


           // Both Stimulus Lights ON
           Serial.print(F("6171:")); // Timestamp for Valid Light On (CHANGE EVENT CODE HERE) 
           Serial.println(led_on_time);

           // RESET PORT COUNTER to count # of incorrect pokes during trial window (when both response port lights come on)
           left_port_counter = 0;
           mid_port_counter = 0;  
           right_port_counter = 0;

           }   // led is on HIGH


         // (VALID POKE - LIGHT OFF) if correct nose poke during valid trial window -> start reward window (type 1 = yes reward)
         // BOTH LIGHTS ON, poke in LEFT PORT
         if ((valid_trial_window) && (led_state_L == HIGH) && (led_state_R == HIGH) && (poke_in_L) && (left_port_counter > 0)) {

		       valid_trial_window = false;  // valid trial window ends and response window starts

           led_state_L = LOW;
           led_state_R = LOW;
           digitalWrite(port_led_L, led_state_L);
           digitalWrite(port_led_R, led_state_R);

           led_off_time = millis();
           // Serial.print(F("Valid led Off:"));

           Serial.print(F("7170:"));    // Timestamp for Valid Light Off
           Serial.println(led_off_time);

           // Serial.print("valid_poke_timer");
           // Serial.println(valid_poke_timer);


           // CHANGED HERE (2/14/20) --> automatically subtracts one from relevant port to account for poke for reward
           // POKEs during the TRIAL Window 
           Serial.print("7589::");   // 75xx
           Serial.println(left_port_counter - 1);
           Serial.print("8589::");    // 85xx
           Serial.println(mid_port_counter);
           Serial.print("9589::");  // 95xx
           Serial.println(right_port_counter);

           i = 1;                // 4 - i (i.e. 3) drops of reward will be given 
           reward_window = true;
           reward_type = 1;     // reward_type = 1 --> dispense reward
                                // reward_type = 2 --> no reward (start iti)
         }

         // (VALID POKE - LIGHT OFF) if correct nose poke during valid trial window -> start reward window (type 1 = yes reward)
         // BOTH LIGHTS ON, poke in RIGHT PORT
         if ((valid_trial_window) && (led_state_L == HIGH) && (led_state_R == HIGH) && (poke_in_R) && (right_port_counter > 0)) {

           valid_trial_window = false;  // valid trial window ends and response window starts

           led_state_L = LOW;  // turn off LEDs
           led_state_R = LOW;
           digitalWrite(port_led_L, led_state_L);
           digitalWrite(port_led_R, led_state_R);

           led_off_time = millis();
           // Serial.print(F("Valid led Off:"));

           Serial.print(F("9170:"));    // Timestamp for Valid Light Off
           Serial.println(led_off_time);

           // Serial.print("valid_poke_timer");
           // Serial.println(valid_poke_timer);

           // CHANGED HERE (2/14/20) --> automatically subtracts one from relevant port to account for poke for reward
            // POKEs during the TRIAL Window (during when response ports light on)
           Serial.print("7589::");   // 75xx
           Serial.println(left_port_counter);
           Serial.print("8589::");    // 85xx
           Serial.println(mid_port_counter);
           Serial.print("9589::");  // 95xx
           Serial.println(right_port_counter - 1);

           i = 3;                // 4 - i (i.e. 1) drops of reward will be given 
           give_small_reward = true;  
           reward_window = true;
           reward_type = 1;     // reward_type = 1 --> dispense reward
                                // reward_type = 2 --> no reward (start iti)
         }

       // NO INVALID POKE NOR OMISSION TRIAL!!!

       }
}

void activate_middle_sol () {

    // SOLENOID ON time will be used to time REWARD_CUE (since they happen concurrently)
    static unsigned long solenoid_on_time = 0;
    static unsigned long solenoid_off_time = 0;

    static unsigned long iti_start_time = 0;
    static unsigned long reward_cue_off_time = 0;

    // solenoid only operates once led is turned off (have to keep solenoid component OFF normally)
    if (led_state_L == LOW) {

        // (SOL_ON) reward_type = 1;  --> dispense reward (need to go into this loop only once! (bc of timer))
        // REWARD_CUE_ON
        if ((reward_window) && (reward_type == 1)) {
            reward_window = false;
            reward_type = 0; // toggle back to default value

            //* solenoid on time = reward cue start time! *//
            solenoid_on_time = millis();  // will only be recorded at the begining; not for each drop 
            Serial.print(F("8271:"));     // MIDDLE SOLENOID Timestamp for Valid Solenoid On
            Serial.println(solenoid_on_time);
            
			      while (i <= 3) { //value of i determined when here is a left or right poke 
            	solenoid_active_M = true;  // to prevent from going into the led loop
            	sol_state_M = HIGH;
            	digitalWrite(port_solenoid_M, sol_state_M);
				
				      if ((i == 1) || (give_small_reward && i == 3)) {
            		
            		// RESET PORT COUNTERS for pokes during reward window
                // only reset in the first iteration of the loop to get an accurate count! 
            		left_port_counter = 0;
           			mid_port_counter = 0;  
            		right_port_counter = 0;

               
                // Reward Cue ON (Reward Cue Window ON)
                reward_cue_window = true;
                led_state_M = HIGH;
                digitalWrite(port_led_M, led_state_M);
				      }

	
          		// (SOL_OFF) reward_type = 1; --> After time for 1 drop expires, turn off solenoid 
              // give 3 drops if you are giving large reward 
          		if ((give_small_reward == false) && (sol_state_M == HIGH) && (millis() - solenoid_on_time > (i * solenoid_on_duration))) {

             		solenoid_active_M = false;   // can now activate the leds
             		sol_state_M = LOW;
             		digitalWrite(port_solenoid_M, sol_state_M);
                i = i + 1; 
                delay(5);                   //puts a 5 ms delay between each drop to ensure the "clicks" are distinguishable
          		}
             
              // (SOL_OFF) reward_type = 1; --> After time expires, turn off solenoid 
              //since small reward trials start with i == 3; we can't use i * solenoid_on_duration for reward dispensing
              if ((give_small_reward == true) && (sol_state_M == HIGH) && (millis() - solenoid_on_time > (solenoid_on_duration))) {
                solenoid_active_M = false;   // can now activate the leds
                sol_state_M = LOW;
                digitalWrite(port_solenoid_M, sol_state_M);
                i = i + 1;
                give_small_reward = false;  
				      }
			       }

             //once the while loop is over, record when the solenoid turned off 
             solenoid_off_time = millis();
             // Serial.print(F("valid_solenoid_off:"));

             Serial.print(F("8270:"));   // Timestamp for Valid Solenoid Off
             Serial.println(solenoid_off_time);

           }
  

           // REWARD CUE OFF (solenoid_on_time = reward_cue_start_time)
           if ((reward_cue_window) && (led_state_M == HIGH) && (millis() - solenoid_on_time > reward_cue_duration) ) {
             reward_cue_window = false;
             led_state_M = LOW;
             digitalWrite(port_led_M, led_state_M);

             reward_cue_off_time = millis();
             // Serial.print(F("Invalid led Off:"));

             Serial.print(F("8170:"));    // Omission Trial END Timestamp (M PORT OMISSION)
             Serial.println(reward_cue_off_time);

             // start ITI here
             start_iti_window = true;
             iti_start_time = millis();

             // POKEs during the REWARD Window
             Serial.print("7549::");   // 75xx
             Serial.println(left_port_counter);
             Serial.print("8549::");    // 85xx
             Serial.println(mid_port_counter);
             Serial.print("9549::");  // 95xx
             Serial.println(right_port_counter);

             left_port_counter = 0;  // reset any port counters before ITI WINDOW starts 
             mid_port_counter = 0;   
             right_port_counter = 0;

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

                get_random_iti = true;
                random_idx = random_iti();

                // prints out the next iti (the next iti duration to be used after it loops around)
                // Serial.print("idx: ");
                // Serial.println(random_idx);
                // Serial.println(iti_interval[random_idx]);


                // RESETTING counters before new trial starts 
                left_port_counter = 0;  // reset any port counters before TRIAL WINDOW starts (so that any invalid pokes get resetted during iti window)
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
