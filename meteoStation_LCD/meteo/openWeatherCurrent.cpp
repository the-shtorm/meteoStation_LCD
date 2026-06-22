/**The MIT License (MIT)

  Copyright (c) 2018 by ThingPulse Ltd., https://thingpulse.com

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#pragma once


#ifdef ESP8266
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#endif

#ifdef ESP32
#include <WebServer.h>
#include <WiFi.h>
#include <HTTPClient.h>
#endif
#include "openWeatherCurrent.h"


OpenWeatherMapCurrent::OpenWeatherMapCurrent() {

}

bool OpenWeatherMapCurrent::updateCurrent(OpenWeatherMapCurrentData *data, String &url) {
  return doUpdate(data, url);
}

bool OpenWeatherMapCurrent::doUpdate(OpenWeatherMapCurrentData *data, String url) {
  unsigned long lostTest = 10000UL;
  unsigned long lost_do = millis();
  bool ret = true;
  this->weatherItemCounter = 0;
  this->data = data;
  JsonStreamingParser parser;
  parser.setListener(this);
  WiFiClient client;
  if (client.connect("api.openweathermap.org", 80))
  {
    bool isBody = false;
    char c;
    //    int size;
    client.print("GET " + url + " HTTP/1.1\r\n"
                 "Host:api.openweathermap.org\r\n"
                 "Connection: close\r\n\r\n");


    while (client.connected() || client.available()) {
      if (client.available()) {
        if ((millis() - lost_do) > lostTest) {
          Serial.println (F("lost in client with a timeout"));
          client.stop();
          //	ESP.restart();
          ret = false;;
          break;
        }
        c = client.read();
        if (c == '{' || c == '[') {
          isBody = true;
        }
        if (isBody) {
          parser.parse(c);
        }
      }
      // give WiFi and TCP/IP libraries a chance to handle pending events
      yield();
    }
  }

  else
  {
    Serial.println(F("ERR connect openW"));
    ret = false;
  }
  this->data = nullptr;
  return ret;
}

void OpenWeatherMapCurrent::whitespace(char c) {
  // Serial.println("whitespace");
}

void OpenWeatherMapCurrent::startDocument() {
  //  Serial.println("start document");
  this->data->rain = 0;
  this->data->snow = 0 ;
#ifdef EN
  this->data->description = "";
#endif
  this->data->icon = "";
  this->data->cod = 0;
}

void OpenWeatherMapCurrent::key(String key) {
  currentKey = String(key);
}

//----------------------------------------------------Begin
#ifndef api_now_2023
void OpenWeatherMapCurrent::value(String value) {
  //  Serial.print("currentParent "); Serial.println(currentParent);
  //  Serial.print("currentKey "); Serial.println(currentKey);
  //  Serial.print("value "); Serial.println(value);

  if (currentParent == "current" ) {


    if (currentKey == "temp") {
      this->data->temp = round(value.toFloat());
    }

    if (currentKey == "pressure") {
      this->data->pressure = value.toInt();
    }

    if (currentKey == "humidity") {
      this->data->humidity = value.toInt();
    }


    if (currentKey == "wind_speed") {
      this->data->windSpeed = round(value.toFloat());
    }

    if (currentKey == "wind_deg") {
      this->data->windDeg = value.toFloat();
    }

    if (currentKey == "dt") {
      this->data->observationTime = value.toInt();
    }


    if (currentKey == "sunrise") {
      this->data->sunrise = value.toInt();
    }

    if (currentKey == "sunset") {
      this->data->sunset = value.toInt();
    }
    if (currentKey == "uvi") {
      this->data->uvi = value.toInt();
    }
    if (currentKey == "timezone") {
      this->data->cityName = value;
    }
  } //curr
  //-----------------------------------
  if (currentParent == "weather" ) {
    if (currentKey == "id") {
      this->data->cod = value.toInt();
    }
#ifdef EN
    if (currentKey == "description") {
      this->data->description = value;
    }
#endif
    if (currentKey == "icon") {
      this->data->icon = value;
    }
  }//weather


  if (currentParent == "rain") {
    if (currentKey == "1h")
      this->data->rain = value.toFloat(); //обрезаю мусор
  }
  if (currentParent == "snow") {
    if (currentKey == "1h")
      this->data->snow = value.toFloat();
  }

}
#endif



//-----------2023-----------------------------------------Begin
#ifdef api_now_2023
void OpenWeatherMapCurrent::value(String value) {

  //  Serial.print("currentParent "); Serial.println(currentParent);
  //  Serial.print("currentKey "); Serial.println(currentKey);
  //  Serial.print("value "); Serial.println(value);

  if (currentParent == "main" ) {

    if (currentKey == "temp") {
      this->data->temp = round(value.toFloat());
    }

    if (currentKey == "pressure") {
      this->data->pressure = value.toInt();
    }

    if (currentKey == "humidity") {
      this->data->humidity = value.toInt();
    }
  } //main

  //-----------------------------------
  if (currentParent == "weather" ) {
    if (currentKey == "id") {
      this->data->cod = value.toInt();
    }
#ifdef EN
    if (currentKey == "description") {
      this->data->description = value;
    }
#endif
    if (currentKey == "icon") {
      this->data->icon = value;
    }
  }//weather

  if (currentParent == "wind" ) {
    if (currentKey == "speed") {
      this->data->windSpeed = round(value.toFloat());
    }

    if (currentKey == "deg") {
      this->data->windDeg = value.toFloat();
    }
  }//wind


  if (currentKey == "dt") {
    this->data->observationTime = value.toInt();
  }

  if (currentParent == "sys" ) {
    if (currentKey == "sunrise") {
      this->data->sunrise = value.toInt();
    }

    if (currentKey == "sunset") {
      this->data->sunset = value.toInt();
    }
  }//sys
  /*
    if (currentKey == "uvi") {
      this->data->uvi = value.toInt();
    }

    if (currentKey == "timezone") {
      this->data->cityName = value;
    }
  */

  if (currentParent == "rain") {
    if (currentKey == "1h")
      this->data->rain = value.toFloat(); //обрезаю мусор
  }
  if (currentParent == "snow") {
    if (currentKey == "1h")
      this->data->snow = value.toFloat();
  }

}
#endif

//------------------E
void OpenWeatherMapCurrent::endArray() {

}


void OpenWeatherMapCurrent::startObject() {
  currentParent = currentKey;
}

void OpenWeatherMapCurrent::endObject() {
#ifndef api_now_2023
  if (currentParent == "weather") {
    weatherItemCounter++;
  }
  currentParent = "";
#endif
}

void OpenWeatherMapCurrent::endDocument() {

}

void OpenWeatherMapCurrent::startArray() {

}
