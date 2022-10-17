//Arduino-based  Tracking System using SIM900A GSM Module and NEO-6M GPS Module
//Tracking system is triggered by SMS, call, and button press to send message...
//By: Paul Bryan

#include <TinyGPS++.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

int state = 0;
const int button = 8;
boolean statusGPS = false;

String gpsLatitude, gpsLongitude;
String gpsDate, gpsTime;

#define gsmRX 2
#define gsmTX 3

TinyGPSPlus gps;
SoftwareSerial SIM900A (gsmRX, gsmTX);

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  SIM900A.begin (9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() > 0)
    if (gps.encode(Serial.read()))
      getDataGPS();
  
  if (digitalRead(button) == HIGH && state == 0){
    sendSMS();
  }
  if (digitalRead(button) == HIGH){
    state = 0;
  }
  delay(100);

}

void getDataGPS(){
  if (gps.location.isValid()){
    gpsLatitude = gps.location.lat(), 6;  //Latitude loaction in degrees
    gpsLongitude = gps.location.lng(), 6; //Longitude loaction in degrees
    statusGPS = true;
  }
  else
    statusGPS = false;
  
  if (gps.date.isValid()){
    gpsDate = gps.date.value(); //Date value in DDMMYY
    statusGPS = true;
  }
  else
    statusGPS = false;
  if (gps.time.isValid()){
    gpsTime = gps.time.value(); //Time value in HHMMSSCC
    statusGPS = true;
  }
  else
    statusGPS = false;
}

void sendSMS (){
  SIM900A.print("AT+CMGF=1\r"); //Set GSM module to sending mode
  delay(1000);
  SIM900A.print("AT+CMGS=\"+63XXXXXXXXXX\"\r"); //Change 10-Xs by the recipient's 10-digit number after 0
  delay(1000);
  //The SMS format to be sent.
  SIM900A.print("Update: Device current location is " + gpsLatitude + ", " + gpsLongitude + ".");
  SIM900A.println("Recorded at " + gpsTime + ", " + gpsDate + ".");
  SIM900A.println("Click the link to see in the map. www.google.com/maps/?q=" + gpsLatitude + "," + gpsLongitude);
  delay(1000);
  SIM900A.write(0x1A);
  SIM900A.println((char)26);
  delay(1000);
}