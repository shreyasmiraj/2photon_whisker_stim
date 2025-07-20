#include <Servo.h>

void reset (Servo* whisker_servo, unsigned long* lastT, unsigned long currentT, int* current_position, bool* indicate_led,
            int* start_position, int* target_position_A_micro, int* target_position_B_micro,
            bool* trigger_flag, unsigned long* trigger_onset,
            bool* start_flag, bool* stop_flag, bool* reset_flag, int* mode) {
  /*****************************
  * Reset to initial setup state
  * ****************************
  */
  whisker_servo->writeMicroseconds(DEFAULT_POSITION);
  *lastT = currentT;
	*current_position = DEFAULT_POSITION;
  *start_position = DEFAULT_POSITION;
  *target_position_A_micro = *start_position - OSCILLATION_AMPLITUDE;
  *target_position_B_micro = *start_position + OSCILLATION_AMPLITUDE;
  *indicate_led = true;
  *trigger_flag = false;
  *start_flag = false;
  *stop_flag = true;
  *reset_flag = false;
  *trigger_onset = 0;
  *mode = 0;
}

int mode_detect(int trigger_input, bool* trigger_flag, unsigned long* trigger_onset, 
                bool* start_flag, bool* stop_flag, bool* reset_flag,
                unsigned long currentT) {
  /*********************************
  * Detect current mode of operation
  * ********************************
  * trigger_input: int variable that has value of current trigger input
  * trigger_flag: pointer to boolean variable that has value of trigger state - set to true if trigger is high else false
  * start_flag: pointer to boolean variable that has truth value of whether start trigger was received
  * start_flag: pointer to boolean variable that has truth value of whether stop trigger was received
  * trigger_onset: pointer to unsigned long variable that stores onset time of trigger_input being high or 1 in microseconds
  * currentT: unsigned long variable of current time in microseconds
  * 
  * returns: int with values 0,1,2 for inactive, active and reset respectively
  */
  unsigned long trigger_onset_duration=0;
  bool trigger_edge_flag = false;

  //if trigger input is high and trigger flag hasn't been set then this is start of a trigger (note this could be start of start or stop or any trigger or non trigger high pulse)
  if (trigger_input) {
    if (!*trigger_flag) {
      *trigger_flag = true;
      *trigger_onset = currentT;
    }
  }
  else {
    if (*trigger_flag) {
      trigger_edge_flag = true;
      *trigger_flag = false;
    }
  }
  
  //if trigger edge has occured, check time since trigger is high
  if (trigger_edge_flag) {
    //check if conditions are met for current trigger to qualify as start/stop trigger and set start/stop state accordingly
    trigger_onset_duration = currentT - *trigger_onset;
    trigger_edge_flag = false;
    trigger_onset = 0;
    Serial.println(int(trigger_onset_duration));
    //check type of trigger
    if (trigger_onset_duration > MIN_TRIGGER_WIDTH && trigger_onset_duration < MAX_TRIGGER_WIDTH) {
      *start_flag = !*start_flag;
      return *start_flag; //0 inactive, 1 active
    }
    if (trigger_onset_duration > RESET_MIN_TRIGGER_WIDTH && trigger_onset_duration < RESET_MAX_TRIGGER_WIDTH) {
      *reset_flag = true;
      return 2; //reset
    }
    if (trigger_onset_duration > POS_MIN_FINE_TRIGGER_WIDTH && trigger_onset_duration < POS_MAX_FINE_TRIGGER_WIDTH) {
      return 3; //positive offset fine
    }
    if (trigger_onset_duration > NEG_MIN_FINE_TRIGGER_WIDTH && trigger_onset_duration < NEG_MAX_FINE_TRIGGER_WIDTH) {
      return 4; //negative offset fine
    }
    if (trigger_onset_duration > POS_MIN_COARSE_TRIGGER_WIDTH && trigger_onset_duration < POS_MAX_COARSE_TRIGGER_WIDTH) {
      return 5; //positive offset coarse
    }
    if (trigger_onset_duration > NEG_MIN_COARSE_TRIGGER_WIDTH && trigger_onset_duration < NEG_MAX_COARSE_TRIGGER_WIDTH) {
      return 6; //negative offset coarse
    }
  }
  if(*start_flag){
    return *start_flag;
  }
  return 0; //0/inactive by default
}

