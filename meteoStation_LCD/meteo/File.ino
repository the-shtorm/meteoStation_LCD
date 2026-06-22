//==========================================================================================================
//                        do_parse_cron
//==========================================================================================================
bool do_parse_cron(String& str)
{
  byte count = 0;

  str.trim();
  if (str[0] != '#') return false;
#ifdef debug
  Serial.print(F("cron:\t"));    Serial.print(str);
#endif
  GParser data((char*) str.c_str(), ';');
  count = data.split();

#ifdef debug
  Serial.print(F("-->count par:"));    Serial.print(count);
  Serial.print(F(" "));
#endif
  if (count != 8 || count == 0)
  {
#ifdef debug
    Serial.println("");
#endif
    return false;
  }
#ifdef debug
  for (byte par = 0; par < count; par++)
  {
    if (par == 0) Serial.print(F("Macr"));
    else if (par == 1) Serial.print(F("Hour"));
    else if (par == 2) Serial.print(F("Min"));
    else if (par == 3) Serial.print(F("Day"));
    else if (par == 4) Serial.print(F("Weekday"));
    else if (par == 5) Serial.print(F("CMD"));
    else if (par == 6) Serial.print(F("Tb"));
    else if (par == 7) Serial.print(F("Te"));

    Serial.print(F(":{"));
    Serial.print( data[par]);
    //   Serial.print( ";Int:");
    //   Serial.print( data.getInt(par));
    Serial.println(F("} "));
  } //for par count
#endif
  //#;Hour;Min;Day(1-31);Weekday(1-7);CMD;tbegin(hour);tend(hour);
  //------------------------------------macros
#ifdef pin_pir
  if (data.equals(0, "#PIR_ON") &&  pir_status != 1)
  {
    Serial.println(F("\tPIR_ON skip stat"));
    return false;
  }
  if (data.equals(0, "#PIR_OFF") &&  pir_status != 0)
  {
    Serial.println(F("\tPIR_OFF skip stat"));
    return false;
  }
#endif
  //-------------HOUR
  if (!data.equals(1, "*") && data.getInt(1) != now_hour)
  {
#ifdef debug
    Serial.println(F("\tHOUR skip"));
#endif
    return false;
  }


  //-------------MINUTE
  mString<10> min_; min_ = data[2];
  byte min_div = 100;
  if (min_.buf[0] == '%')
  {
    int8_t min_int_ = 99;
    min_.buf[0] = ' ';
    min_int_ = min_.toUint(1);
    if (min_int_ > 0)  min_div = now_min % min_int_;
#ifdef debug
    if (min_div != 0)
      Serial.print(F("\t(now_min % minute_int_)SKIP <>0:")); Serial.println(min_div);
#endif
  }
  if (!data.equals(2, "*") && data.getInt(2) != now_min && min_div != 0)
  {
#ifdef debug
    Serial.println(F("\tMINUTE skip"));
#endif
    return false;
  }

  //-------------DAY
  if (!data.equals(3, "*") && data.getInt(3) != now_day)
  {
#ifdef debug
    Serial.println(F("\tDAY skip"));
#endif
    return false;
  }

  //-------------Weekday
  if (!data.equals(4, "*") && data.getInt(4) != now_weekday)
  {
#ifdef debug
    Serial.println(F("\tWeekday skip"));
#endif
    return false;
  }

  if (!data.equals(6, "*") && !data.equals(7, "*"))
  { //----------------------------T_end > T_begin
    if (data.getInt(7) > data.getInt(6)) //Te>Tb ////Tb=10 Te=22
    {
      if (now_hour < data.getInt(6) || now_hour >= data.getInt(7)  )
      {
#ifdef debug
        Serial.println(F("\tT_b or T_e skip"));
#endif
        return false;
      }
    } //----------------------------T_end < T_begin Tb=22 Te=2
    else if  (data.getInt(7) < data.getInt(6))
    {
      if (now_hour < data.getInt(6) && now_hour >= data.getInt(7)) //
      {
#ifdef debug
        Serial.println(F("\tT_b or T_e skip"));
#endif
        return false;
      }
    }
  } //!=*

  str = data[5];
  if (str.length() > 0)
  {
#ifdef debug
    Serial.print(F("\t----CRON Add "));
    Serial.print(F("cmd:")); Serial.println(str);
#endif
    return true;
  }
  //else fn_cmd = "";
  else str = "";
  return false;
}
//==========================================================================================================
//                        do_read_cron_txt
//==========================================================================================================
void do_read_cron_txt()
{
  String cmd_list = "";
  cmd_list.reserve(100);
  static uint8_t min_last = 99;


#ifdef pin_sd
  File f = SD.open( "/cron.txt", FILE_READ);  // or, file handle reference for SD library
#else
  fs::File f = SPIFFS.open( "/cron.txt", "r");    // File handle reference for SPIFFS
#endif
  if (!f) {
    Serial.println(F("ERR open cron.txt"));
    return ;
  }
  //-------------------
#ifdef debug
  Serial.println(' ');
  Serial.print(F("Hour:"));  Serial.print( now_hour);
  Serial.print(F("\tMin:"));  Serial.print( now_min);
  Serial.print(F("\tDay:"));  Serial.print( now_day);
  Serial.print(F("\tWeek:"));  Serial.println( now_weekday);
#endif
  while (f.available()) {
    log_str = f.readStringUntil('\n');
    log_str.replace(" ", "");
    //уже обработан, только макросы
    if (min_last == now_min && log_str[1] == ';' ) continue;

    if (do_parse_cron(log_str))
    {
      if (cmd_list != "") cmd_list += "#";
      cmd_list += log_str;
#ifdef debug
      //      Serial.print(F("\t cmd_list:")); Serial.println(cmd_list);
#endif
    }
  }
  f.close();
  min_last = now_min;

  //START  cmd_list
  if (cmd_list != "")
  {
#ifdef debug
    Serial.print(F("\t------CMD_ALL:")); Serial.print(cmd_list);
#endif
    //do
    GParser data((char*) cmd_list.c_str(), '#');
    byte count = data.split();
#ifdef debug
    Serial.print(F(" Count:"));
    Serial.println( count);
#endif
    //1111111111111111111111111
    for (byte i = 0; i < count; i++)
    {
#ifdef debug
      Serial.print(F("\tCMD_")); Serial.print(i);
      Serial.print(F(" {"));
      Serial.print( data[i]);
      Serial.print(F("} "));
#endif
      log_str = data[i];
      if (strncmp(log_str.c_str(), "-out", 4) == 0 || strncmp(log_str.c_str(), "-led", 4) == 0)
      {
        log_str[0] = '=';
        Serial.println(F("cron (out || led): repl '='"));
        do_parse_str(log_str);
      }
      else
        do_short_cmd(log_str);

    }
    Serial.println("");
  }

}


