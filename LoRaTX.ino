#include <Adafruit_GPS.h>
#include <Adafruit_PMTK.h>
#include <NMEA_data.h>
#include <LoRa.h>


const long frequency = 915E6;

const int cs = A2;
const int rs = A3;
const int it = A1;

#define GPSSerial Serial1
Adafruit_GPS GPS(&Serial1);

#define GPSECHO false

uint32_t timer = millis();

void setup() {
  Serial.begin(9600);
  Serial.println("Comienzo de conexion");

  LoRa.setPins(cs, rs, it);

  if(!LoRa.begin(915E6)){
    Serial.println("Comunicacion fallida por error de conexion, revisar cableado");
    while(true);
  }

  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);

  delay(1000);

  GPSSerial.print(PMTK_Q_RELEASE);
}

void loop() {
  LoRa.beginPacket();
  char c = GPS.read();
    if (c) Serial.print(c);
  if (GPS.newNMEAreceived()) {

    Serial.print(GPS.lastNMEA()); 
    if (!GPS.parse(GPS.lastNMEA())) 
      return; 
  }

  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    Serial.print("\nTime: ");
    if(GPS.hour < 10) {Serial.print('0');}
    Serial.print(GPS.hour, DEC); Serial.print(':'); LoRa.print(GPS.hour, DEC);
    if(GPS.minute < 10){Serial.print('0');}
    Serial.print(GPS.minute, DEC); Serial.print(':');  LoRa.print(GPS.minute, DEC);
    if(GPS.seconds <10); {Serial.print('0');}
    Serial.print(GPS.seconds, DEC); Serial.print(':'); LoRa.print(GPS.seconds, DEC);
    if (GPS.milliseconds < 10); {Serial.print("00");} LoRa.print(GPS.milliseconds, DEC);

    Serial.print("\nFecha: ");
    Serial.print(GPS.day, DEC); Serial.print('/'); LoRa.print(GPS.day, DEC);
    Serial.print(GPS.month, DEC); Serial.print("/20"); LoRa.print(GPS.month, DEC);
    Serial.println(GPS.year, DEC);  LoRa.print(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix); LoRa.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality); LoRa.print((int)GPS.fixquality);

    if(GPS.fix){
      Serial.print("Ubicacion: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat); LoRa.print("Lat: "); LoRa.print(GPS.latitude, 4);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon); LoRa.print("Lng: "); LoRa.print(GPS.longitude, 4);    
      Serial.print("Altitude: "); Serial.println(GPS.altitude); LoRa.print("Altitud: "); LoRa.print(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites); LoRa.print("Satelites: "); LoRa.print(GPS.satellites);
    }

  }
  LoRa.endPacket();
  delay(10000);
}




