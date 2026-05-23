#define BLYNK_TEMPLATE_ID "TMPL3QsliiFVr"
#define BLYNK_TEMPLATE_NAME "accident alert system"
#define BLYNK_AUTH_TOKEN "SzB99ekBh6TCBC5QBbzlyYMg4I56N4lc"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

char auth[] ="SzB99ekBh6TCBC5QBbzlyYMg4I56N4lc";
char ssid[] = "Ajith";
char pass[] = "00000000";

SoftwareSerial arduinoSerial(D6, D5);   // RX, TX
SoftwareSerial gpsSerial(D2, D1);       // RX, TX



// === GPS ===
TinyGPS gps;
float flat, flon;

int lastAccident = 0;
int lastFire = 0;
int lastVibration = 0;

String data = "";

void setup() {
  Serial.begin(9600);
  arduinoSerial.begin(9600);
  gpsSerial.begin(9600);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  Serial.println("System Started");
}

void loop() {
  Blynk.run();

  // continuously read GPS characters
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);
  }

  // get GPS position
  float lat, lng;
  gps.f_get_position(&lat, &lng);

  

  // show GPS in serial monitor
  Serial.print("Latitude: ");
  Serial.println(lat, 6);
  Serial.print("Longitude: ");
  Serial.println(lng, 6);

  // read Arduino sensor data
  if (arduinoSerial.available()) {
    data = arduinoSerial.readStringUntil('\n');
    data.trim();

    Serial.print("Arduino Data: ");
    Serial.println(data);

    int p1 = data.indexOf(',');
    int p2 = data.indexOf(',', p1 + 1);

    if (p1 > 0 && p2 > 0) {
      int xVal = data.substring(0, p1).toInt();
      int fireVal = data.substring(p1 + 1, p2).toInt();
      int vibrationVal = data.substring(p2 + 1).toInt();

      String link = "https://www.google.com/maps?q=" + String(lat, 6) + "," + String(lng, 6);

      if ((xVal < 330 || xVal > 350) && lastAccident == 0) {
        Blynk.logEvent("alert", "Accident Detected " + link);
        Serial.println("Accident Detected");
        lastAccident = 1;
      }
      if (xVal >= 330 && xVal <= 350) {
        lastAccident = 0;
      }

      if (fireVal == 0 && lastFire == 0) {
        Blynk.logEvent("alert", "Fire Detected " + link);
        Serial.println("Fire Detected");
        lastFire = 1;
      }
      if (fireVal == 1) {
        lastFire = 0;
      }

      if (vibrationVal == 1 && lastVibration == 0) {
        Blynk.logEvent("alert", "Vibration Detected " + link);
        Serial.println("Vibration Detected");
        lastVibration = 1;
      }
      if (vibrationVal == 0) {
        lastVibration = 0;
      }
    }
  }

  delay(1500);
}