//==========================================================================================================
//                        do_read_config_txt
//==========================================================================================================
bool do_read_config_txt()
{

  //  const char filename[] = "/config.txt";
  bool val_0 = false;
  scr_show_count = 0;
  uint16_t len;
#ifdef pin_sd
  File f = SD.open( "/config.txt", FILE_READ);  // or, file handle reference for SD library
#else
  fs::File f = SPIFFS.open( "/config.txt", "r");    // File handle reference for SPIFFS
#endif


  if (!f) {
    Serial.println(F(" ERR open config.txt"));
    return false;
  }

  //----------------------------
  String name, val, str;
  str.reserve(150);
  name.reserve(150);
  str.reserve(150);

#ifdef debug
  Serial.println(F("-------config.txt"));
#endif
  //----------------------------------
  while (f.available()) {
    str = f.readStringUntil('\n');
    str.trim();
    int tz = str.indexOf("#") ;
    if (tz != 0) continue;

    //---------------------------
    int tz2 = str.indexOf(";", tz + 1);
    if (tz2 == -1) continue;
    //name
    name = str.substring(tz + 1, tz2);
    name.trim();
    //val-----------------
    tz = tz2 + 1;
    tz2 = str.indexOf(";", tz);
    if (tz2 == -1) continue;
    val = str.substring(tz, tz2);
    val.trim();
    len = val.length();
    if (val == "0") val_0 = true; else val_0 = false; //если =0

    //kkkkkkkkkkkkkkkkkkkkkkkkkkkkey
#if defined (openw)
    if (name == "key")
    {
      if (len > 0)
      {
        appid = val;
      }
      else appid = "";
#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(appid);
#endif
    }
#endif
    if (name == "p_min")
    {
      if (val.toInt() > 0) p_min = val.toInt();
      else p_min = 0;
#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(p_min);
#endif
    }

    if (name == "p_max")
    {
      if (val.toInt() > 0) p_max = val.toInt();
      else p_max = 10000;
#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(p_max);
#endif
    }


#if defined (tft2_Sf)&&defined (def_core0)
    if (name == (char *)"Sf2_slide_on_play")
    {
      if (val.toInt() > 0) Sf2_slide_on_play = val.toInt();
#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(Sf2_slide_on_play);
#endif
    }
#endif
    if (name == "v_max")
    {
      if (val.toInt() > 0) v_max = val.toInt();
      else v_max = 99;
#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(v_max);
#endif
    }

    if (name == "TIMEZONE")
    {
      if (len > 0) TIMEZONE = val.toInt();
      else TIMEZONE = 0;
#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(TIMEZONE);
#endif
    }


    if (name == "lat")
    {
      if (len > 0) lat = val;
      else lat = "";
#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(lat);
#endif
    }

    if (name == "lon")
    {
      if (len > 0) lon = val;
      else lon = "";
#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(lon);
#endif
    }


    if (name == "t_max")
    {
      if (val.toInt() != 0 || val_0 == true) t_max = val.toInt();
      else t_max = 100;
#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(t_max);
#endif
    }


    if (name == "t_min")
    {
      if (val.toInt() != 0 || val_0 == true) t_min = val.toInt();
      else t_min = -100;
#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(t_min);
#endif
    }


    if (name == "uv_max")
    {
      if (val.toInt() != 0 || val_0 == true) uv_max = val.toInt();
      else uv_max = 100;
#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(uv_max);
#endif
    }


    if (name == "delta_p")
    {
      if (val.toInt() != 0 || val_0 == true) delta_p = val.toInt();
#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(delta_p);
#endif
    }

    if (name == "delta_t")
    {
      if (val.toInt() != 0 || val_0 == true) delta_t = val.toInt();
#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(delta_t);
#endif
    }


    if (name == "delta_t0")
    {
      if (val.toInt() != 0 || val_0 == true) delta_t0 = val.toInt();
#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(delta_t0);
#endif
    }


    if (name == "beep_b")
    {
      if (val.toInt() > 0 || val_0 == true) beep_b = val.toInt();
      else beep_b = 0;
#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(beep_b);
#endif
    }

    if (name == "beep_e")
    {
      if (val.toInt() > 0 || val_0 == true) beep_e = val.toInt();
      else beep_e = 0;
#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(beep_e);
#endif
    }

#ifdef AUDIO
    //WWW
    if (name == "play_vol")
    {
      int pl_vol = val.toInt();
      if (pl_vol > 0 && pl_vol < 101)
      {
        vol_def = pl_vol;
      }
      //    else pl_vol = vol_def_mp3; //init 80

#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(vol_def);
#endif
    }
#endif


#if defined (pin_sd)&&defined (AUDIO)
    if (name == "beep_mp3_vol")
    {
      int vol_mp3 = val.toInt();
      if (vol_mp3 > 0 && vol_mp3 < 101)
      {
        beep_mp3_vol = vol_mp3;
        if (beep_mp3_vol == 100) beep_mp3_vol = 99;

      }
      else beep_mp3_vol = 0;

#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(beep_mp3_vol);
#endif
    }

    if (name == "play_mp3_vol")
    {
      int pl_vol_mp3 = val.toInt();
      if (pl_vol_mp3 > 0 && pl_vol_mp3 < 101)
      {
        vol_def_mp3 = pl_vol_mp3;
        //  if (volume_mp3 == 100) volume_mp3 = 99;

      }
      //     else volume_mp3 = vol_def_mp3; //init=80

#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(vol_def_mp3);
#endif
    }

    //sort
    if (name == "mp3_random")
    {
      mp3_random = true;

#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(mp3_random);
#endif
    }
#endif //sd+audio


#if defined (fmSi4703)|| defined (fmRDA5807)
    if (name == "play_fm_vol")
    {
      int vol_def_tmp = val.toInt();
      if (vol_def_tmp > 0 && vol_def_tmp < 16)
      {
        vol_def_fm = vol_def_tmp;
      }


#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(vol_def_fm);
#endif
    }
#endif //FM



    if (name == "t_b")
    {
      if (val.toInt() != 0 || val_0 == true) t_b = val.toInt();
#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(t_b);
#endif
    }


    if (name == "t_e")
    {
      if (val.toInt() != 0 || val_0 == true) t_e = val.toInt();
#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(t_e);
#endif
    }


    if (name == "scr_name_def")
    {
      if (len > 0)
      {
        byte len = val.length() + 1;
        char buf[len];
        val.toCharArray(buf, len);
        strcpy(scr_name_def, buf);
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(scr_name_def);
#endif

      }
    }


    //scr round

    if (name == (char *)"S1_show")
    {
      if (val.toInt() > 0)
      {
#ifdef S1
        t_S1_show = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(t_S1_show);
#endif
        scr_show_count++;
#endif
      }
    }


    if (name == (char *)"S2_show")
    {
      if (val.toInt() > 0)
      {
#ifdef S2
        t_S2_show = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(t_S2_show);
#endif
        scr_show_count++;
#endif
      }
    }


    if (name == (char *)"S3_show")
    {
#ifndef  tft2_S3
      if (val.toInt() > 0)
      {
#ifdef S3
        t_S3_show = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(t_S3_show);
#endif
        scr_show_count++;
#endif
      }
#endif //tft2_S3

    }


    if (name == (char *)"S4_show")
    {
#ifndef  tft2_S4
      if (val.toInt() > 0)
      {
#ifdef S4
        t_S4_show = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(t_S4_show);
#endif
        scr_show_count++;
#endif
      }
#endif //tft2_S4
    }


    if (name == (char *)"S5_show")
    {
      if (val.toInt() > 0)
      {
#ifdef S5
        t_S5_show = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(t_S5_show);
#endif
        scr_show_count++;
#endif
      }
    }


    if (name == (char *)"S6_show")
    {
      if (val.toInt() > 0)
      {
#ifdef S6
        t_S6_show = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(t_S6_show);
#endif
        scr_show_count++;
#endif
      }
    }



    if (name == (char *)"Sf_show")
    {
      if (val.toInt() > 0)
      {
#ifdef Sf
        t_Sf_show = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(t_Sf_show);
#endif
        scr_show_count++;
#endif
      }
    }



    if (name == (char *)"Sg_show")
    {

#ifndef  tft2_Sg
      if (val.toInt() > 0)
      {
#ifdef Sg
        t_Sg_show = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(t_Sg_show);
#endif
        scr_show_count++;
#endif
      }

#endif //tft2_Sg
    }




    if (name == (char *)"S0_show")
    {
      if (val.toInt() > 0)
      {
#ifndef openw
        t_S0_show = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(t_S0_show);
#endif
        scr_show_count++;
#endif
      }
    }




#if defined (Sf)||defined (Sfepd)
    if (name == (char *)"Sf_t_slide")
    {
      if (val.toInt() > 0)
      {

        Sf_t_slide = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(Sf_t_slide);
#endif

      }
    }


#endif

    //-----------------TIMER------------------------
    if (name == "tm_alarm_period")
    {
      if (val.toInt() > 0)
      {
        tm_alarm_period = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(tm_alarm_period);
#endif

      }
    }

    if (name == "tm_alarm_count")
    {
      if (val.toInt() > 0)
      {

        tm_alarm_count = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(tm_alarm_count);
#endif

      }
    }


#ifdef pin_beep
    if (name == "tm_alarm_count_beep")
    {
      if (val.toInt() > 0)
      {
        tm_alarm_count_beep = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(tm_alarm_count_beep);
#endif

      }
    }

    if (name == "tm_alarm_period_beep")
    {
      if (val.toInt() > 0)
      {
        tm_alarm_period_beep = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(tm_alarm_period_beep);
#endif

      }
    }

#endif //beep
    //----------------





    //------------------------IoT-------------------------------------------
#if defined (iot_send)||defined (iot_read)
    //iot_id-------------
    if (name == "iot_id" || name == "iot_id_read")
    {
      String iot_id_tmp;
      if (len > 0)
      {
        iot_id_tmp = val;
        char **pointer, *stringVar;
        unsigned long unsignedVar;
        char charBufVar[20];
        iot_id_tmp.toCharArray(charBufVar, 20);
        stringVar = charBufVar;
        unsignedVar = strtoul(stringVar, pointer, 10);
        if (unsignedVar > 0)
        {
          if (name == "iot_id") iot_id = unsignedVar;
          if (name == "iot_id_read") iot_id_read = unsignedVar;

#ifdef debug
          Serial.print(name); Serial.print(':');
          Serial.println(unsignedVar);
#endif
        }


      }
    }



    if (name == "iot_key" || name == "iot_key_read")
    {
      if (len > 0)
      {

        if (name == "iot_key") iot_key = val;
        if (name == "iot_key_read") iot_key_read = val;

#ifdef debug
        Serial.print(name); Serial.print(':');
        if (name == "iot_key") Serial.println(iot_key);
        if (name == "iot_key_read") Serial.println(iot_key_read);
#endif

      }
      else
        iot_key = "";

    }

    //period send sec>=20
    if (name == "iot_period_send" || name == "iot_period_read")
    {
      String iot_period_send_tmp;
      if (len > 0)
      {
        iot_period_send_tmp = val;
        char **pointer, *stringVar;
        unsigned long unsignedVar;

        char charBufVar[20];
        iot_period_send_tmp.toCharArray(charBufVar, 20);

        stringVar = charBufVar;
        unsignedVar = strtoul(stringVar, pointer, 10); //=324234
        if (unsignedVar > 0)
        {
          if (name == "iot_period_send") iot_period_send = unsignedVar;
          if (name == "iot_period_read") iot_period_read = unsignedVar;
        }

#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(unsignedVar);
#endif
      }
    }



    //-------VCC read
    if (name == "iot_VCC_fld_read")
    {
      if (val.toInt() > 0 && val.toInt() < 10)
        iot_VCC_fld_read = val.toInt();

#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(val.toInt());
#endif
    }


    //temp квартира(room);ном поля(num field) bme280
    if (name == "iot_T1_fld_send" || name == "iot_T1_fld_read")
    {
      if (val.toInt() > 0 && val.toInt() < 10)
      {
        if (name == "iot_T1_fld_send") iot_T1_fld_send = val.toInt();
        if (name == "iot_T1_fld_read") iot_T1_fld_read = val.toInt();
      }
#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(val.toInt());
#endif
    }


    //humidity квартира(room) ;ном поля(num field) bme280

    if (name == "iot_H_fld_send" || name == "iot_H_fld_read")
    {
      if (val.toInt() > 0 && val.toInt() < 10)
      {
        if (name == "iot_H_fld_send") iot_H_fld_send = val.toInt();
        if (name == "iot_H_fld_read") iot_H_fld_read = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(val.toInt());
#endif
      }

    }


    //temp улица(street)
    if (name == "iot_T0_fld_send" || name == "iot_T0_fld_read")
    {
      if (val.toInt() > 0 && val.toInt() < 10)
      {
        if (name == "iot_T0_fld_send") iot_T0_fld_send = val.toInt();
        if (name == "iot_T0_fld_read") iot_T0_fld_read = val.toInt();

#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(val.toInt());
#endif
      }

    }

    //давление(pressure)
    if (name == "iot_P_fld_send" || name == "iot_P_fld_read")
    {
      if (val.toInt() > 0 && val.toInt() < 10)
      {
        if (name == "iot_P_fld_send") iot_P_fld_send = val.toInt();
        if (name == "iot_P_fld_read") iot_P_fld_read = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(val.toInt());
#endif
      }

    }

    //скорость ветра (wind speed)
    if (name == "iot_W_fld_send")
    {
      if (val.toInt() > 0 && val.toInt() < 10)
      {
        iot_W_fld_send = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(iot_W_fld_send);
#endif
      }
    }


    //дождь мм(rain)
    if (name == "iot_R_fld_send")
    {
      if (val.toInt() > 0 && val.toInt() < 10) iot_R_fld_send = val.toInt();
#ifdef debug
      Serial.print(name); Serial.print(':');
      Serial.println(iot_R_fld_send);
#endif
    }



#endif //iot


    //----------------------mqtt
#if defined (mqtt)||defined (mqtt2)||defined (mqtt3)

#ifdef AUDIO
    if (name == "mqtt_plist_topic_send")
    {
      if (len > 0)
      {
        mqtt_plist_topic_send = val;

#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_plist_topic_send);
#endif
      }
    }

    if (name == "mqtt_song_topic_send")
    {
      if (len > 0)
      {
        mqtt_song_topic_send = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_song_topic_send);
#endif
      }

    }
#endif


    if (name == "mqtt_server")
    {
      if (len > 0)
      {
        mqtt_server = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_server);
#endif

      }

    }

    // port
    if (name == "mqtt_port")
    {
      if (val.toInt() > 0 )
      {
        mqtt_port = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_port);
#endif
      }
    }


    if (name == "mqtt_user")
    {
      if (len > 0)
      {
        mqtt_user = val.c_str();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_user);
#endif

      }

    }


    if (name == "mqtt_password")
    {
      if (len > 0)
      {
        mqtt_password = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_password);
#endif

      }

    }


