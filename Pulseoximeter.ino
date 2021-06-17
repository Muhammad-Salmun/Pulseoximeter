#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <Fonts/Org_01.h> 


#define ENABLE_MAX30100 1
#define SCREEN_ADDRESS 0x3c 

Adafruit_SSD1306 display(128, 32, &Wire, -1);


#if ENABLE_MAX30100
PulseOximeter pox;
#endif


bool tsLastReport = 0;
bool placed;


// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
  Serial.println("Beat!");
  if (placed == 0) 
  {
    display.clearDisplay();
    display.setFont(&FreeSerif9pt7b);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(7, 16);
    display.print("Calculating...");    /////////////////////////////////////////////////////printing Calculating
    display.display();
    placed = 1;
  }
}

void setup()  ///////////////////////////////////////////////////////////////////////////void setup
{
  Serial.begin(115200);
  Serial.println("SSD1306 128x32 OLED TEST");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setFont(&FreeSerif9pt7b);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(6, 16);
  display.print("PulseOxiMeter");    /////////////////////////////////////////////////////printing pulseoximeter
  display.display();
  delay(5000); // Pause for 2 seconds
  display.cp437(true);
  display.clearDisplay();
  Serial.print("Initializing pulse oximeter..");
#if ENABLE_MAX30100
  // Initialize the PulseOximeter instance
  // Failures are generally due to an improper I2C wiring, missing power supply
  // or wrong target chip
  if (!pox.begin()) 
  {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);

#endif
}


void loop()       /////////////////////////////////////////////////////////////////void loop
{
#if ENABLE_MAX30100
  // Make sure to call update as fast as possible
  pox.update();
  byte bpm = 0;
  byte spo2 = 0;
  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
  if (millis() - tsLastReport > 5)         //printing can be delayed but not pox.update()
 {
    bpm = pox.getHeartRate();
    spo2 = pox.getSpO2();
    Serial.println(bpm);    
    Serial.println(spo2);
  
    if (bpm>30 and bpm<220 and spo2>30 and spo2<100)  
    {
        display.clearDisplay();
        display.setFont();
        display.setTextSize(1);      // Normal 1:1 pixel scale
        
        display.setCursor(13, 3); 
        display.print("%SpO");          //////////////////////////////////////////////SpO2%
        display.setFont(&Org_01);
        display.print("2"); 
    
        display.setFont(&FreeSerif12pt7b);
        display.setCursor(20, 30);
        display.print(spo2);          ////////////////////////////////////////////////SpO2_value
    
        
        display.setFont();
        display.setCursor(70, 3); 
        display.print("PRbpm");         //////////////////////////////////////////////PRbpm
             
      
        display.setFont(&FreeSerif12pt7b);
        display.setCursor(77, 30);
        display.print(bpm);          ////////////////////////////////////////////////BPM_value
        
        display.display();         //print in display 
    }
    
    if (bpm == 0 and spo2 == 0) 
    {
        placed = 0;
        display.clearDisplay();
        display.setFont(&FreeSerif9pt7b);
        display.setCursor(25, 13);       /////////////////////////////////////////////////////asking to place finger
        display.print("Place Your");
        display.setCursor(40,26);
        display.print("Finger");    
        display.display();
    }
    
    tsLastReport = millis();
    Serial.println(tsLastReport);
    
  }
#endif
}
