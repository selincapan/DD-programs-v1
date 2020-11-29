
int random_port(){

   if (get_random_port and count < 10) { //if it's within the first 10 trials 
      current_port = random(1,3);  // picks a number betwwen 1 and 2
                                    // 1 --> Left PORT (FC)
                                    // 2 --> Right Port (FC) 
                                    
      }
    else if (get_random_port) {
      current_port = random(1,21);  // picks a number between 1 and 20
                                    // 1 --> Left PORT FC
                                    // 2 --> Right Port FC 
                                    // any other value --> regular trial 
                                    // LEAVE 0 for null value (needed for resetting)
      }
      // Serial.print("port num: ");
      // Serial.println(current_port);

      get_random_port = false;

      return current_port;
  }

int random_iti(){

    if (get_random_iti){
        random_idx = random(0, 12); // number from 0 to 11  (12 values)
                                    // (random index value to use in the array)

    }

    get_random_iti = false;

    return random_idx;


}
