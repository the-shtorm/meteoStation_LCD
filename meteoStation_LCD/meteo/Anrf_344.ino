#if defined (nrf_send) ||defined (pin_r433_tx)
//-----------------------------------------------------------------------------
//               do_send_data
//-----------------------------------------------------------------------------
void do_send_data()
{

  //#define SEND_moon  //time


  static uint32_t t_next = 0;//ntp send
  static String str_send = "";
  static bool sending_all = false;
  static byte count = 1;
  static uint32_t t_pause = 0;
  uint32_t mls = millis();
  if (mls < t_next && sending_all == true) return;
  if (mls < t_pause) return;
  t_pause = mls + 1000; //1sec
  sending_all = false;

  //ttttttttttttttttttttttttttttttttttttttttttttt
#ifdef SEND_tm
  if (sync_t&&count == 1)
  {
    str_send = "-tm:" + String(t);
#ifdef pin_r433_tx
    do_send_r433(str_send);
#endif
#ifdef nrf_send
    do_send_nrf(str_send);
#endif
    count = 2;
    return;
  }
#else
  count = 2;
#endif //time


  //moon
#ifdef SEND_moon
  if (sync_t&&count == 2)
  {
    String str = do_phasemoon();
    str_send = "-Mn:" + str;
#ifdef pin_r433_tx
    do_send_r433(str_send);
#endif
#ifdef nrf_send
    do_send_nrf(str_send);
#endif
    count = 3;
    return;
  }
#else
  count = 3;

#endif //moon

  //-------------VVVVVVVVVZZZZZZZZZZZZ

#ifdef SEND_tm_sun
  if (t_sun_v_hh != 99 && t_sun_z_hh != 99 && count == 3) //init val
  {
    str_send = "-UPh:" + String(t_sun_v_hh) + ";UPm" + String(t_sun_v_min - (60 * t_sun_v_hh));
    // str_send = "-Tv:" + String(t_);
#ifdef pin_r433_tx
    do_send_r433(str_send);
#endif
#ifdef nrf_send
    do_send_nrf(str_send);
#endif
    str_send = "-DNh:" + String(t_sun_z_hh) + ";DNm" + String(t_sun_z_min - (60 * t_sun_z_hh));
#ifdef pin_r433_tx
    do_send_r433(str_send);
#endif
#ifdef nrf_send
    do_send_nrf(str_send);
#endif
    count = 4;
    return;
  }
#else
  count = 4;

#endif //SEND_tm_sun



  //-------TTTTTTT HHHHHHHHHHHHHH PPPPPPPPPPP
#ifdef SEND_sensor_data
  if (count == 4)
  {
    str_send = "";
    if (temp_kv != -100) str_send = "T1:" + String(temp_kv);
    if (temp_u != -100)
    {
      if (str_send != "") str_send = str_send + ";";
      str_send = str_send + "T0:" + String(temp_u);
    }

    if (h_kv != -100)
    {
      if (str_send != "") str_send = str_send + ";";
      str_send = str_send + "H:" + String(h_kv);
    }
    if (pr_kv != -100)
    {
      if (str_send != "") str_send = str_send + ";";
      str_send = str_send + "P:" + String(pr_kv);
    }

    if (str_send != "")
    {
      str_send = "-" + str_send;
#ifdef pin_r433_tx
      do_send_r433(str_send);
#endif
#ifdef nrf_send
      do_send_nrf(str_send);
#endif

    }
    count = 5;
    return;
  }
#else
  count = 5;

#endif //SEND_sensor_data



  //----------wwwwwwwwwwwwwwwww0000000000000
  //погода
  //#define SEND_H3 //now+3
#ifdef SEND_W0 //now
  if (count == 5)
  {
    if (icon_ch[0] != "")
    {
      str_send = "";
      if (temperatura_ch[0] != -100)
        str_send = "T:" + String(temperatura_ch[0]);
      //ночь
      if (  temp_ch_n != -100)
      {
        if (str_send != "")   str_send = str_send + ";";
        str_send = str_send + "t:" + String(temp_ch_n);
      }

      //icon

      if (str_send != "")   str_send = str_send + ";";
      str_send = str_send + "I:" + String(icon_ch[0]);
      if (str_send != "")
      {
        str_send = "-W0" + str_send;
#ifdef pin_r433_tx
        do_send_r433(str_send);
#endif
#ifdef nrf_send
        do_send_nrf(str_send);
#endif
      }
      //------------------
      str_send = "";
      //Pres
      if (davlenie0 != -100)
      {
        if (str_send != "")   str_send = str_send + ";";
        str_send = str_send + "P:" + String(davlenie0);
      }

      //Vm/s
      if (str_send != "")   str_send = str_send + ";";
      str_send = str_send + "V:" + String(v_ch[0]);

      if (rain_ch[0] != 0)
      {
        if (str_send != "")   str_send = str_send + ";";
        str_send = str_send + "R:" + String(rain_ch[0]);
      }
      if ( v_gr_ch[0] != 999)
      {
        if (str_send != "")   str_send = str_send + ";";
        str_send = str_send + "g:" + String(v_gr_ch[0] );
      }

      if (str_send != "")
      {
        str_send = "-W0" + str_send;
#ifdef pin_r433_tx
        do_send_r433(str_send);
#endif
#ifdef nrf_send
        do_send_nrf(str_send);
#endif
      }
    }
    count = 6;
    return;
  }
#else
  count = 6;

#endif//SEND_W0



  //----------wwwwwwwwwwwwwwwww+11111111111111
  //погода
#ifdef SEND_W1 //now
  if (count == 6)
  {
    if (icon7[1] != "")
    {
      str_send = "";
      if (temperatura7[1] != -100)
        str_send = "T:" + String(temperatura7[1]);
      //ночь
      if (  temperatura7n[1] != -100)
      {
        if (str_send != "")   str_send = str_send + ";";
        str_send = str_send + "t:" + String(temperatura7n[1]);
      }

      //icon
      if (str_send != "")   str_send = str_send + ";";
      str_send = str_send + "I:" + String(icon7[1]);

      if (str_send != "")
      {
        str_send = "-W1" + str_send;
#ifdef pin_r433_tx
        do_send_r433(str_send);
#endif
#ifdef nrf_send
        do_send_nrf(str_send);
#endif
      }
      //------------------
      str_send = "";
      //Pres
      if (davlenie7[1] != -100)
      {
        if (str_send != "")   str_send = str_send + ";";
        str_send = str_send + "P:" + String(davlenie7[1]);
      }

      //Vm/s
      if (str_send != "")   str_send = str_send + ";";
      str_send = str_send + "V:" + String(veter7[1]);

      if (rain7[1] != 0)
      {
        if (str_send != "")   str_send = str_send + ";";
        str_send = str_send + "R:" + String(rain7[1]);
      }
      if ( v_gr_7[0] != 999)
      {
        if (str_send != "")   str_send = str_send + ";";
        str_send = str_send + "g:" + String(v_gr_7[1] );
      }

      if (str_send != "")
      {
        str_send = "-W1" + str_send;
#ifdef pin_r433_tx
        do_send_r433(str_send);
#endif
#ifdef nrf_send
        do_send_nrf(str_send);
#endif
      }
    }
    count = 7;
    return;
  }
#else
  count = 7;

#endif//SEND_W1


  //----------wwwwwwwwwwwwwwwww+22222222222222
  //погода
#ifdef SEND_W1 //now
  if (count == 7)
  {
    if (icon7[2] != "")
    {
      str_send = "";
      if (temperatura7[2] != -100)
        str_send = "T:" + String(temperatura7[2]);
      //ночь
      if (  temperatura7n[2] != -100)
      {
        if (str_send != "")   str_send = str_send + ";";
        str_send = str_send + "t:" + String(temperatura7n[2]);
      }

      //icon

      if (str_send != "")   str_send = str_send + ";";
      str_send = str_send + "I:" + String(icon7[2]);

      if (str_send != "")
      {
        str_send = "-W2" + str_send;
#ifdef pin_r433_tx
        do_send_r433(str_send);
#endif
#ifdef nrf_send
        do_send_nrf(str_send);
#endif
      }
      //------------------
      str_send = "";
      //Pres
      if (davlenie7[2] != -100)
      {
        if (str_send != "")   str_send = str_send + ";";
        str_send = str_send + "P:" + String(davlenie7[2]);
      }

      //Vm/s
      if (str_send != "")   str_send = str_send + ";";
      str_send = str_send + "V:" + String(veter7[2]);

      if (rain7[2] != 0)
      {
        if (str_send != "")   str_send = str_send + ";";
        str_send = str_send + "R:" + String(rain7[2]);
      }
      if ( v_gr_7[0] != 999)
      {
        if (str_send != "")   str_send = str_send + ";";
        str_send = str_send + "g:" + String(v_gr_7[2] );
      }

      if (str_send != "")
      {
        str_send = "-W2" + str_send;
#ifdef pin_r433_tx
        do_send_r433(str_send);
#endif
#ifdef nrf_send
        do_send_nrf(str_send);
#endif
      }
    }
    count = 8;
    return;
  }
#else
  count = 8;

#endif//SEND_W2


  //----------hhhhhhhhhhhhhhhhhh+3333333333333333333
  //погода
  //now+3
#ifdef SEND_H3 //now
  if (count == 8)
  {
    if (icon_ch[1] != "")
    {
      str_send = "";
      if (temperatura_ch[1] != -100)
        str_send = "T:" + String(temperatura_ch[1]);
      //time+3
      if (day_ch[1] != -100)
      {
        if (str_send != "")   str_send = str_send + ";";
        str_send = str_send + "tm:" + String(day_ch[1]);
      }

      //icon

      if (str_send != "")   str_send = str_send + ";";
      str_send = str_send + "I:" + String(icon_ch[1]);

      if (str_send != "")
      {
        str_send = "-H3" + str_send;
#ifdef pin_r433_tx
        do_send_r433(str_send);
#endif
#ifdef nrf_send
        do_send_nrf(str_send);
#endif
      }
      //Vm/s
      str_send = "";
      str_send = "V:" + String(v_ch[1]);

      if (rain_ch[1] != 0)
      {
        if (str_send != "")   str_send = str_send + ";";
        str_send = str_send + "R:" + String(rain_ch[1]);
      }
      if ( v_gr_ch[1] != 999)
      {
        if (str_send != "")   str_send = str_send + ";";
        str_send = str_send + "g:" + String(v_gr_ch[1] );
      }

      if (str_send != "")
      {
        str_send = "-H3" + str_send;
#ifdef pin_r433_tx
        do_send_r433(str_send);
#endif
#ifdef nrf_send
        do_send_nrf(str_send);
#endif
      }
    }
    count = 9;
    return;
  }
#else
  count = 9;

#endif//SEND_H3



  //----------hhhhhhhhhhhhhhhhhh+666666666666666666666666666
  //погода
  //now+3
#ifdef SEND_H6 //now
  if (count == 9)
  {
    if (icon_ch[2] != "")
    {
      str_send = "";
      if (temperatura_ch[2] != -100)
        str_send = "T:" + String(temperatura_ch[2]);
      //time+3
      if (day_ch[2] != -100)
      {
        if (str_send != "")   str_send = str_send + ";";
        str_send = str_send + "tm:" + String(day_ch[2]);
      }

      //icon

      if (str_send != "")   str_send = str_send + ";";
      str_send = str_send + "I:" + String(icon_ch[2]);

      if (str_send != "")
      {
        str_send = "-H6" + str_send;
#ifdef pin_r433_tx
        do_send_r433(str_send);
#endif
#ifdef nrf_send
        do_send_nrf(str_send);
#endif
      }
      str_send = "";
      //Vm/s
      str_send = "V:" + String(v_ch[2]);

      if (rain_ch[2] != 0)
      {
        if (str_send != "")   str_send = str_send + ";";
        str_send = str_send + "R:" + String(rain_ch[2]);
      }
      if ( v_gr_ch[2] != 999)
      {
        if (str_send != "")   str_send = str_send + ";";
        str_send = str_send + "g:" + String(v_gr_ch[2] );
      }

      if (str_send != "")
      {
        str_send = "-H6" + str_send;
#ifdef pin_r433_tx
        do_send_r433(str_send);
#endif
#ifdef nrf_send
        do_send_nrf(str_send);
#endif
      }
    }
    count = 10;
    return;
  }
#else
  count = 10;
#endif//SEND_H6




  //------------
  str_send = "";
  sending_all = true;
  t_next = millis() + SEND_time_period * 1000UL;
  count = 1;
#ifdef debug
  Serial.println(F("------------SEND_END"));
#endif

}
#endif //nrf || 433

