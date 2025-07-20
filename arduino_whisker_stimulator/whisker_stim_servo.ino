#include <Servo.h>
#include "CONFIG.h"
#include "util.h"

int input_pin;
int current_position;
int start_position;
int target_position_A_micro;
int target_position_B_micro;

int mode;
bool indicate_led;
bool trigger_flag;
bool start_flag;
bool stop_flag;
bool reset_flag;
unsigned long trigger_onset;
unsigned long lastT;

Servo whisker_servo;

void setup() {
  /***********************************
  * Initialization of flags and timers
  * **********************************
  */
	Serial.begin(74880);
  pinMode(ANALOG_TRIGGER, INPUT);
  pinMode(RESET_PIN, INPUT);

	whisker_servo.attach(SERVOPORT, 544, 2400);//, 850, 2090);	//need to find out min and max of the servo in use
	whisker_servo.writeMicroseconds(DEFAULT_POSITION);
  

  input_pin = ANALOG_TRIGGER;
  lastT = micros();
	current_position = DEFAULT_POSITION;
  start_position = DEFAULT_POSITION;
  target_position_A_micro = start_position - OSCILLATION_AMPLITUDE;
  target_position_B_micro = start_position + OSCILLATION_AMPLITUDE;
  mode = 0;
  indicate_led = true;
  trigger_flag = false;
  start_flag = false;
  stop_flag = true;
  reset_flag = false;
  trigger_onset = 0;
}

void loop() {
  /****************************************************************************************************
  * Main loop, perpetually check for input and set mode state to 1-active or 0-inactive depending if in
  * between start and stop trigger or outside it and execute active/inactive code
  * ***************************************************************************************************
  */
  unsigned long currentT = micros();

  int trigger_input = read_input(input_pin);
  mode = mode_detect(trigger_input, &trigger_flag, &trigger_onset, &start_flag, &stop_flag, &reset_flag, currentT);
  
  switch (mode) {
    case 0: 
      // Serial.println("Inactive");
      execute_inactive_mode(currentT, &lastT);
      break;
    case 1: 
      // Serial.println("Active");
      execute_active_mode(&whisker_servo, currentT, &lastT, &current_position, &target_position_A_micro, &target_position_B_micro, indicate_led);
      break;
    case 2:
      Serial.println("reset");
      reset(&whisker_servo, &lastT, currentT, &current_position, &indicate_led,
            &start_position, &target_position_A_micro, &target_position_B_micro,
            &trigger_flag, &trigger_onset,
            &start_flag, &stop_flag, &reset_flag, &mode);
      break;
    case 3:
      Serial.println("pos_offset_fine");
      positive_offset(0, &whisker_servo, &start_position, &current_position, &target_position_A_micro, &target_position_B_micro);
      break;
    case 4:
      Serial.println("neg_offset_fine");
      negative_offset(0, &whisker_servo, &start_position, &current_position, &target_position_A_micro, &target_position_B_micro);
      break;
    case 5:
      Serial.println("pos_offset_coarse");
      positive_offset(1, &whisker_servo, &start_position, &current_position, &target_position_A_micro, &target_position_B_micro);
      break;
    case 6:
      Serial.println("neg_offset_coarse");
      negative_offset(1, &whisker_servo, &start_position, &current_position, &target_position_A_micro, &target_position_B_micro);
      break;
    
    default:
      break;
  }
}