#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = "_FsBoJv0mrZY1-12Sdr9RKua7nS6u-_u";
char ssid[] = "skynet";
char pass[] = "amuitato";
char server[] = "192.168.1.111";
int port = 8088;

#define DHTPIN 0          // D3
#define DHTTYPE DHT11   // DHT 11, AM2302, AM2321

//Setup connection of the sensor
Adafruit_BMP280 bmp; // I2C

DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float a = bmp.readTemperature();
  float b = bmp.readPressure()/100;
  float c = bmp.readAltitude(1015.80);

  String umiditate =  String(h);
  String temperatura_dht =  String(t);
  String temperatura_bmp =  String(a);
  String presiune =  String(b);
  String altitudine =  String(c);

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);  //V5 is for Humidity on DHT11
  Blynk.virtualWrite(V6, t);  //V6 is for Temperature on DHT11
  Blynk.virtualWrite(V7, a);  //V7 is for Temperature on BMP280
  Blynk.virtualWrite(V8, b);  //V8 is for Pressure on BMP280
  Blynk.virtualWrite(V9, c);  //V9 is for Altitude on BMP280

  lcd.print("Temp: " + temperatura_dht + "°C DHT11");
  lcd.print("Humidity: " + umiditate + "%");
  
  lcd.print("Temp: " + temperatura_bmp + "°C BMP280");
  lcd.print("Pres: " + presiune + "milibar");
  lcd.print("Alt: " + altitudine + "m");
}

void setup()
{
  Serial.begin(9600); // See the connection status in Serial Monitor

  Wire.begin(2,0);
  
  lcd.begin();   // initializing the LCD 

  Blynk.begin(auth, ssid, pass, server, port);  // Connect Blynk

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
}