#ifdef mqtt_send
    if (name == "mqtt_H_topic_send")
    {
      if (len > 0)
      {
        mqtt_H_topic_send = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_H_topic_send);
#endif
      }
    }


    if (name == "mqtt_T1_topic_send")
    {
      if (len > 0)
      {
        mqtt_T1_topic_send = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_T1_topic_send);
#endif
      }
    }



    if (name == "mqtt_T0_topic_send")
    {
      if (len > 0)
      {
        mqtt_T0_topic_send = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_T0_topic_send);
#endif
      }
    }

    if (name == "mqtt_P_topic_send")
    {
      if (len > 0)
      {
        mqtt_P_topic_send = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_P_topic_send);
#endif
      }
    }

    if (name == "mqtt_esp_name")
    {
      if (len > 0)
      {
        mqtt_esp_name = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_esp_name);
#endif
      }
    }

    //period reconn mqtt on ERR
    if (name == "t_reconn_mqtt")
    {
      if (len > 0 && (val.toInt() > 0 ))
      {
        t_reconn_mqtt = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(t_reconn_mqtt);
#endif
      }
    }



    //mqtt_period_send
    if (name == "mqtt_period_send" )
    {
      if (len > 0 && (val.toInt() > 0 ))
      {
        mqtt_period_send = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_period_send);
#endif
      }
    }
