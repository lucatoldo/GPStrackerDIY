#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <FS.h>
#include <LittleFS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>

TinyGPSPlus gps;
File myfile;  
SoftwareSerial ss(4, 5); // The serial connection to the GPS device

const char* ssid = "YOURSSID";
const char* password = "YOURPWD";

float latitude , longitude;
int year , month , date, hour , minute , second;
String date_str , time_str , lat_str , lng_str;
int pm;

AsyncWebServer server(80);
void setup()
{
  Serial.begin(115200);
  ss.begin(9600);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  SPI.begin();
 InitalizeFileSystem();
 initializeFile();

 server.serveStatic("/", SPIFFS, "/");

  server.begin();

}

void initializeFile() {
    myfile = SPIFFS.open("/track.gpx", "w");
    myfile.println("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>");
    myfile.println("<gpx xmlns=\"http://www.topografix.com/GPX/1/1\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.garmin.com/xmlschemas/GpxExtensions/v3 http://www.garmin.com/xmlschemas/GpxExtensions/v3/GpxExtensionsv3.xsd http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd\">");
    myfile.println("<trk><trkseg>");
    myfile.println("</trk></trkseg>");
    myfile.close();
}


boolean InitalizeFileSystem()
{
  bool initok = false;
  initok = SPIFFS.begin();
  if (!(initok))
  {
    SPIFFS.format();
    initok = SPIFFS.begin();
  }
  if (!(initok))
  {
    SPIFFS.format();
    initok = SPIFFS.begin();
  }
   return initok;
}

void loop()
{
  while (ss.available() > 0)
    if (gps.encode(ss.read()))  {
      if (gps.location.isValid()) {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
        lat_str.replace(",",".");
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
        lng_str.replace(",",".");
      }

      if (gps.date.isValid())  {
        date_str = "";
        date = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();

        if (date < 10)
          date_str = '0';
        date_str += String(date);

        date_str += " / ";

        if (month < 10)
          date_str += '0';
        date_str += String(month);

        date_str += " / ";

        if (year < 10)
          date_str += '0';
        date_str += String(year);
      }

      if (gps.time.isValid()) {
        time_str = "";
        hour = gps.time.hour();
        minute = gps.time.minute();
        second = gps.time.second();

        minute = (minute + 30);
        if (minute > 59)
        {
          minute = minute - 60;
          hour = hour + 1;
        }
        hour = (hour + 5) ;
        if (hour > 23)
          hour = hour - 24;

        if (hour >= 12)
          pm = 1;
        else
          pm = 0;

        hour = hour % 12;

        if (hour < 10)
          time_str = '0';
        time_str += String(hour);

        time_str += " : ";

        if (minute < 10)
          time_str += '0';
        time_str += String(minute);

        time_str += " : ";

        if (second < 10)
          time_str += '0';
        time_str += String(second);

        if (pm == 1)
          time_str += " PM ";
        else
          time_str += " AM ";
         delay(10000);
         yield();
         if (lat_str.length()>0) {
           myfile = SPIFFS.open("/track.gpx", "r+");
           myfile.seek(17,SeekEnd);
           myfile.println("<trkpt lat=\""+lat_str+"\" lon=\""+lng_str+"\"></trkpt>\n");
           myfile.println("</trk></trkseg>");
           myfile.close();
         }
       }
    }
}
