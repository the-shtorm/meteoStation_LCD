#if defined (S1) && defined(tft_320_240)&& defined (openw)
//time///////////////////////////////////////tttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt
void S1_time()
{
  static bool fill_sec = false;

  if (new_tick && !fill_sec)
  {
    tft.fillCircle(135, 105, 4, color_sec);
    fill_sec = true;
    S1_alert(false);
  }
  else if (!new_tick && fill_sec)
  {
    S1_alert(true);
    //sec
    tft.fillCircle(135, 105, 4, TFT_BLACK);
    fill_sec = false;
  }


  if (ref_wh || refresh_all) //обновилась погода
  {
    S1_show_3ch();
    S1_show_5d();
    S1_graf_davlenie();
    S1_show_now();
  }
  else if (ref_temp_u || ref_wh_now) //есть данные с улицы
  {
    S1_show_now();
  }
  else if (new_minute_b ) //draw color
  {
    draw_line_Tu(165, 113, 127 );
#ifdef def_th
    S_show_th_room(270, 72); //ref
#endif
  }

#ifdef def_th
  if (!refresh_all) S1_th_moon(270, 72);
#endif


  //если мин не изм  и не надо все обновить выходим
  if (!new_minute_b && refresh_all == false) {
    return;
  }

  //-----------------------------------------------------------------


  //hour///hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
  String st, fileName ;
  if (refresh_all || now_min == 0)
  {

    if (now_hour < 10)
    {
      fileName = F("n0_112");
      drawJpeg(fileName, 0, 0); //220

      fileName = "n" + String(now_hour) + F("_112");
      drawJpeg(fileName, 64, 0); //220
    }
    else
    {
      st = String(now_hour);
      fileName = "n" + st.substring(0, 1) + F("_112");
      drawJpeg(fileName, 0, 0); //220

      fileName = "n" + st.substring(1) + F("_112");
      drawJpeg(fileName, 64, 0); //220

    }

  }

  //mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm

  if (now_min < 10)
  {
    fileName = F("n0_112");
    drawJpeg(fileName, 140, 0); //220
    fileName = "n" + String(now_min) + F("_112");
    drawJpeg(fileName, 204, 0); //220

  }
  else
  {
    st = String(now_min);
    fileName = "n" + st.substring(0, 1) + F("_112");
    drawJpeg(fileName, 140, 0); //220
    fileName = "n" + st.substring(1) + F("_112");
    drawJpeg(fileName, 204, 0);

  }


  //дата----data/ddddddddddddddddddddddddddddddddddddddddddddddd
  if (!refresh_all && year(t) > 2015)
    return;



  /////day
  tft.fillRect(270, 0, 50, 112, TFT_BLACK);

  //wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww
  draw_week_day(270, 47);

  tft.setTextColor(c_date);
  tft.setFreeFont(v18b);

  String str = String(str_num(now_day));
  tft.setCursor(270, 30);
  tft.print(str);
  tft.drawFastHLine(270, 33, 50, TFT_CYAN) ;
  str = String(str_num(now_month));
  tft.setFreeFont(v9);
  tft.setTextColor(c_month);
  tft.setCursor(284, 47);
  tft.print(str);


#ifndef def_th
  fileName = do_phasemoon() + F("_32");
  drawJpeg(fileName, 276, 75);
#else
  S1_th_moon(270, 72);
#endif

  tft.drawFastHLine(0, 113, 320, TFT_CYAN) ;

}




#ifdef def_th
//--------------------------------------------------------------
//        S1_th_moon()
//--------------------------------------------------------------
void S1_th_moon(int x, int y)
{

  static uint32_t t_next_moon = 0;
  static uint32_t t_next_th = 0;


  //init
  // if (refresh_all || ref_temp_u || ref_wh_now || ref_wh || ref_kv_th)
  if (refresh_all || ref_kv_th)
  {
    t_next_moon = 0;
    t_next_th = 0;
    //   Serial.println("S1_th_moon");
    //  return;
  }

  //--------------------------------
  if (millis() > t_next_th || refresh_all == true )
  {
    S1_clear_moon(x, y);
    S_show_th_room(x, y);
    t_next_moon = millis() + S_t_bme * 1000L;
    t_next_th = t_next_moon + S_t_moon * 1000L;
    //   Serial.println("S1_th_moon TH");
    return;
  }

  //MOON mmmmmmmmmmmmmmmm
  if (millis() > t_next_moon )
  {
    S1_clear_moon(x, y);
    String fileName = do_phasemoon() + F("_32");
    drawJpeg(fileName, x + 6, y + 3);
    t_next_th = millis() + S_t_moon * 1000L;
    t_next_moon = t_next_th + S_t_bme * 1000L;
  }

}


//-------------------------------------------------------------
//                  S1_clear_moon
///-------------------------------------------------------------

void S1_clear_moon(int x, int y)
{


  for (byte i = 0; i < 44; i++) {
    tft.drawFastVLine(x + i, y, 38, TFT_CYAN) ;
    if (i > 0)
      tft.drawFastVLine(x + i - 1, y, 38, TFT_BLACK) ;
    delay(15);
  }
  tft.fillRect(x, y, 44, 38, TFT_BLACK);
}
#endif //th

//===============================================================
//                          S1_alert
//===============================================================
void S1_alert(bool sh)
{

  if (sh == true)
  {

    //alert  ttttttttttttttttttttttttttttttttttttttttttt
    if (temperatura_ch[0] != -100)
    {

      if (temperatura_ch[0] >= t_max || temperatura_ch[1] >= t_max || temp_ch_n >= t_max) //t alert
        tft.fillRect(18, 117, 50, 3, TFT_YELLOW);

      if (temperatura_ch[0] <= t_min || temperatura_ch[1] <= t_min || temperatura_ch[2] <= t_min || temp_ch_n <= t_min) //t alert
        tft.fillRect(18, 117, 50, 3, TFT_CYAN);


    }

    //vvvvvvvvvvvvvvvvvvvvvvvvvvv
    if (v_ch[0] != -100)
    {
      if (v_ch[0] >= v_max || v_ch[1] >= v_max) //1,2 период по 3ч
        tft.fillRect(151, 139, 10, 3, TFT_CYAN);

    }

    //ppppppppppppppppppppppppppppppppppp
    int pp;
    if (pr_kv != -100) pp = pr_kv;
    else
      pp = davlenie0;

    if (pp != -100)
    {
      if (pp >= p_max || pp <= p_min)
        tft.fillRect(111, 157, 7, 5, TFT_GREEN);
    }

    //////uv
    if (uv >= uv_max)
      S_show_uv( false, 0, 115);


    //rainrrrrrrrrrrrrrrrrrrrrrrrrr
    if (rain_ch[0] >= rain_m * 2)
      do_draw_rain(74, 123, rain_ch[0], TFT_CYAN);

  }
  else //------------------------------------------------------
  {

    //////uv///////////////
    if (uv >= uv_max)
      S_show_uv( true, 0, 115);
    //ppppppppppppppppppppppppppppppppp

    tft.fillRect(111, 157, 7, 5, TFT_BLACK);

    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

    tft.fillRect(151, 139, 10, 3, TFT_BLACK);


    //tttttttttttt
    tft.fillRect(18, 117, 50, 3, TFT_BLACK);


    //rainrrrrrrrrrrrrrrrrrrrrrrrrr
    if (rain_ch[0] >= rain_m * 2)
      do_draw_rain(74, 123, rain_ch[0], TFT_BLACK);

  }


}



#endif //openw