#endif //send

#ifdef mqtt_read
    if (name == "mqtt_H_topic_read")
    {
      if (len > 0)
      {
        mqtt_H_topic_read = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_H_topic_read);
#endif
      }
    }


    if (name == "mqtt_T1_topic_read")
    {
      if (len > 0)
      {
        mqtt_T1_topic_read = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_T1_topic_read);
#endif
      }
    }

    if (name == "mqtt_T0_topic_read")
    {
      if (len > 0)
      {
        mqtt_T0_topic_read = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_T0_topic_read);
#endif
      }
    }

    if (name == "mqtt_P_topic_read")
    {
      if (len > 0)
      {
        mqtt_P_topic_read = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_P_topic_read);
#endif
      }
    }

    if (name == "mqtt_CMD_topic_read")
    {
      if (len > 0)
      {
        mqtt_CMD_topic_read = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_CMD_topic_read);
#endif
      }
    }


    //out1
#if defined(gpio_out)&&defined(pin_out1)
    if (name == "mqtt_out1_topic_read")
    {
      if (len > 0)
      {
        mqtt_out1_topic_read = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_out1_topic_read);
#endif
      }
    }

#if defined(pin_out1_color_leds)
    if (name == "mqtt_out1_topic_read_color")
    {
      if (len > 0)
      {
        mqtt_out1_topic_read_color = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_out1_topic_read_color);
#endif
      }
    }
    if (name == "mqtt_out1_topic_send_color")
    {
      if (len > 0)
      {
        mqtt_out1_topic_send_color = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_out1_topic_send_color);
#endif
      }
    }
