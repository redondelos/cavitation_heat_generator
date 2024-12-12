#include "max6675.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pushbuttons
const int buttonNext = 8;
const int buttonSelect = 9;

// Thermocouple pins
int thermoDO_1 = 10;
int thermoCS_1 = 3;
int thermoCLK_1 = 4;

int thermoDO_2 = 5;
int thermoCS_2 = 6;
int thermoCLK_2 = 7;

MAX6675 thermocouple1(thermoCLK_1, thermoCS_1, thermoDO_1);
MAX6675 thermocouple2(thermoCLK_2, thermoCS_2, thermoDO_2);

// Flowmeter sensor pin
const int flowPin = 2; // Pin connected to the FS300A flowmeter

// Flowmeter variables
volatile int pulseCount = 0; // Counts the number of pulses
float flowRate = 0.0;        // Flow rate in liters per minute (L/min)
float totalVolume = 0.0;     // Total volume in liters
unsigned long oldTime = 0;   // Time tracking for flow rate calculation

// Calibration factor for FS300A flowmeter
const float calibrationFactor = 5.5;

// Menu states
enum MenuState { THERMOCOUPLE_READINGS, OTHER_SENSOR_READINGS, FLOWMETER_READINGS };
MenuState currentMenu = THERMOCOUPLE_READINGS;

// Temperature unit state
enum TempUnit { CELSIUS, FAHRENHEIT };
TempUnit currentUnit = CELSIUS;

void setup() {
  Serial.begin(9600);
  delay(500);

  lcd.init();
  lcd.clear();
  lcd.backlight();

  // Initial display message
  lcd.setCursor(0, 0);
  lcd.print("Cavitation Heat");
  lcd.setCursor(2, 1);
  lcd.print("Generator");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Capstone Project");
  lcd.setCursor(0, 1);
  lcd.print("Mechanical Eng.");
  delay(2000);

  lcd.clear();

  // Set up button pins
  pinMode(buttonNext, INPUT_PULLUP);
  pinMode(buttonSelect, INPUT_PULLUP);

  // Set up flowmeter pin
  pinMode(flowPin, INPUT_PULLUP);

  // Attach interrupt to the flowmeter pin
  attachInterrupt(digitalPinToInterrupt(flowPin), pulseCounter, FALLING);
}

void loop() {
  // Calculate flow rate and volume every second
  unsigned long currentTime = millis();
  if (currentTime - oldTime > 1000) { // 1-second interval
    // Disable interrupts while calculating
    detachInterrupt(digitalPinToInterrupt(flowPin));

    // Calculate the flow rate in L/min
    flowRate = (pulseCount / calibrationFactor);

    // Calculate the total volume in liters
    totalVolume += (flowRate / 60.0); // Convert L/min to liters per second

    // Reset pulse count and re-enable interrupts
    pulseCount = 0;
    attachInterrupt(digitalPinToInterrupt(flowPin), pulseCounter, FALLING);

    // Update the time
    oldTime = currentTime;
  }

  // Print thermocouple and flowmeter data to Serial Monitor
  Serial.print(thermocouple1.readCelsius());
  Serial.print(",");
  Serial.print(thermocouple2.readCelsius());
  Serial.print(",");
  Serial.print(flowRate);
  Serial.print(",");
  Serial.println(totalVolume);

  // Check button presses for menu navigation
  if (digitalRead(buttonNext) == LOW) {
    currentMenu = static_cast<MenuState>((currentMenu + 1) % 3); // Cycle through 3 menu states
    delay(200); // Debounce delay
  }

  // Check button presses for unit toggle
  if (digitalRead(buttonSelect) == LOW) {
    currentUnit = static_cast<TempUnit>((currentUnit + 1) % 2);
    delay(200); // Debounce delay
  }

  // Display based on current menu
  switch (currentMenu) {
    case THERMOCOUPLE_READINGS:
      displayThermocoupleReadings();
      break;
    case OTHER_SENSOR_READINGS:
      displayOtherSensorReadings();
      break;
    case FLOWMETER_READINGS:
      displayFlowmeterReadings();
      break;
  }

  delay(1000); // Delay for readability and sensor update
}

void displayThermocoupleReadings() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T in  = ");
  lcd.print(readTemperature(thermocouple1));
  lcd.print(currentUnit == CELSIUS ? " C" : " F");

  lcd.setCursor(0, 1);
  lcd.print("T out = ");
  lcd.print(readTemperature(thermocouple2));
  lcd.print(currentUnit == CELSIUS ? " C" : " F");
}

void displayOtherSensorReadings() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Other Sensor:");
  lcd.setCursor(0, 1);
  lcd.print("Value = ");
  lcd.print(readOtherSensor());
}

void displayFlowmeterReadings() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Flow Rate: ");
  lcd.print(flowRate);
  lcd.print(" L/min");

  lcd.setCursor(0, 1);
  lcd.print("Volume: ");
  lcd.print(totalVolume);
  lcd.print(" L");
}

float readTemperature(MAX6675 &thermocouple) {
  return currentUnit == CELSIUS ? thermocouple.readCelsius() : thermocouple.readFahrenheit();
}

float readOtherSensor() {
  // Placeholder for reading another sensor
  // Replace with actual sensor reading code
  return 0.0; // Example value
}

// Interrupt service routine (ISR) to count pulses
void pulseCounter() {
  pulseCount++;
}