int read_input(const int INPUT_PIN, int analog_low_threshold = 50, int analog_high_threshold = 973) {
  /****************************************************************
  * read and parse input based on input type i.e analog or digital.
  * if analog convert the analog signal to 0 or based on threshold
  * ***************************************************************
  * INPUT_PIN: variable of type const int representing the pin for input (0-53 is digital, 54-69 is analog)
  * analog_low_threshold: variable of type int representing the limit below which analog input value is considered as low
  * analog_high_threshold: variable of type int representing the limit above which anaolg input value is considered as high
  * limits can be thought of as tolerance, by default it is set to 4.88% of analog read range (i.e 4.88% of 0-5V mapped onto 0-1023 - low < 50, high > 973)
  */
  //check if input pin is analog or digital and process input accordingly
  if (INPUT_PIN > 53) {
    int input_val = analogRead(INPUT_PIN);

    if (input_val >= analog_high_threshold) {
      return 1;
    }
    else if (input_val <= analog_low_threshold) {
      return 0;
    }
  }
  else {
    return digitalRead(INPUT_PIN);
  }
}

void execute_active_mode(Servo* whisker_servo, unsigned long currentT, unsigned long* lastT,
                         int* current_position, int* target_position_A_micro, int* target_position_B_micro,
                         bool indicate_led) {
  /**********************************
  * active mode - whisker stimulation
  * *********************************
  */
  unsigned long deltaT = currentT - *lastT;
    //  move to target_positon_A
    if (deltaT < ACTIVE_INTERVAL) {
      if (*current_position != *target_position_A_micro) {
        // whisker_servo.write(target_position_A);
        whisker_servo->writeMicroseconds(*target_position_A_micro);
        *current_position = *target_position_A_micro;
        if (indicate_led) {
          analogWrite(LED_INDICATOR, 255);
        }
      }
    }
    //move to target_positon_B
    if (deltaT > ACTIVE_INTERVAL && deltaT < 2*ACTIVE_INTERVAL) {
      if (*current_position != *target_position_B_micro){
        // whisker_servo.write(target_position_B);
        whisker_servo->writeMicroseconds(*target_position_B_micro);
        *current_position = *target_position_B_micro;
        if (indicate_led){
          analogWrite(LED_INDICATOR, 0);
        }
      }
    }

    if (deltaT >= 2*ACTIVE_INTERVAL) {
      *lastT = currentT;
    }
}

void execute_inactive_mode(unsigned long currentT, unsigned long* lastT) {
  /**********************************************************
  * Inactive mode - periodic blinking on arduino - 0.5Hz freq
  * *********************************************************
  */
  unsigned long deltaT = currentT - *lastT;
  if (deltaT < INACTIVE_INTERVAL) {
    analogWrite(LED_INDICATOR, 255);
  }
  if (deltaT > INACTIVE_INTERVAL && deltaT < 2*INACTIVE_INTERVAL) {
    analogWrite(LED_INDICATOR, 0);
  }
  if (deltaT >= 2*INACTIVE_INTERVAL) {
    *lastT = currentT;
  }
}

void positive_offset(int type, Servo* whisker_servo, int* start_position, int* current_position, int* target_position_A_micro, int* target_position_B_micro) {
  //0 for fine, 1 for coarse
  int offset_value;
  if(type) {
    offset_value = OFFSET_COARSE;
  }
  else {
    offset_value = OFFSET_FINE;
  }
  *start_position = min(*start_position + offset_value, 2400-OSCILLATION_AMPLITUDE);
  *target_position_A_micro = *start_position - OSCILLATION_AMPLITUDE;
  *target_position_B_micro = *start_position + OSCILLATION_AMPLITUDE;
  whisker_servo->writeMicroseconds(*start_position);
  *current_position = *start_position;
}

void negative_offset(int type, Servo* whisker_servo, int* start_position, int* current_position, int* target_position_A_micro, int* target_position_B_micro) {
  //0 for fine, 1 for coarse
  int offset_value;
  if(type) {
    offset_value = OFFSET_COARSE;
  }
  else {
    offset_value = OFFSET_FINE;
  }
  *start_position = max(*start_position - offset_value, 544+OSCILLATION_AMPLITUDE);
  *target_position_A_micro = *start_position - OSCILLATION_AMPLITUDE;
  *target_position_B_micro = *start_position + OSCILLATION_AMPLITUDE;
  whisker_servo->writeMicroseconds(*start_position);
  *current_position = *start_position;
}