#endif //color
    if (name == "mqtt_out1_topic_send")
    {
      if (len > 0)
      {
        mqtt_out1_topic_send = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_out1_topic_send);
#endif
      }
    }
#endif //ou11

    //out2
#if defined(gpio_out)&&defined(pin_out2)
    if (name == "mqtt_out2_topic_read")
    {
      if (len > 0)
      {
        mqtt_out2_topic_read = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_out2_topic_read);
#endif
      }
    }

    if (name == "mqtt_out2_topic_send")
    {
      if (len > 0)
      {
        mqtt_out2_topic_send = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_out2_topic_send);
#endif
      }
    }
#endif //ou12
    //out3
#if defined(gpio_out)&&defined(pin_out3)
    if (name == "mqtt_out3_topic_read")
    {
      if (len > 0)
      {
        mqtt_out3_topic_read = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_out3_topic_read);
#endif
      }
    }

    if (name == "mqtt_out3_topic_send")
    {
      if (len > 0)
      {
        mqtt_out3_topic_send = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_out3_topic_send);
#endif
      }
    }
#endif //ou13

    //out4
#if defined(gpio_out)&&defined(pin_out4)
    if (name == "mqtt_out4_topic_read")
    {
      if (len > 0)
      {
        mqtt_out4_topic_read = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_out4_topic_read);
#endif
      }
    }

    if (name == "mqtt_out4_topic_send")
    {
      if (len > 0)
      {
        mqtt_out4_topic_send = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_out4_topic_send);
#endif
      }
    }
