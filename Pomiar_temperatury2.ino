#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

/*
 * 0x28, 0x81, 0x6, 0x11, 0x0, 0x0, 0x0, 0xE8 (red, white orange)
 * 0x28, 0xF9, 0x4A, 0x11, 0x0, 0x0, 0x0, 0x19(blue, green, yellow)
 */

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
Adafruit_NeoPixel leds = Adafruit_NeoPixel(8, A0, NEO_GRB + NEO_KHZ800);
OneWire oneWire(A4);
DallasTemperature sensors(&oneWire);
DHT dht11;

DeviceAddress LaptTerm = {0x28, 0xF9, 0x4A, 0x11, 0x0, 0x0, 0x0, 0x19};
DeviceAddress ArduTerm = {0x28, 0x81, 0x6, 0x11, 0x0, 0x0, 0x0, 0xE8};

void setup() {
  lcd.begin(16, 2);
  leds.begin();
  sensors.begin();
  dht11.setup(A3);

  lcd.setCursor(0, 0);
  lcd.print("Booting...");
  for(int i = 0; i<8; i++) {
    leds.setPixelColor(i, leds.Color(random(0, 40), random(0, 40), random(0, 40)));
    leds.show();
    delay(400);
  }
}

void loop() {
  int humidity = dht11.getHumidity();
  int tempRoom = dht11.getTemperature();
  
  if (dht11.getStatusString() == "OK") {
    lcd.setCursor(0, 0);
    lcd.print("Temp - Room:");
    lcd.setCursor(0, 1);
    lcd.print(tempRoom + 1);
    lcd.print(" C deg.     ");
    TempLeds(tempRoom + 1);
    delay(5000);

    lcd.setCursor(0 ,0);
    lcd.print("Humidity:   ");
    lcd.setCursor(0, 1);
    lcd.print(humidity);
    lcd.print(" %RH     ");
    HumidityLeds(humidity);
    delay(5000);
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Read Error");
    delay(2000);
  }
  
  sensors.requestTemperatures();
  lcd.setCursor(0, 0);
  lcd.print("Temp - Lapt:");
  lcd.setCursor(0, 1);
  lcd.print(sensors.getTempC(LaptTerm), 1);
  lcd.print(" C deg.     ");
  TempLeds(sensors.getTempC(LaptTerm));
  delay(5000);

  lcd.setCursor(0, 0);
  lcd.print("Temp - Ardu:");
  lcd.setCursor(0, 1);
  lcd.print(sensors.getTempC(ArduTerm), 1);
  lcd.print(" C deg.     ");
  TempLeds(sensors.getTempC(ArduTerm));
  delay(5000);
}

void TempLeds(int temperature) {
  //0 - 100
  leds.clear();
  int temp = 0;
  if (temperature <= 16 && temperature > 0) {
    temp = map(temperature, 0, 16, 1, 8);
    for (int i = 0; i < temp; i++) {
      leds.setPixelColor(i, leds.Color(0, 0, 40));
    }
  }
  else if (temperature > 16 && temperature <= 30) {
    temp = map(temperature, 16, 30, 1, 8);
    for (int i = 0; i < temp; i++) {
      leds.setPixelColor(i, leds.Color(0, 40, 0));
    }
  }
  else if (temperature > 30 && temperature <= 46) {
    temp = map(temperature, 30, 46, 1, 8);
    for (int i = 0; i < temp; i++) {
      leds.setPixelColor(i, leds.Color(40, 0, 0));
    }
  }
  else if (temperature > 46 && temperature <= 58) {
    temp = map(temperature, 46, 58, 1, 8);
    for (int i = 0; i < temp; i++) {
      leds.setPixelColor(i, leds.Color(40, 0, 40));
    }
  }
  else {
    leds.rainbow(0, 1, 255, 50);
  }
  leds.show();
}
void HumidityLeds(int humidity) {
  leds.clear();
  int humid = 0;
  if (humidity < 20 && humidity > 100) {
    leds.rainbow(0, 1, 255, 50);
  }
  else {
    humid = map(humidity, 20, 95, 1, 7);
    for(int i = 0; i < humid; i++) {
      leds.setPixelColor(i, leds.Color(0, 40, 40));
    }
    leds.setPixelColor(7, leds.Color(0, 0, 50));
  }
  leds.show();
}
