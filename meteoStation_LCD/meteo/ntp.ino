
void do_ntp()
{

  static uint32_t t_next = 0;
  uint32_t    t_repeat = 2000;//def error !sync

  if (millis() < t_next  && !sync_force) return ;
  else if (sync_t == true && sync_force == false && !ntp_err) t_repeat = t_ntp_rep_err * 1000UL; //уже было время синх.


#ifdef plotter
  do_plotter('T');//err
#endif



  while (udp.parsePacket() > 0) ; // discard any previously received packets

  if (ntp_num == 5)ntp_num = 1;
  if (ntp_num == 1)
    WiFi.hostByName(ntpServerName1, timeServerIP);
  if (ntp_num == 2)
    WiFi.hostByName(ntpServerName2, timeServerIP);
  if (ntp_num == 3)
    WiFi.hostByName(ntpServerName3, timeServerIP);
  if (ntp_num == 4)
    WiFi.hostByName(ntpServerName4, timeServerIP);
  Serial.print(F("ntp_num =")); Serial.println(ntp_num);

  //------------------------------------------
  sendNTPpacket(timeServerIP);

  uint32_t t_start = millis();
  int  size  = 0;
  do {
    if (sync_t&&millis() - t_start > ntp_timeout_break * 1000L) break;
    else if (millis() - t_start > 5000) break; //1 раз до loop
    size = udp.parsePacket();
    yield();
  } while (size != 48);

  t_next = millis() + t_repeat; //повтор предварительно
#ifdef debug
  Serial.print(F("----NTP wait Packet mSec:")); Serial.println(millis() - t_start);
#endif


  // TRUE ntp-----------------------------
  if (size >= NTP_PACKET_SIZE) {

    t_ntp_update = millis();
#ifdef debug
    Serial.print(F("t_ntp_update mSec:")); Serial.println(t_ntp_update);
#endif
    udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
    unsigned long secsSince1900;
    // convert four bytes starting at location 40 to a long integer
    secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
    secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
    secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
    secsSince1900 |= (unsigned long)packetBuffer[43];



#ifndef timechange_summer_winter
    if (TIMEZONE != 99) //on config.txt
    {
      ntp_time =  secsSince1900 - 2208988800UL + TIMEZONE * SECS_PER_HOUR;
      setTime(ntp_time);

#ifdef debug
      Serial.print("ntp_time TZ config.txt="); Serial.println(ntp_time);
#endif

    }
    else //not TZ in config.txt
    {
      Serial.print("ntp_time ERROR TIMEZONE not in  config.txt");

    }

#endif


#ifdef timechange_summer_winter
    if (TIMEZONE == 99) //not config.txt
    {
      TimeChangeRule *tcr;    // pointer to the time change rule, use to get TZ abbrev
      ntp_time =  secsSince1900 - 2208988800UL;
      time_t local = TZ.toLocal(ntp_time, &tcr);
      setTime(local);
      int32_t z = (local - ntp_time);
      z = z / 3600;
      TIMEZONE = z;
#ifdef debug
      Serial.print(F("ntp_time_S_W_=")); Serial.println(ntp_time);
      Serial.print(F("local_S_W_=")); Serial.println(local);
#endif
    }
    else //TZ in config.txt
    {

      ntp_time =  secsSince1900 - 2208988800UL + TIMEZONE * SECS_PER_HOUR;
      setTime(ntp_time);
#ifdef debug
      Serial.print(F("ntp_time_S_W_TZ_from_config.txt=")); Serial.println(ntp_time);
#endif
    }
#endif //sum win


#if !defined(no_tft)&& !defined(epd)

    if (!sync_t) do_fread_timer();//если был сброс таймера до перезагр проверка, один раз
#endif //#ifndef epd

    tm_ntp = millis();
    sync_t = true;
    do_readnow();
    sync_force = false;
    t_next = tm_ntp + t_period_ntp * 1000UL;
    ntp_err = false;
#ifndef openw //заход - восх расчитываю если не с openw
    do_sun();
#endif
    refresh_all = true;
#ifdef two_tft
    ref_tft2 = true;
#endif

#ifdef debug
    Serial.print(F("NTP OK ntp_num:")); Serial.println(ntp_num);
#endif
    return ;
    //-----OK----------------OK----------------OK---------------------------------------
  }

  //--------------------------------ERR
#ifdef debug
  Serial.println(F("NTP ERR"));
#endif
  ntp_num++;
  ntp_err = true;
#ifdef plotter
  do_plotter('t');//err
#endif
  return ;
}





