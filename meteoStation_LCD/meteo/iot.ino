#if defined (iot_send)
//------------------------------------------------------------------------
//                          do_send_iot()
//------------------------------------------------------------------------
void do_send_iot()
{

  static uint32_t t_next = 0;
  byte f = 0;

  if (iot_period_send < 18)
  {
    //Serial.print(F("---------ERR iot_period_send <15"));
    return;
  }

  if (millis() < t_next) return;


#ifdef debug
  Serial.println(F("---IoT--send--"));
#endif

  //ttttttttttttttt
  if (iot_T1_fld_send != 0 && temp_kv != -100)
  {
#ifdef debug
    Serial.print(F("temp_kv="));
    Serial.println(temp_kv);
#endif
    ThingSpeak.setField(iot_T1_fld_send, temp_kv);
    f++;
  }
  //hhhhhhhhhhhhhhhhhhh
  if (iot_H_fld_send != 0 && h_kv != -100)
  {
#ifdef debug
    Serial.print(F("h_kv="));
    Serial.println(h_kv);
#endif
    ThingSpeak.setField(iot_H_fld_send, h_kv);
    f++;
  }
  //Tu-----------
  if (iot_T0_fld_send != 0 && temp_u != -100)
  {
#ifdef debug
    Serial.print(F("temp_u="));
    Serial.println(temp_u);
#endif
    ThingSpeak.setField(iot_T0_fld_send, temp_u);
    f++;
  }


  int P = -100;
  //ppppppppppppppppppppppppppppp  давление(pressure)
#ifdef openw
  P = davlenie0;
#endif

#ifdef bme280
  P = pr_kv;
#endif

  if (iot_P_fld_send != 0 && (P != -100))
  {
#ifdef debug
    Serial.print(F("P="));
    Serial.println(P);
#endif
    ThingSpeak.setField(iot_P_fld_send, P);
    f++;
  }


#ifdef openw
  //wwwwwwwwwwwwwwwwwwwwwwwwww скорость ветра (wind speed)
  if (iot_W_fld_send != 0 && v_ch[0] != -100)
  {
#ifdef debug
    Serial.print(F("Wind="));
    Serial.println(v_ch[0]);
#endif
    ThingSpeak.setField(iot_W_fld_send, v_ch[0]);
    f++;
  }

  //rrrrrrrrrrrrrrrrrrr  byte =6; //дождь мм(rain)

  if (iot_R_fld_send != 0 && rain_ch[0] != 0) //есть ли данные, пров. по davlenie0.
  {
#ifdef debug
    Serial.print(F("Rain="));
    Serial.println(rain_ch[0]);
#endif
    ThingSpeak.setField(iot_R_fld_send, rain_ch[0]);
    f++;
  }
#endif //openw


#ifdef debug
  Serial.print(F("id="));
  Serial.print(iot_id);
  Serial.print(F("   iot_key="));
  Serial.println(iot_key);
#endif

  if (f == 0)
  {
    Serial.println(F("IoT No fields, exit"));
    t_next = millis() + 60 * 1000L;
    return;
  }
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(iot_id, iot_key.c_str());
  if (x == 200) {

#ifdef debug
    Serial.println(F("---IoT update successful."));
#endif
    t_next = millis() + iot_period_send * 1000L;


  }
  else {
    Serial.print(F("---- IoT send ERR code:" ));
    Serial.println(x);
    t_next = millis() + 60 * 1000L;
  }
}
#endif


