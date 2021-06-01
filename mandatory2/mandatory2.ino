#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#define DHTPIN 2
#define DHTTYPE DHT11

LiquidCrystal_I2C lcd(0x3F, 20, 4);

DHT dht(DHTPIN, DHTTYPE);

// Pin setup
//temperatur og fugtighedssensor pin 2
//lyssensor pin A0
//elektrisk lys LED 10
//lys LED 11
//temperatur LED 12
//fugtigheds LED 13
//lcd A5 og A4, bruger en 20x4


String myString;
String incomingByte;
int lightSetting = 600;
int tempSetting = 25;
int humiSetting = 60;

int h = 0;
int m = 0;
int s = 0;

int hmin = 3;
int hmax = 5;

bool control;

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();
}

void loop() {

  delay(1000);
  float humiValue  = dht.readHumidity();
  float tempValue = dht.readTemperature();
  int lightValue = analogRead(A0);  

  String placeholdera = "";
  String placeholderb = "";
  String placeholderc = "";
  String placeholderd = "";
  String msg = "";
if (Serial.available() > 0)
 {
   while(Serial.available())
   {
      msg = Serial.readString();

      String placeholdera = stringSplitter(msg, ':', 0);
      String placeholderb = stringSplitter(msg, ':', 1);
      String placeholderc = stringSplitter(msg, ':', 2);
      String placeholderd = stringSplitter(msg, ':', 3);

      if (placeholdera == "set")
      {
        humiSetting = placeholderb.toInt();
        tempSetting = placeholderc.toInt();
        lightSetting = placeholderd.toInt();
      }

      if (placeholdera == "time")
      {
        h = placeholderb.toInt();
        m = placeholderc.toInt();
        s = placeholderd.toInt();
      }

      if (placeholdera == "timeset")
      {
        hmin = placeholderb.toInt();
        hmax = placeholderc.toInt();
      }

      if (placeholdera == "control")
      {
        control = true;
      }

      
        if(control = true)
        {
          if (placeholdera == "electricon")
          {
            digitalWrite(10, HIGH);
          } else if (placeholdera == "electricoff")
          {
            digitalWrite(10, LOW);
          }
            
          if (placeholdera == "lighton")
          {
            digitalWrite(11, HIGH);
          } else if (placeholdera == "lightoff")
          {
            digitalWrite(11, LOW);
          }
          if (placeholdera == "tempon")
          {
            digitalWrite(12, HIGH);
          } else if (placeholdera == "tempoff")
          {
            digitalWrite(12, LOW);
          }
          if (placeholdera == "humion")
          {
            digitalWrite(13, HIGH);
          } else if (placeholdera == "humioff")
          {
            digitalWrite(13, LOW);
          }
          if (placeholdera == "controloff")
          {
            control = false;
          }
        }
      }
      
   }


  ur();

    // LCD setup

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(humiValue);
    lcd.setCursor(10, 0);
    lcd.print(humiSetting);
    lcd.setCursor(0, 1);
    lcd.print(tempValue);
    lcd.setCursor(10, 1);
    lcd.print(tempSetting);
    lcd.setCursor(0, 2);
    lcd.print(lightValue);
    lcd.setCursor(10, 2);
    lcd.print(lightSetting);
    lcd.setCursor(0,3);
    lcd.print(h);
    lcd.print(":");
    lcd.print(m);
    lcd.print(":");
    lcd.print(s);
    lcd.setCursor(10,3);
    lcd.print(hmin);
    lcd.print(":");
    lcd.print(hmax);
    //Automatisk tænding af lamperne

  if(control == false)
  {
    if(lightValue >= lightSetting)
    {
      digitalWrite(11, HIGH);
    } else if (lightValue < lightSetting)
    {
      digitalWrite(11, LOW);
    }
  
    if(tempValue >= tempSetting)
    {
      digitalWrite(12, HIGH);
    } else if (tempValue < tempSetting)
    {
      digitalWrite(12, LOW);
    }
  
    if(humiValue > humiSetting)
    {
      digitalWrite(13, HIGH);
    } else if (humiValue < humiSetting)
    {
      digitalWrite(13, LOW);
    }
  
    if(hmin < hmax)
    {
       if(h >= hmin && h < hmax)
      {
        digitalWrite(10, HIGH);
      } else {
        digitalWrite(10, LOW);
      }
    } else if (hmin > hmax)
    {
      if(h >= hmin || h <= hmax)
      {
        digitalWrite(10, HIGH);
      } else {
        digitalWrite(10, LOW);
      }
    }
  }

  
}

//Funktion til at splitte strings op

String stringSplitter(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

// Styrer tiden for uret

void ur()
{
  s = s + 1;

  if (s == 60)
  {
    s = 0;
    m = m + 1;
  }
  if (m == 60)
  {
    m = 0;
    h = h + 1;
  }
  if (h == 25)
  {
    h = 1;
  }
  
}