//====================================================
//                  sendNTPpacket
//====================================================
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}




//====================================================
//                  now_correct
//====================================================
#ifdef  time_correct

time_t now_correct()
{

  if (!sync_t) return 0;

  time_t tm = ntp_time + (millis() - t_ntp_update) / 1000;
  time_t delta = 0;
  if (t > tm) delta = t - tm;
  if (tm > t)  delta = tm - t;
  //ERR ttttttttttttttttttttttttttt
  if (delta < 200) return t;
#ifdef debug
  Serial.print("delta=");
  Serial.println(delta);
  Serial.print("t now=");
  Serial.println(t);
  Serial.print("NTP now=");
  Serial.println(tm);
#endif
  setTime(tm);
  t = now();
  sync_force = true;
#ifdef debug

  Serial.print("ntp now=");
  Serial.println(tm);
  Serial.print("hour=");
  Serial.println(hour(tm));
  Serial.print("min=");
  Serial.println(minute(tm));
  Serial.print("day=");
  Serial.println(day(tm));
  Serial.print("month=");
  Serial.println(month(tm));
  Serial.print("year=");
  Serial.println(year(tm));



#ifdef debug
  log_str = F("ERR do now_correct delta=");
  log_str += delta;
  Serial.println(log_str);
#endif




#endif
  return tm ;
}
#endif //time_correct



//---------------------------------------------------------------------------
//                                          do_readnow
//---------------------------------------------------------------------------
void do_readnow()
{
  static uint32_t last = 0;
  t = now();
  if (millis() < last) return; //2sec
  //---------control timers



  //--------------------plotter
#ifdef plotter
  if (Plotter) do_plotter(' ');
#endif

#ifdef vcc_read
  static bool vcc_alert_draw = false;
  if (vcc_alert)
  {
    vcc_alert_draw = !vcc_alert_draw;
    if (vcc_alert_draw)
    {
      tft.fillTriangle(x1_vcc, y1_vcc, x2_vcc, y2_vcc, x3_vcc, y3_vcc, TFT_RED);
#ifdef    pin_vcc_low_alert
      digitalWrite(pin_vcc_low_alert, HIGH);
#endif
    }
    else
    {
      tft.fillTriangle(x1_vcc, y1_vcc, x2_vcc, y2_vcc, x3_vcc, y3_vcc, TFT_WHITE);
#ifdef    pin_vcc_low_alert
      digitalWrite(pin_vcc_low_alert, LOW);
#endif
    }
  }
  else
  {
    if (vcc_alert_draw) //ret status off alert
    {
      tft.fillTriangle(x1_vcc, y1_vcc, x2_vcc, y2_vcc, x3_vcc, y3_vcc, TFT_WHITE);
#ifdef    pin_vcc_low_alert
      digitalWrite(pin_vcc_low_alert, LOW);
#endif
      vcc_alert_draw = false;
    }

  }
#endif //vcc

  if (now_min == minute(t))
  {
    last = millis() + 2000;
    new_tick = !new_tick; //2sec on-off
    return;
  }
  //----------------

  new_minute_b = true; //end loop=false
  now_min = minute(t);
#ifdef plotter
  if (Plotter && now_min == 0) do_plotter('H');
  else if (Plotter && now_min % 10 == 0) do_plotter('h');
#endif

  if (last == 0 || now_min == 0)
  {
    now_hour = hour(t);
    now_day = day(t);
    now_month = month(t);
    // now_weekday = weekday(t);
    if (weekday(t) == 1) now_weekday = 7; //RU 1-7
    else now_weekday = weekday(t) - 1;
    now_year =  year(t);
    //-------saver
    if ((!play && !play_mp3 && !play_fm) || play_bg)
    {
      if ((now_hour == 0 && now_min == 0) || (now_min % t_ref_tft == 0))
      {
        //   tft.fillScreen(TFT_BLACK);
        refresh_all = true;
#ifdef two_tft
        ref_tft2 = true;
#endif
      }
    }
#ifdef debug
    Serial.print(F("now_hour:")); Serial.print(now_hour);
    Serial.print(F(" now_min:")); Serial.print(now_min);
    Serial.print(F(" day:")); Serial.print(now_day);
    Serial.print(F(" month:")); Serial.print(now_month);
    Serial.print(F(" weekday:")); Serial.print(now_weekday);
    Serial.print(F(" weekday name:"));
    if (now_weekday == 7) Serial.println(dayNames[0]);
    else
      Serial.println(dayNames[now_weekday]);
#endif
  }


  //now_time
  if (now_hour < 10)
    now_time = "0" + String(now_hour);
  else
    now_time = String(now_hour);
  now_time = now_time + ":";
  if (now_min < 10)
    now_time = now_time + "0" + String(now_min);
  else
    now_time = now_time + String(now_min);



  //now_date
  if (now_day < 10)
    now_date = "0" + String(now_day);
  else
    now_date = String(now_day);
  now_date = now_date + '.';
  if (now_month < 10)
    now_date = now_date + "0" + String(now_month);
  else
    now_date = now_date + String(now_month);



  //    Serial.print("now_time "); Serial.println(now_time);
  //    Serial.print("now_date "); Serial.println(now_date);

}