#endif //ou14

    //out5
#if defined(gpio_out)&&defined(pin_out5)
    if (name == "mqtt_out5_topic_read")
    {
      if (len > 0)
      {
        mqtt_out5_topic_read = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_out5_topic_read);
#endif
      }
    }

    if (name == "mqtt_out5_topic_send")
    {
      if (len > 0)
      {
        mqtt_out5_topic_send = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_out5_topic_send);
#endif
      }
    }
#endif //out5

    if (name == "mqtt_VCC_topic_read")
    {
      if (len > 0)
      {
        mqtt_VCC_topic_read = val;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(mqtt_VCC_topic_read);
#endif
      }
    }

#endif //read
#endif //mqtt



    //----------------wifi force---------------------


    if (name == "wifi_force_connect")
    {
      if (val.toInt() > 0)
      {
        t_wifi_force_connect = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(t_wifi_force_connect);
#endif

      }
    }
    //-------------------LED--------------------------
#if defined (pin_led)
    //   byte led_sunset = 70; // яркость после захода ,after sunset
    //   int led_sunset_delta = 70; // поправка в минутах после захода+-мин ,correction in minutes after sunset+-
    if (name == "led_def")
    {
      if (val.toInt() >= 0 && val.toInt() < 101)
      {
        led_def = val.toInt();
        led_val = led_def;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(led_def);
#endif
      }
    }


    if (name == "led_night")
    {
      if (val.toInt() >= 0 && val.toInt() < 101)
      {
        led_night = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(led_night);
#endif
      }
    }


    if (name == "led_sunset")
    {
      if (val.toInt() >= 0 && val.toInt() < 101)
      {
        led_sunset = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(led_sunset);
#endif
      }
    }

#endif //led
    /*
        //-------------------GPIO_OUT--------------------------
      #if defined (gpio_out)
      #if defined (pin_out1)
        if (name == "pin_out1")
        {
          if (val.toInt() >= 0 && val.toInt() < 101)
          {
            po1_val = val.toInt();
      #ifdef debug
            Serial.print(name + "=");
            Serial.println(po1_val);
      #endif
          }
        }
      #endif //out1

      #if defined (pin_out2)
        if (name == "pin_out2")
        {
          if (val.toInt() >= 0 && val.toInt() < 101)
          {
            po2_val = val.toInt();
      #ifdef debug
            Serial.print(name + "=");
            Serial.println(po2_val);
      #endif
          }
        }
      #endif //out2
      #if defined (pin_out3)
        if (name == "pin_out3")
        {
          if (val.toInt() >= 0 && val.toInt() < 101)
          {
            po3_val = val.toInt();
      #ifdef debug
            Serial.print(name + "=");
            Serial.println(po3_val);
      #endif
          }
        }
      #endif //out3
      #if defined (pin_out4)
        if (name == "pin_out4")
        {
          if (val.toInt() >= 0 && val.toInt() < 101)
          {
            po4_val = val.toInt();
      #ifdef debug
            Serial.print(name + "=");
            Serial.println(po4_val);
      #endif
          }
        }
      #endif //out4

      #if defined (pin_out5)
        if (name == "pin_out5")
        {
          if (val.toInt() >= 0 && val.toInt() < 101)
          {
            po5_val = val.toInt();
      #ifdef debug
            Serial.print(name + "=");
            Serial.println(po5_val);
      #endif
          }
        }
      #endif //out1


      #endif //gpio_out
    */


#if defined (pin_pir)
    if (name == "t_pir_period")
    {
      if (val.toInt() > 0)
      {
        t_pir_period = val.toInt() * 1000L;
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(t_pir_period);
#endif
      }
    }

#endif //t_pir_period

    //-----------------------------------------------KEY1---------------
#if defined (pin_analog_key1)

    if (name == "key1_debounce")
    {
      if (val.toInt() > 0)
      {
        key1_debounce = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key1_debounce);
#endif
      }
    }

    if (name == "key1_timeout")
    {
      if (val.toInt() > 0)
      {
        key1_timeout = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key1_timeout);
#endif
      }
    }


    if (name == "key1_clicktimeout")
    {
      if (val.toInt() > 0)
      {
        key1_clicktimeout = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key1_clicktimeout);
#endif
      }
    }


    if (name == "key1_steptimeout")
    {
      if (val.toInt() > 0)
      {
        key1_steptimeout = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key1_steptimeout);
#endif
      }
    }

#endif //pin_analog_key1


#if defined (pin_analog_key1) && defined( key1btn_1)

    if (name == "key1btn1_min")
    {
      if (val.toInt() >= 0)
      {
        key1btn1_min = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key1btn1_min);
#endif
      }
    }

    if (name == "key1btn1_max")
    {
      if (val.toInt() >= 0)
      {
        key1btn1_max = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key1btn1_max);
#endif
      }
    }

#endif //k1

#if defined (pin_analog_key1) && defined(key1btn_2)
    if (name == "key1btn2_min")
    {
      if (val.toInt() >= 0)
      {
        key1btn2_min = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key1btn2_min);
#endif
      }
    }

    if (name == "key1btn2_max")
    {
      if (val.toInt() >= 0)
      {
        key1btn2_max = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key1btn2_max);
#endif
      }
    }
