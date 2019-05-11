// připojení potřebné knihovny
//teplotni cidla
#include <OneWire.h>
#include <DallasTemperature.h>
//pocetni operace
#include <math.h>
//lcd display
#include <LiquidCrystal_I2C.h>

//PINY
//teplotni cidla
const int pinCidlaDS = 3;
//reke
int rele = 7;

//INIT
// vytvoření instance oneWireDS z knihovny OneWire
OneWire oneWireDS(pinCidlaDS);
// vytvoření instance senzoryDS z knihovny DallasTemperature
DallasTemperature senzoryDS(&oneWireDS);
//LCD display
// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,16,2);

//KONSTANTY
//max min teplota
const int maxTemp = 8;
const int minTemp = -1;


void setup() {
  // zapnutí komunikace knihovny s teplotním čidlem
  senzoryDS.begin();  
  // initialize the lcd 
  // inicializace LCD
  lcd.begin();                     
  // Print a message to the LCD.
  lcd.backlight();
  //rele
  pinMode(rele, OUTPUT);
  digitalWrite(rele, HIGH);
}

void loop() {
  // načtení informací ze všech připojených čidel na daném pinu
  senzoryDS.requestTemperatures();
  float sensorTop = senzoryDS.getTempCByIndex(0);
  float sensorBottom = senzoryDS.getTempCByIndex(1);

  //kontrola teploty
  int releVal = digitalRead(rele);
  if(sensorBottom >= maxTemp && releVal == HIGH) {
    //teplota je vysoka, je nutno zapnout chlazeni
    digitalWrite(rele, LOW);
  } else if((sensorBottom <= minTemp || sensorTop <= minTemp) && releVal == LOW) {
    //teplota je nizka, je nutno vypnout chlazeni
    digitalWrite(rele, HIGH);
  }
  
  //zobrazeni temploty z cidla nahore
  lcd.clear();  
  lcd.setCursor(0,0);
  lcd.print("Nahore: ");
  lcd.print(sensorTop);
  lcd.print(" C");  
  //zobrazeni temploty z cidla dole  
  lcd.setCursor(0,1);
  lcd.print("Dole: ");
  lcd.print(sensorBottom);
  lcd.print(" C");
  
  // pauza
  delay(30000);
}
