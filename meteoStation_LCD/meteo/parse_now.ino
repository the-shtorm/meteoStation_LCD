#ifdef openw
bool do_parse_now()
{
#ifdef plotter
  do_plotter('N');
#endif

  bool find = false;
  url = url_b;
#ifndef api_now_2023
  url += F("onecall?lat=");
#else
  url += F("weather?lat=");
#endif
  url += lat;
  url += F("&lon=");
  url += lon;
  url += F("&appid=");
  url += appid;
#ifndef EN
#ifndef api_now_2023
  url += F("&lang=RU&units=metric&exclude=minutely,hourly,daily,alerts");
#else
  url += F("&lang=RU&units=metric");
#endif

#else //EN
#ifndef api_now_2023
  url += F("&lang=EN&units=metric&exclude=minutely,hourly,daily,alerts");
#else
  url += F("&lang=EN&units=metric");
#endif

#endif


#ifdef debug
  Serial.print(F("do_parse_now()len:"));
  Serial.println(url.length());
  Serial.println(url);
  serial_time_start();
#endif
  //------------------------------------------------

  OpenWeatherMapCurrentData data;
  if (owNOW.updateCurrent(&data, url) == true)
  {
    if (data.cod != 0 && data.icon != "")
    {
      do_init_now();
      find = true;


      //pppppppppppppppppp pppppppppppppppppppppppppppp
      if (data.pressure > 900 && data.pressure < 1100)
      {
#ifndef hPa
        davlenie0 = int(data.pressure * 0.75) + delta_p;
#else
        davlenie0 = data.pressure + delta_p;
#endif
        if (p7_max < davlenie0) p7_max = davlenie0;
        if (p7_min > davlenie0) p7_min = davlenie0;
      }
      else
      {
#ifdef debug
        Serial.print(F("data.pressure ERR:"));
        Serial.println(data.pressure);
#endif
      }
      time_t time ;
      time = data.observationTime + TIMEZONE * 3600L;;
      day_ch[0] = hour(time);
#ifdef debug
      Serial.print(F("Time parse now="));
      Serial.println( ctime(&time));
#endif
      icon_ch[0] = data.icon;
      temperatura_ch[0] = data.temp;
      v_ch[0] = data.windSpeed; //veter
      v_gr_ch[0] = data.windDeg;
 
#ifndef api_now_2023      
      uv = data.uvi;
#endif
      /////////rainrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
      if (data.rain > 0.1) //отсеиваю мелочь
        rain_ch[0] = data.rain;
      if (data.snow > 0.1 && data.snow > data.rain)
        rain_ch[0] = data.snow; ///20);  //снег см примерно в мм дождя


      time = data.sunrise + TIMEZONE * 3600L;
      t_sun_v = do_sun_format(hour(time), minute(time));
      t_sun_v_hh = hour(time);
      t_sun_v_min = hour(time) * 60 + minute(time);

      time = data.sunset + TIMEZONE * 3600L;;
      t_sun_z = do_sun_format(hour(time), minute(time));
      t_sun_z_hh = hour(time);
      t_sun_z_min =  hour(time) * 60 + minute(time);
#ifdef debug
      Serial.print(F("t_sun_v_min="));
      Serial.println(t_sun_v_min);
      Serial.print(F("t_sun_z_min="));
      Serial.println(t_sun_z_min);
#endif
#ifdef EN
      descript_now = data.description.c_str();
#endif

#ifdef debug
      log_str = F("PARSE now P=");
      log_str += davlenie0;
      log_str += F(";T=");
      log_str += data.temp;
      log_str += F(";icon=");
      log_str += data.icon;
      log_str += F(";mm=");
      log_str += String(rain_ch[0]);
      Serial.println(log_str);
#endif



#ifdef debug
      Serial.print(F("t_sun_v="));
      Serial.print(t_sun_v);
      Serial.print(F(" t_sun_z="));
      Serial.println(t_sun_z);
#ifdef EN
      Serial.print(F("description="));
      Serial.println( data.description.c_str());
#endif
      Serial.print(F("icon="));
      Serial.println( data.icon.c_str());
      Serial.print(F("temp="));
      Serial.println(  data.temp);
      Serial.print(F("pressure="));
#ifndef hPa
      Serial.println( data.pressure * 0.75);
#else
      Serial.println( data.pressure);
#endif


      Serial.print(F("humidity="));
      Serial.println( data.humidity);
      Serial.print(F("windSpeed="));
      Serial.println(  data.windSpeed);
      Serial.print(F("windDeg="));
      Serial.println(data.windDeg);
      Serial.print(F("rain="));
      Serial.println(data.rain);
      Serial.print(F("snow="));
      Serial.println(data.snow);
      time = data.sunrise + TIMEZONE * 3600L;;
      Serial.print(F("sunrise="));
      Serial.println(ctime(&time));
      time = data.sunset + TIMEZONE * 3600L;;
      Serial.print(F("sunset="));
      Serial.println(  ctime(&time));
      Serial.print(F("cityName="));
      Serial.println( data.cityName.c_str());
#ifndef api_now_2023
      Serial.print(F("UV="));
      Serial.println(  data.uvi);
#endif
      Serial.print(F("cod="));
      Serial.println( data.cod);
      Serial.println(F("---------------------------------------------------/\n"));
      Serial.print(F("do_parse_END mem="));
      Serial.println( ESP.getFreeHeap());
#endif

    }
    else
    {
      Serial.print(F("ERR parse now data.cod=")); Serial.println(data.cod);
    }
  }
  else
  {
    Serial.println(F("ERR parse now ret false"));
  }

  postingInterval_now = find ? t_period_now * 1000L : 180L * 1000L;
  lastConnectionTime_now = millis();
#ifdef plotter
  if (!find) do_plotter('n');
#endif
  return find;
}//do_parse_ch




//-----------------------------------------------------------------
//                          do_init_now()
//-------------------------------------------------------------
void do_init_now()
{
  davlenie0 = -100;
  icon_ch[0] = ""; //1 - min -700
  temperatura_ch[0] = -100; // 1 -min +100
  day_ch[0] = -100; //
  v_ch[0] = -100; //veter
  v_gr_ch[0] = 999; //напр.
  rain_ch[0] = 0; //rain
#ifdef EN
  descript_now = "";
#endif
  uv = -1; //reset uv
}


#endif