#endif //k2


#if defined (pin_analog_key1) && defined(key1btn_3)
    if (name == "key1btn3_min")
    {
      if (val.toInt() >= 0)
      {
        key1btn3_min = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key1btn3_min);
#endif
      }
    }

    if (name == "key1btn3_max")
    {
      if (val.toInt() >= 0)
      {
        key1btn3_max = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key1btn3_max);
#endif
      }
    }
#endif //k3


#if defined (pin_analog_key1) && defined(key1btn_4)
    if (name == "key1btn4_min")
    {
      if (val.toInt() >= 0)
      {
        key1btn4_min = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key1btn4_min);
#endif
      }
    }

    if (name == "key1btn4_max")
    {
      if (val.toInt() >= 0)
      {
        key1btn4_max = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key1btn4_max);
#endif
      }
    }
#endif //k4


#if defined (pin_analog_key1) && defined(key1btn_5)
    if (name == "key1btn5_min")
    {
      if (val.toInt() >= 0)
      {
        key1btn5_min = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key1btn5_min);
#endif
      }
    }

    if (name == "key1btn5_max")
    {
      if (val.toInt() >= 0)
      {
        key1btn5_max = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key1btn5_max);
#endif
      }
    }
#endif //key1

    //-----------------------------------------------key2---------------
#if defined (pin_analog_key2)

    if (name == "key2_debounce")
    {
      if (val.toInt() > 0)
      {
        key2_debounce = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key2_debounce);
#endif
      }
    }

    if (name == "key2_timeout")
    {
      if (val.toInt() > 0)
      {
        key2_timeout = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key2_timeout);
#endif
      }
    }


    if (name == "key2_clicktimeout")
    {
      if (val.toInt() > 0)
      {
        key2_clicktimeout = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key2_clicktimeout);
#endif
      }
    }


    if (name == "key2_steptimeout")
    {
      if (val.toInt() > 0)
      {
        key2_steptimeout = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key2_steptimeout);
#endif
      }
    }

#endif //pin_analog_key2


#if defined (pin_analog_key2) && defined( key2btn_1)

    if (name == "key2btn1_min")
    {
      if (val.toInt() >= 0)
      {
        key2btn1_min = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key2btn1_min);
#endif
      }
    }

    if (name == "key2btn1_max")
    {
      if (val.toInt() >= 0)
      {
        key2btn1_max = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key2btn1_max);
#endif
      }
    }

#endif //k1

#if defined (pin_analog_key2) && defined(key2btn_2)
    if (name == "key2btn2_min")
    {
      if (val.toInt() >= 0)
      {
        key2btn2_min = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key2btn2_min);
#endif
      }
    }

    if (name == "key2btn2_max")
    {
      if (val.toInt() >= 0)
      {
        key2btn2_max = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key2btn2_max);
#endif
      }
    }
#endif //k2


#if defined (pin_analog_key2) && defined(key2btn_3)
    if (name == "key2btn3_min")
    {
      if (val.toInt() >= 0)
      {
        key2btn3_min = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key2btn3_min);
#endif
      }
    }

    if (name == "key2btn3_max")
    {
      if (val.toInt() >= 0)
      {
        key2btn3_max = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key2btn3_max);
#endif
      }
    }
#endif //k3


#if defined (pin_analog_key2) && defined(key2btn_4)
    if (name == "key2btn4_min")
    {
      if (val.toInt() >= 0)
      {
        key2btn4_min = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key2btn4_min);
#endif
      }
    }

    if (name == "key2btn4_max")
    {
      if (val.toInt() >= 0)
      {
        key2btn4_max = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key2btn4_max);
#endif
      }
    }
#endif //k4


#if defined (pin_analog_key2) && defined(key2btn_5)
    if (name == "key2btn5_min")
    {
      if (val.toInt() >= 0)
      {
        key2btn5_min = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key2btn5_min);
#endif
      }
    }

    if (name == "key2btn5_max")
    {
      if (val.toInt() >= 0)
      {
        key2btn5_max = val.toInt();
#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(key2btn5_max);
#endif
      }
    }
#endif //key2



    //--------------------------------telegram
#ifdef tlg

    if (name == "tlg_token") //save log
    {
      if (len > 0)
      {
        tlg_token = val;

#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(tlg_token);
#endif
      }
    }

    if (name == "chat_id") //save log
    {
      if (len > 0)
      {
        chat_id = val;

#ifdef debug
        Serial.print(name); Serial.print(':');
        Serial.println(chat_id);
#endif
      }
    }
#endif //tlg


  } //while

#ifdef debug
  Serial.print(F("scr_show_count = "));
  Serial.println(scr_show_count);
#endif



  f.close();
#ifdef debug
  Serial.println(F("-------END-config.txt"));
#endif

  return true;
}