#ifdef plotter
//---------------------------------------------------------------------------
//                                          do_plotter
//---------------------------------------------------------------------------
void do_plotter(char ch)
{
  if (!Plotter) return;
  Serial.print(F("$"));
  //   Serial.print(ESP.getHeapSize());//возвращает полный размер кучи в памяти.
  //   Serial.print(F(" "));
  uint16_t fr = round(ESP.getFreeHeap() / 100); fr = (int)fr / 10;
  //1 free
  Serial.print(fr);//free
#ifdef ESP32
  Serial.print(F(" "));
  //2 alloc
  uint16_t alloc = round(ESP.getMaxAllocHeap() / 100); alloc = (int)alloc / 10;
  Serial.print(alloc);
#endif //32
  Serial.print(F(" "));
  //3 WIFI
  Serial.print(WiFi.status() * 10);
  Serial.print(F(" "));
  //4 Tin
  if (temp_kv != -100) Serial.print(temp_kv); else Serial.print(0);
  Serial.print(F(" "));
  //5 H
  if (h_kv != -100) Serial.print(h_kv); else Serial.print(0);
  Serial.print(F(" "));
  //6 Tout
  if (temp_u != -100) Serial.print(temp_u); else Serial.print(0);
  Serial.print(F(" "));

  //7 ntp T
  if (ch == 'T') Serial.print(50);
  else if (ch == 't')
    Serial.print(100);
  else Serial.print(0);
  Serial.print(F(" "));

  //8 now N
  if (ch == 'N') Serial.print(50);
  else if (ch == 'n') Serial.print(100);
  else Serial.print(0);
  Serial.print(F(" "));

  //9 Wh W
  if (ch == 'W') Serial.print(50);
  else if (ch == 'w') Serial.print(100);
  else Serial.print(0);
  Serial.print(F(" "));

  //10 play,play_mp3
  if (play) Serial.print(10);
  else if (play_mp3) Serial.print(5);
  else Serial.print(0);
  Serial.print(F(" "));

  //11 hour
  if (ch == 'H') Serial.print(8);
  else if (ch == 'h') Serial.print(4);
  else Serial.print(0);
  //Serial.print(F(" "));
  //------------
  Serial.println(F(""));
}
#endif //d
