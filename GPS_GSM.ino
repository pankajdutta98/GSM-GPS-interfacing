#include <SoftwareSerial.h>
#include <TinyGPS.h>
TinyGPS gps;
SoftwareSerial mySerial(9, 11);          //for GSM Module
SoftwareSerial ss(3, 5);                 //for GPS Module
float flat, flon;

void setup()
{
  Serial.begin(9600); 
  mySerial.begin(9600);   //baud rate of GSM Module  
  ss.begin(9600);
  Serial.print("GPS COORDINATES");
  delay(100);
}


void loop()
{
   bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (Serial.available()>0)
   switch(Serial.read())
  {
    case 's':
    if (newData)
  {
    //float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LATITUDE=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" \tLONGITUDE=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
  Serial.print(" \n");
  //delay(4000);
}    
      SendMessage();
      break;
    case 'r':
      RecieveMessage();
      break;
  }

 if (mySerial.available()>0)
   Serial.write(mySerial.read());
}


 void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+919672030993\"\r"); // Replace x with mobile number
  delay(1000);
 mySerial.println("LATITUDE=");
  delay(100);
  mySerial.println(flat);// The SMS text you want to send
  delay(100);
  mySerial.println("LONGITUDE=");
  delay(100);  
  mySerial.println(flon);
  delay(100);
    mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}


 void RecieveMessage()
{
  mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(1000);
   if (mySerial.available()>0)
   Serial.write(mySerial.read());
 }

