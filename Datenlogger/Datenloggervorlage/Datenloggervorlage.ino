#include <SPI.h>
#include <SD.h>
#include "DHT.h"
# include <RTClib.h>

#define RTCPIN 10

#define DHTPIN 8    
#define DHTTYPE DHT11   
DHT sensor(DHTPIN, DHTTYPE);

RTC_DS1307 rtc;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(RTCPIN)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  if (! rtc.begin()) {
    Serial.println("No RTC found");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is not configured");
  }

  sensor.begin();
  // Set Date and Time (Year, DAY, Month, Hour(24h), Minute, Second)
  //rtc.adjust(DateTime(2024, 9, 06, 19, 47, 40)); //Uncomment this line to set Date and Time
}

void loop() {
  delay(1000);
  DateTime aktuell = rtc.now();
  // make a string for assembling the data to log:
  String dataString = "";

  float h = sensor.readHumidity(); //read values
  float t = sensor.readTemperature();
  
  char Zeit[] = "hh:mm:ss";
  String timeStr = aktuell.toString(Zeit);
  
  if (isnan(h) || isnan(t)) { // Check if any reads failed and exit early (to try again).
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  dataString = "Time:" + timeStr + ", Temperature DHT:" + String(t)+ ", Humidity DHT:" + String(h); //compact the data
  
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}
