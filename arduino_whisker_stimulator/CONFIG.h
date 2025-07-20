const int SERVOPORT = 53;       //digital pin 10
const int LED_INDICATOR = 13;   //digital pin 13
const int DIGITAL_TRIGGER = 51; //digital PIN 12
const int RESET_PIN = 52;       //digital PIN 11
const int ANALOG_TRIGGER = A0;  //analog PIN A0    //analog can only recognize 0.87ms(from NIDAQ) and above pulse width trigger with min at 800, max at 1200

const int DEFAULT_POSITION = 850;        //microseconds
const int OSCILLATION_AMPLITUDE = 50;    //microseconds

const int OFFSET_FINE = 5;             //microseconds
const int OFFSET_COARSE = 50;          //microseconds

const long ACTIVE_INTERVAL = 50000;     //microseconds
const long INACTIVE_INTERVAL = 1000000; //microseconds

const unsigned long TRIGGER_WIDTH_MARGIN = 200; //microseconds

const unsigned long START_STOP_TRIGGER_WIDTH = 1000;          //microseconds 0.001s  //500microseconds was the lowest possible width for trigger detect
const unsigned long RESET_TRIGGER_WIDTH = 2000;               //microseconds 0.002s
const unsigned long POS_OFFSET_FINE_TRIGGER_WIDTH = 3000;     //microseconds 0.003s
const unsigned long NEG_OFFSET_FINE_TRIGGER_WIDTH = 4000;     //microseconds 0.004s
const unsigned long POS_OFFSET_COARSE_TRIGGER_WIDTH = 5000;   //microseconds 0.005s
const unsigned long NEG_OFFSET_COARSE_TRIGGER_WIDTH = 6000;   //microseconds 0.006s


const unsigned long MIN_TRIGGER_WIDTH = START_STOP_TRIGGER_WIDTH - TRIGGER_WIDTH_MARGIN; 
const unsigned long MAX_TRIGGER_WIDTH = START_STOP_TRIGGER_WIDTH + TRIGGER_WIDTH_MARGIN;
const unsigned long RESET_MIN_TRIGGER_WIDTH = RESET_TRIGGER_WIDTH - TRIGGER_WIDTH_MARGIN;
const unsigned long RESET_MAX_TRIGGER_WIDTH = RESET_TRIGGER_WIDTH + TRIGGER_WIDTH_MARGIN;
const unsigned long POS_MIN_FINE_TRIGGER_WIDTH = POS_OFFSET_FINE_TRIGGER_WIDTH - TRIGGER_WIDTH_MARGIN;
const unsigned long POS_MAX_FINE_TRIGGER_WIDTH = POS_OFFSET_FINE_TRIGGER_WIDTH + TRIGGER_WIDTH_MARGIN;
const unsigned long NEG_MIN_FINE_TRIGGER_WIDTH = NEG_OFFSET_FINE_TRIGGER_WIDTH - TRIGGER_WIDTH_MARGIN;
const unsigned long NEG_MAX_FINE_TRIGGER_WIDTH = NEG_OFFSET_FINE_TRIGGER_WIDTH + TRIGGER_WIDTH_MARGIN; 
const unsigned long POS_MIN_COARSE_TRIGGER_WIDTH = POS_OFFSET_COARSE_TRIGGER_WIDTH - TRIGGER_WIDTH_MARGIN;
const unsigned long POS_MAX_COARSE_TRIGGER_WIDTH = POS_OFFSET_COARSE_TRIGGER_WIDTH + TRIGGER_WIDTH_MARGIN;
const unsigned long NEG_MIN_COARSE_TRIGGER_WIDTH = NEG_OFFSET_COARSE_TRIGGER_WIDTH - TRIGGER_WIDTH_MARGIN;
const unsigned long NEG_MAX_COARSE_TRIGGER_WIDTH = NEG_OFFSET_COARSE_TRIGGER_WIDTH + TRIGGER_WIDTH_MARGIN; 