#if defined (nrf_send)
//=======================================================
//                do_send_nrf
//=======================================================
void do_send_nrf(String & str_send)
{

#if defined(nrf_read)
  if (nrf_sending == false)
  {
    nrf_sending = true;
    radio.stopListening();  //не слушаем радиоэфир, мы передатчик
    delay(100);
#if nrf_pipe_send==1
    radio.openWritingPipe ( 0xAABBCCDD11LL);
#elif nrf_pipe_send==2
    radio.openWritingPipe ( 0xAABBCCDD22LL);
#elif nrf_pipe_send==3
    radio.openWritingPipe ( 0xAABBCCDD33LL);                 // Открываем 1 трубу с идентификатором 1 передатчика 0xAABBCCDD11, для приема данных
#elif nrf_pipe_send==4
    radio.openWritingPipe ( 0xAABBCCDD44LL);
#elif nrf_pipe_send==5
    radio.openWritingPipe ( 0xAABBCCDD55LL);                 // Открываем 1 трубу с идентификатором 1 передатчика 0xAABBCCDD11, для приема данных

#else
#error "ERR set nrf_pipe "
#endif
  }
#endif

  byte str_len = str_send.length() + 1;
  char buf[str_len];
  str_send.toCharArray(buf, str_len);
  radio.write(&buf, sizeof(buf));
  // delay(50);
#ifdef debug
  Serial.print(F("---NRF_S: ")); Serial.println(buf);
#endif
}
#endif //nrf_send



//=======================================================
//                do_read_nrf
//=======================================================
#if defined (nrf_read)
void do_read_nrf()
{
  //#ifdef nrf_send //reopen pipe or on boot
  if (nrf_sending == true)
  {
    radio.openReadingPipe (1, 0xAABBCCDD11LL);
    radio.openReadingPipe (2, 0xAABBCCDD22LL);
    radio.openReadingPipe (3, 0xAABBCCDD33LL);
    radio.openReadingPipe (4, 0xAABBCCDD44LL);
    radio.openReadingPipe (5, 0xAABBCCDD55LL);
    radio.startListening();  // put radio in RX mode
    nrf_sending = false;
  }
  //#endif


  static uint32_t t_next = 0;
  byte pipeNo;
  if ( radio.available(&pipeNo)) // Non-blocking
  {

    char buff[32];
    uint8_t len = sizeof(buff);

    radio.read( &buff, sizeof(buff) );
#ifdef debug
    Serial.print(F("NRF_R str: ")); Serial.println(buff);
    Serial.print(F("Pipe: "));    Serial.println(pipeNo);
#endif
    log_str = (const char*) buff;
    // do_parse_str();
    do_short_cmd(log_str);
  }
  t_next = millis() + 500;
}
#endif //nrf_read