//------------------------------------------------------------------------
//                          do_read_iot()
//------------------------------------------------------------------------
#if defined (iot_read)
void do_read_iot()
{

  static uint32_t t_next = 0;
  static time_t t_last = 0;
  if (!sync_t)  return;
  if (t_last == 0)
  {
    t_last = now() - millis() / 1000L;
#ifdef debug
    Serial.println(F("t_last start: ")); Serial.println(t_last);
#endif
  }
  if (millis() < t_next) return;

#ifdef debug
  Serial.println(F("-----IoT--read"));
#endif
  /*
    struct tm tm = {0};
    strptime(str.c_str(), "%Y-%m-%d T %H:%M:%S Z", &tm);
    Serial.print(F("---------!TM.year")); Serial.println(tm.tm_year);
    Serial.print(F("---------!TM.tm_mon")); Serial.println(tm.tm_mon + 1);
    Serial.print(F("---------!TM.tm_mday")); Serial.println(tm.tm_mday);
    Serial.print(F("---------!TM.tm_hour")); Serial.println(tm.tm_hour);
    Serial.print(F("---------!TM.tm_min")); Serial.println(tm.tm_min);
    Serial.print(F("---------!TM.tm_sec")); Serial.println(tm.tm_sec);

    time_t t_ = mktime(&tm) + TIMEZONE * SECS_PER_HOUR;
    Serial.print(F("---------!time_t \t")); Serial.println(t_);
    Serial.print(F("---------!time_t year \t ")); Serial.println(year(t_));
    Serial.print(F("---------!time_t month \t")); Serial.println(month(t_));
    Serial.print(F("---------!time_t day \t")); Serial.println(day(t_));
    Serial.print(F("---------!time_t hour \t")); Serial.println(hour(t_));
    Serial.print(F("---------!time_t min \t")); Serial.println(minute(t_));
    Serial.print(F("---------!time_t sec \t")); Serial.println(second(t_));

    tmElements_t tm2;
    t_ = makeTime(tm2) + TIMEZONE * SECS_PER_HOUR;
    Serial.print(F("---------!time_t ARDUINO\t")); Serial.println(t_);
    str = ThingSpeak.readRaw(iot_id_read, "feeds/last.json", iot_key_read.c_str());
    Serial.print(F("-------------------!readRaw")); Serial.println(str);
  */
  int code = 0;

  code = ThingSpeak.readMultipleFields(iot_id_read, iot_key_read.c_str());
  if (code == 200)
  {
    String str = "";
    struct tm tm = {0};
    str = ThingSpeak.getCreatedAt();
    if (str.length() > 10)
    {
      strptime(str.c_str(), "%Y-%m-%d T %H:%M:%S Z", &tm);
      time_t t_ = mktime(&tm);
      t_ += TIMEZONE * SECS_PER_HOUR;
#ifdef debug
      Serial.print(F("At = "));  Serial.println(str);
      //    Serial.print(F("At tm:")); Serial.print(t_);
      //    Serial.print(F("\t t_last:")); Serial.println(t_last);
#endif
      if (t_ <= t_last) //old row
      {
        t_next = millis() + iot_period_read * 1000L;
        return;
      }
      t_last = t_;
      /*
          int f1 = ThingSpeak.getFieldAsInt(1); // Field 1
          int f2 = ThingSpeak.getFieldAsInt(2); // Field 2
          int f3 = ThingSpeak.getFieldAsInt(3); // Field 3
          int f4 = ThingSpeak.getFieldAsInt(4); // Field 4
          int f5 = ThingSpeak.getFieldAsInt(5); // Field 5
          int f6 = ThingSpeak.getFieldAsInt(6); // Field 6
          int f7 = ThingSpeak.getFieldAsInt(7); // Field 7
          int f8 = ThingSpeak.getFieldAsInt(8); // Field 8
          //  String statusMessage = ThingSpeak.getStatus(); // Status message
          //  String createdAt = ThingSpeak.getCreatedAt(); // Created-at timestamp
          Serial.print(F("-------!F1 = ")); Serial.println(f1);
          Serial.print(F("-------!F2 = ")); Serial.println(f2);
          Serial.print(F("-------!F3 = ")); Serial.println(f3);
          Serial.print(F("-------!F4 = ")); Serial.println(f4);
          Serial.print(F("-------!F5 = ")); Serial.println(f5);
          Serial.print(F("-------!F6 = ")); Serial.println(f6);
          Serial.print(F("-------!F7 = ")); Serial.println(f7);
          Serial.print(F("-------!F8 = ")); Serial.println(f8);
      */
      //tttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt
      if (iot_T1_fld_read != 0)
      {
        String fld_s = ThingSpeak.getFieldAsString(iot_T1_fld_read);
        int fld_i = -99;
        if (fld_s.length() > 0) fld_i = fld_s.toInt();
        if (fld_i > -60 && fld_i < 60)
        {
          if (fld_i != temp_kv)
          {
            ref_kv_th = true;
            temp_kv = fld_i;
          }
          t_read_tk = millis();
        }
#ifdef debug
        Serial.print(iot_T1_fld_read);
        Serial.print(F(" T1 str/int\t")); Serial.print(fld_s); Serial.print("\t"); Serial.println(fld_i);
#endif

      }


      //hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
      if (iot_H_fld_read != 0)
      {
        String fld_s = ThingSpeak.getFieldAsString(iot_H_fld_read);
        int fld_i = -99;
        if (fld_s.length() > 0) fld_i = fld_s.toInt();
        if (fld_i > 0 && fld_i < 101)
        {
          if (fld_i != h_kv)
          {
            ref_kv_th = true;
            h_kv = fld_i;
          }
          t_read_tk = millis();
        }
#ifdef debug
        Serial.print(iot_H_fld_read);
        Serial.print(F(" H str/int\t")); Serial.print(fld_s); Serial.print("\t"); Serial.println(fld_i);
#endif

      }

      //Tu----TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
      if (iot_T0_fld_read != 0)
      {
        String fld_s = ThingSpeak.getFieldAsString(iot_T0_fld_read);
        int fld_i = -99;
        if (fld_s.length() > 0) fld_i = fld_s.toInt();
        if (fld_i > - 60 && fld_i < 60)
        {
          if (fld_i != temp_u)
          {
            ref_temp_u = true;
            temp_u = fld_i;
          }
          period_ulica = millis();
        }
#ifdef debug
        Serial.print(iot_T0_fld_read);
        Serial.print(F(" T0 str/int\t")); Serial.print(fld_s); Serial.print("\t"); Serial.println(fld_i);
#endif
      }

      //ppppppppppppppppppppppppppppp  давление(pressure)
      if (iot_P_fld_read != 0)
      {
        String fld_s = ThingSpeak.getFieldAsString(iot_P_fld_read);
        int fld_i = -99;
        if (fld_s.length() > 0) fld_i = fld_s.toInt();
        if (fld_i > 600 && fld_i < 10000)
        {
          if (fld_i != pr_kv)
          {
            pr_kv = fld_i;
          }

        }
#ifdef debug
        Serial.print(iot_P_fld_read);
        Serial.print(F(" P str/int\t")); Serial.print(fld_s); Serial.print("\t"); Serial.println(fld_i);
#endif
      }

      //VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
#ifdef vcc_read
      if (iot_VCC_fld_read != 0)
      {
        String fld_s = ThingSpeak.getFieldAsString(iot_VCC_fld_read);
        int fld_i = -99;
        if (fld_s.length() > 0) fld_i = fld_s.toInt();
        if (fld_i > 0 && fld_i < vcc_full * 2) //control err
        {
          if (fld_i != vcc)
          {
            vcc = fld_i;
            t_next_read_vcc = millis() + t_vcc_ref * 1000L;
            draw_vcc();
          }
        }
#ifdef debug
        Serial.print(iot_VCC_fld_read);
        Serial.print(F(" VCC str/int\t")); Serial.print(fld_s); Serial.print("\t"); Serial.println(fld_i);
#endif
      }
#endif //vcc
    }
  }//==200


  if (code == 200)
    t_next = millis() + iot_period_read * 1000L;
  else
  {
    t_next = millis() + 60 * 1000L;
    Serial.print(F("ERR read iot:")); Serial.println(code);
  }

}

#endif