#ifdef pin_ir
//==========================================================================================================
//                              do_read_file_ik
//==========================================================================================================
bool do_read_file_ik()
{
  bool find = false;
  String str_code = "";

#ifdef pin_sd
  File f = SD.open( "/ik.txt", FILE_READ);  // or, file handle reference for SD library
#else
  fs::File f = SPIFFS.open( "/ik.txt", "r");    // File handle reference for SPIFFS
#endif

  if (!f) {
    Serial.println(F(" ERR open ik.txt"));
    return find;
  }

  String str = "";
  //-------------------
  while (f.available()) {
    str = f.readStringUntil('\n');
#ifdef debug
    Serial.println(str);
#endif
    if (str.indexOf("//") == 0) continue;
    if (str == "") continue;
    char **pointer;
    char buf[11];
    int tz = 0;
    int tz2 = 0;
    String func = "";
    ir_code = 0;
    str_code = "";
    //---------------
    tz = str.indexOf("#");
    //Serial.println("#");
    //Serial.println(tz);
    if (tz >= 0)
    {

      //func------------------------
      tz2 = str.indexOf(";", tz + 1);
      if (tz2 > 0)
      {
        func = str.substring(tz + 1, tz2);
        //Serial.println(func);
      }

      //code ir-----------------
      tz = tz2;
      tz2 = str.indexOf(";", tz + 1);
      if (tz2 > 0)
      {
        //Serial.println(str.substring(tz+1,tz2));
        str_code = str.substring(tz + 1, tz2);
        str_code.trim();
        //Serial.println(code);
        str_code.toCharArray(buf, sizeof(buf));
        ir_code = atoll(buf);


#ifdef debug
        Serial.println(func);
        Serial.println(ir_code);
#endif
      }
      if (func != "" && ir_code != 0)
      {
        //если кнопка
        if (func.substring(0, 2) == "bt")
        {
          String func_btn = func.substring(2);
          int num_btn = func_btn.toInt();
          if (num_btn < 10 && num_btn >= 0) code_btn[num_btn] = ir_code;
#ifdef debug
          Serial.print (F("num_btn=")); Serial.println(num_btn);
#endif
          continue;
        }
        if (func.substring(0, 2) == "id")
        {

          if (ir_code > 0) id_ir = str_code;//string
#ifdef debug
          Serial.print (F("id IR=")); Serial.println(id_ir);
#endif
          continue;
        }
        //ищу фун
        for (int i = 0; i < fn_count; i++) {

          if (func == fn[i]) {
            code[i] = ir_code;
#ifdef debug
            Serial.print(F("FIND----->")); Serial.print (func);
            Serial.println(code[i]);
#endif
            find = true;
          }
        }



      }



    }

  }
  f.close();
  return find;
}

#endif // pin_ir



//--------------------------------------------------------
//                      do_count_pl_list
//--------------------------------------------------------
#if defined ESP32 && defined(AUDIO)
void do_count_pl_list()
{
 // byte count = 0;
  String filename;

  for (byte i = 1; i < 10; i++)
  {
    filename = F("/i_stat");
    filename += String(i);
    filename += F(".txt");

#if defined (pin_sd)
    if (SD.exists(filename))
    {
      Serial.print(F("ok SD "));
      Serial.println(filename);
      pl_list_max = i;
    }
    else
    {
      Serial.print(F("not SD "));
      Serial.println(filename);
      break;
    }

#else
    //    fl = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
    if (SPIFFS.exists(filename))
    {
      Serial.print(F("ok SPIFFS "));
      Serial.println(filename);
      pl_list_max = i;
    }
    else
    {
      Serial.print(F("not SPIFFS "));
      Serial.println(filename);
      break;
    }

#endif

  }
  Serial.print(F("pl_list_max "));
  Serial.println(pl_list_max);
}
#endif //32



//--------------------------------------------------------
//                      do_maxnum_file foto mp3
//--------------------------------------------------------
#if defined (pin_sd) //&& (defined (Sf)||defined (tft2_Sf))
//unsigned int do_maxnum_file(String & path )
unsigned int do_maxnum_file(const char * path )
{
  unsigned int max_num = 0;
  String s_fname = "";
  byte n1, n2;
  unsigned int num_tmp = 0;
  unsigned int min_num_tmp = 55555;

  String s_fname_num;


  File fname;
  File dir;
  dir = SD.open(path);

  if (!dir) {
    Serial.print(F("Failed to open dir")); Serial.println(path);
    return 0;
  }
  if (!dir.isDirectory()) {
    Serial.print(path); Serial.println(F(" Not a dir"));
    return 0;
  }

  fname = dir.openNextFile();
  while (fname) {

    if (num_tmp % 3 == 0)
      tft.fillCircle(x_onload, y_onload, 3, TFT_YELLOW);
    else tft.fillCircle(x_onload, y_onload, 3, TFT_BLACK);

    if (!fname.isDirectory()) {
      s_fname = fname.name();

      //     Serial.println(s_fname);

      n1 = s_fname.lastIndexOf("/");
      if (n1 == -1) n1 = 0; else n1++;
      n2 = s_fname.lastIndexOf('.');

      s_fname_num = s_fname.substring(n1, n2);

      num_tmp = s_fname_num.toInt();
      if (min_num_tmp > num_tmp)
        min_num_tmp = num_tmp;


      if (num_tmp > max_num)
      {
        max_num = num_tmp;
      }

    }

    fname = dir.openNextFile();
  }

  tft.fillCircle(x_onload, y_onload, 3, TFT_BLACK);
  fname.close();

#ifdef ESP32
  if (String(path) == "/mp3" && min_num_tmp > 0 && max_num > 0) min_num_mp3 = min_num_tmp;
#endif
  return max_num;

}
#endif  //pin_sd
