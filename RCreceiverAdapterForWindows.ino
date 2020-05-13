#include <XInput.h>
#include <EEPROM.h>

// Channels input PINs from radio-receiver
const uint8_t Pin_CH1 = 11;
const uint8_t Pin_CH2 = 10;
const uint8_t Pin_CH3 = 9;
const uint8_t Pin_CH4 = 8;

// Input values from radio-reveiver
uint16_t CH1 = 0;
uint16_t CH2 = 0;
uint16_t CH3 = 0;
uint16_t CH4 = 0;

// Min and Max possibles input values for XInput libraby joysticks functions.
const int32_t JoyMin = -32768;
const int32_t JoyMax = 32767;

// Mapped Values from radio-receiver to XInput library functions
int32_t JoyLeftX = 0;
int32_t JoyLeftY = 0;
int32_t JoyRightX = 0;
int32_t JoyRightY = 0;

// Structure to save in EEPROM in one shot the Ranges of values of input channels from radio-receiver
struct Input_Channels_Min_Max_Values {
  // Min input values from radio-reveiver
  uint16_t CH1_MIN;
  uint16_t CH2_MIN;
  uint16_t CH3_MIN;
  uint16_t CH4_MIN;

  // Max input values from radio-reveiver
  uint16_t CH1_MAX;
  uint16_t CH2_MAX;
  uint16_t CH3_MAX;
  uint16_t CH4_MAX;
};

// Create and initialise with zeros an Input_Channels_Min_Max_Values object that is named Ranges
Input_Channels_Min_Max_Values Ranges{
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

// Address of EEPROM for reading and writing saved ranges values
int eeAddr = 0;

void setup() {

  pinMode(A0, INPUT_PULLUP); // Input PIN for calibrating button
  pinMode(13, OUTPUT); // Output PIN for calibration indicator LED

  if (!digitalRead(A0)) { // If the button to enter on calibration mode on the A0 PIN is grounded, go into calibration mode.

    while (!digitalRead(A0)) { // While the button to enter on calibration mode on the A0 PIN is grounded.

      // Get values from receiver
      CH1 = pulseIn(Pin_CH1, HIGH);
      CH2 = pulseIn(Pin_CH2, HIGH);
      CH3 = pulseIn(Pin_CH3, HIGH);
      CH4 = pulseIn(Pin_CH4, HIGH);

      // If received values from radio-receiver into channel's PINs are 0, it's probably because radio-emitter is off, so on board LED is blinking to indicate it.
      if (!CH1 || !CH2 || !CH3 || !CH4) {
        digitalWrite(13, HIGH);
        delay(1000);
        digitalWrite(13, LOW); // no low state delay needed for the LED because pulseIn fonction when waits to PWN signal have timeout that acts as a delay.
      }
      // Otherwise if channel's PINs receives values from radio-receiver, lets go on calibration mode.
      else {

        digitalWrite(13, HIGH);// solid LED indicate calibrating mode is on

        // Initialise MIN and MAX values of each channels with the current channel's values
        if (!Ranges.CH1_MIN || !Ranges.CH1_MAX || !Ranges.CH2_MIN || !Ranges.CH2_MAX || !Ranges.CH3_MIN || !Ranges.CH3_MAX || !Ranges.CH4_MIN || !Ranges.CH4_MAX) {
          Ranges.CH1_MIN = CH1;
          Ranges.CH1_MAX = CH1;
          Ranges.CH2_MIN = CH2;
          Ranges.CH2_MAX = CH2;
          Ranges.CH3_MIN = CH3;
          Ranges.CH3_MAX = CH3;
          Ranges.CH4_MIN = CH4;
          Ranges.CH4_MAX = CH4;
        }
        /*
          After MIN and MAX values of each channels were initialised, no need to do it again.
          That's why inititailisation only occurs when MIN and MAX values are zero (as defined globally).
          Now it's time to move both joysticks on radio-emitter to get the full motion ranges of both axes for both joysticks.
        */
        // For each channel if current channel value is higer or lower than MIN and MAX stored values, the MIN or MAX value is updated.
        if (CH1 < Ranges.CH1_MIN)
          Ranges.CH1_MIN = CH1;
        else if (CH1 > Ranges.CH1_MAX)
          Ranges.CH1_MAX = CH1;

        if (CH2 < Ranges.CH2_MIN)
          Ranges.CH2_MIN = CH2;
        else if (CH2 > Ranges.CH2_MAX)
          Ranges.CH2_MAX = CH2;

        if (CH3 < Ranges.CH3_MIN)
          Ranges.CH3_MIN = CH3;
        else if (CH3 > Ranges.CH3_MAX)
          Ranges.CH3_MAX = CH3;

        if (CH4 < Ranges.CH4_MIN)
          Ranges.CH4_MIN = CH4;
        else if (CH4 > Ranges.CH4_MAX)
          Ranges.CH4_MAX = CH4;

      }
    }
    // Saving Ranges struct that embed MIN and MAX values on EEPROM memory.
    EEPROM.put(eeAddr, Ranges);
    // LED OFF to indicate calibration is OFF
    digitalWrite(13, LOW);
  }

  // If calibration mode is disabled, lets read EPPROM to get stored MIN and MAX values into Ranges object for using them into the sketch.
  else {
    EEPROM.get(eeAddr, Ranges);
  }
  
  // Start using XInput
  XInput.begin(); 
}

void loop() {
  
  // Get values from receiver
  CH1 = pulseIn(Pin_CH1, HIGH);
  CH2 = pulseIn(Pin_CH2, HIGH);
  CH3 = pulseIn(Pin_CH3, HIGH);
  CH4 = pulseIn(Pin_CH4, HIGH);

  // If received values from radio-receiver into channel's PINs are 0, it's probably because radio-emitter is off, so on board LED is blinking to indicate it.
  if (!CH1 || !CH2 || !CH3 || !CH4) {
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW); // No low state delay needed for the LED because pulseIn fonction when waits to PWN signal have timeout that acts as a delay.
  }
  // Otherwise if channel's PINs receives values from radio-receiver, lets go mapping them to prepare them for been used by XInput library functions.
  else {

    // Input values from radio-receiver are mapped
    JoyLeftX = (CH4 - Ranges.CH4_MIN) * (JoyMax - JoyMin) / (Ranges.CH4_MAX - Ranges.CH4_MIN) + JoyMin;
    JoyLeftY = (CH3 - Ranges.CH3_MIN) * (JoyMax - JoyMin) / (Ranges.CH3_MAX - Ranges.CH3_MIN) + JoyMin;
    JoyRightX = (CH1 - Ranges.CH1_MIN) * (JoyMax - JoyMin) / (Ranges.CH1_MAX - Ranges.CH1_MIN) + JoyMin;
    JoyRightY = (CH2 - Ranges.CH2_MIN) * (JoyMax - JoyMin) / (Ranges.CH2_MAX - Ranges.CH2_MIN) + JoyMin;

    // Mapped values are send to usb by using XInput libraby joysticks functions
    XInput.setJoystickX(JOY_LEFT, JoyLeftX);
    XInput.setJoystickY(JOY_LEFT, JoyLeftY);
    XInput.setJoystickX(JOY_RIGHT, JoyRightX);
    XInput.setJoystickY(JOY_RIGHT, JoyRightY);
    
  }
}
