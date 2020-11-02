/*
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

  * Download Filters library from https://drive.google.com/file/d/1Zb3yGRoztrP29oK-2G0f1e55asYaJVlV/view
 */

// include the library code:
#include <LiquidCrystal.h>
#include <Filters.h>                      //This library does a massive work check it's .cpp file

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#define ACS_Pin A0                        //Sensor data pin on A0 analog input

float ACS_Value;                              //Here we keep the raw data valuess
float testFrequency = 50;                    // test signal frequency (Hz)
float windowLength = 40.0/testFrequency;     // how long to average the signal, for statistist
float intercept = 0; // to be adjusted based on calibration testing
float slope = 0.00897; // to be adjusted based on calibration testing
float Amps_TRMS; // estimated actual current in amps

unsigned long printPeriod = 1000; // in milliseconds
// Track time in milliseconds since last reading 
unsigned long previousMillis = 0;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("AC RMS Voltage:");
  pinMode(ACS_Pin,INPUT);  //Define the pin mode
}

void loop() {
  RunningStatistics inputStats;                 // create statistics to look at the raw test signal
  inputStats.setWindowSecs( windowLength );     //Set the window length
   
  while( true ) {   
    ACS_Value = analogRead(ACS_Pin);  // read the analog in value:
    inputStats.input(ACS_Value);  // log to Stats function
        
    if((unsigned long)(millis() - previousMillis) >= printPeriod) { //every second we do the calculation
      previousMillis = millis();   // update time
      
      Amps_TRMS = intercept + slope * inputStats.sigma();

      // set the cursor to column 0, line 1
      // (note: line 1 is the second row, since counting begins with 0):
      lcd.setCursor(0, 1);
      lcd.print(Amps_TRMS);

    }
  }
}
