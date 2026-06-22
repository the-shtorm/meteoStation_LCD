#if defined (S1) && defined(tft_480_320)&& defined (openw)


#if defined (two_tft) && defined (tft2_S1)
#define tft tft2
#endif



void S1_time()
{
  static bool fill_sec = false;


#if defined (two_tft) && defined (tft2_S1)

  if (fn_cmd_tmp == "prev" || fn_cmd_tmp == "next") //&& (play || play_mp3 || play_fm))
  {

    Serial.print("fn_cmd_tmp  "); Serial.println(fn_cmd_tmp);
    return;
  }
#endif

  if (new_tick && !fill_sec)
  {
    if (S1 != 0)
      tft.fillCircle(135, 105, 4, color_sec);
    fill_sec = true;
    S1_alert(false);
  }
  else if (!new_tick && fill_sec)
  {
    S1_alert(true);
    //sec
    if (S1 != 0)
      tft.fillCircle(135, 105, 4, TFT_BLACK);
    fill_sec = false;
  }



  if (refresh_all || ref_wh)
  {

    if (S1 == 0) draw_Calendar(5, 0, 260, 0);
    S1_show_now();
    S1_wh_ch();
    S1_show_7day();
    S1_graf_t();
    S1_graf_davlenie();
  } else if (ref_temp_u || ref_wh_now) //есть данные с улицы
  {
    S1_show_now();
  }
  else if (new_minute_b)
  {
    draw_line_Tu(285, 0, 159 );
#ifdef def_th
    S_show_th_room(428, 97); //ref
#endif
  }

#ifdef def_th
  S1_th_moon(428, 97);
#endif


  if (S1 == 0) return;

#ifdef tft2_S1
  return;
#endif


#ifndef tft2_S1

  if (!new_minute_b && !refresh_all) {
    return;
  }
  String str1 = "", fileName = "";
  //-----------------------------------------------------------------
  if (now_min == 0 || refresh_all)
  {
    //hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh


    if (now_hour < 10)
    {
      fileName = F("n0_112");
      drawJpeg(fileName, 0, 0); //220
      fileName = "n" + String(now_hour) + F("_112");
      drawJpeg(fileName, 64, 0); //220
    }
    else
    {
      str1 = String(now_hour);
      fileName = "n" + str1.substring(0, 1) + F("_112");
      drawJpeg(fileName, 0, 0); //220
      fileName = "n" + str1.substring(1) + F("_112");
      drawJpeg(fileName, 64, 0); //220

    }
  }




  //////////////////min///////////////////////////////

  if (now_min < 10)
  {
    fileName = F("n0_112");

    drawJpeg(fileName, 140, 0); //220
    if (!play_bg) S1_clear_min();
    fileName = "n" + String(now_min) + F("_112");

    drawJpeg(fileName, 204, 0); //220

  }
  else
  {
    str1 = String(now_min);
    fileName = "n" + str1.substring(0, 1) + F("_112");

    drawJpeg(fileName, 140, 0); //220
    fileName = "n" + str1.substring(1) + "_112";
    if (!play_bg) S1_clear_min();
    drawJpeg(fileName, 204, 0);
  }


  //дата DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
  if (!refresh_all)
    return;

  /////day
  tft.fillRect(0, 115, 280, 44, TFT_BLACK);
  tft.drawFastHLine(0, 115, 280, TFT_WHITE) ;
  tft.setTextColor(c_date);
  tft.setFreeFont(v24);

  str1 = String(str_num(now_day));

  tft.setCursor(0, 155);
  tft.print(str1);
  tft.setFreeFont(v18b);
  tft.setTextColor(c_month);
  str1 = '.' + String(str_num(now_month));
  tft.print(str1);

  //день недели-------------------------

  draw_week_day(132, 124);

  //zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
  tft.drawFastVLine(190, 115, 45, 0x07E0) ;
  print_sun(210, 121, false);



#endif //#ifndef tft2_S1

}


#ifdef def_th
//--------------------------------------------------------------
//                                S1_th_moon()
//--------------------------------------------------------------
void S1_th_moon(int x, int y)
{
  static uint32_t t_next_moon = 0;
  static uint32_t t_next_th = 0;


  //init
  if (refresh_all || ref_temp_u || ref_wh_now || ref_wh || ref_kv_th)
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
    drawJpeg(fileName, x + 10, y + 10);
    t_next_th = millis() + S_t_moon * 1000L;
    t_next_moon = t_next_th + S_t_bme * 1000L;
    //   Serial.println("S1_th_moon MOON");
  }

}


//-------------------------------------------------------------
//                  S1_clear_moon
///-------------------------------------------------------------
void S1_clear_moon(int x, int y)
{
  for (int i = 0; i < 51 ; i++) {
    tft.drawFastVLine(x + i + 1, y, 57, TFT_CYAN) ;
    tft.drawFastVLine(x + i, y, 57, TFT_BLACK) ;
    delay(7);
    tft.drawFastVLine(479 - i - 1, y, 57, TFT_CYAN) ;
    tft.drawFastVLine(479 - i, y, 57, TFT_BLACK) ;
    delay(7);
    //    yield();
  }
  tft.fillRect(x, y, 52, 57, TFT_BLACK);
}
#endif //#def_th

//-------------------------------------------------------------------
//                        S1_alert
//-------------------------------------------------------------------
//#ifndef tft2_S1
void S1_alert(bool sh)
{
  if (sh == true)
  {

    //alert  ttttttttttttttttttttttttttttttttttttttttttt
    if (temperatura_ch[0] != -100)
    {

      if (temperatura_ch[0] >= t_max || temperatura_ch[1] >= t_max || temp_ch_n >= t_max) //t alert
        tft.fillRect(315, 150, 54, 3, TFT_YELLOW);

      if (temperatura_ch[0] <= t_min || temperatura_ch[1] <= t_min || temperatura_ch[2] <= t_min || temp_ch_n <= t_min) //t alert
        tft.fillRect(315, 150, 54, 3, TFT_CYAN);


    }

    //vvvvvvvvvvvvvvvvvvvvvvvvvvv
    if (v_ch[0] != -100)
    {
      if (v_ch[0] >= v_max) //1,2 период по 3ч
        tft.fillRect(435, 46, 30, 3, TFT_CYAN);

    }

    //ppppppppppppppppppppppppppppppppppp
    int pp;
    if (pr_kv != -100) pp = pr_kv;
    else
      pp = davlenie0;
    if (pp != -100)
    {
      if (pp >= p_max || pp <= p_min)
        tft.fillRect(405, 83, 44, 3, TFT_GREEN);
    }

    //////uv
    if (uv >= uv_max)
      S_show_uv( false, 287, 0);

  }
  else //------------------------------------------------------
  {

    //////uv///////////////
    S_show_uv( true, 287, 0);


    //ppppppppppppppppppppppppppppppppp

    tft.fillRect(405, 83, 44, 3, TFT_BLACK);

    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

    tft.fillRect(435, 46, 30, 3, TFT_BLACK);


    //tttttttttttt
    tft.fillRect(315, 150, 54, 3, TFT_BLACK);


  }


}



#ifndef tft2_S1
//---------------------------------------------------------------
//                S1_clear_min()
//---------------------------------------------------------------
void S1_clear_min()
{

  for (byte f = 0; f < 112; f++) //62 низ
  {
    tft.drawFastHLine(204, f, 64, TFT_BLACK) ;
    tft.drawFastHLine(204, 112 - f, 64, TFT_BLACK) ;
    delay(7);
  }

}

#endif //#ifndef tft2_S1


//-------------------------------------------------------------
//                  s1_graf_t
///-------------------------------------------------------------
void S1_graf_t()
{
  if (temperatura7[0] == -100) return;

  unsigned int col;

  int y = 307;
  int x = 240;
  int y_min;
  if (temperatura_ch[0] < temperatura7[0])
    y_min = temperatura_ch[0];
  else
    y_min = temperatura7[0];
  col = color_t(temperatura_ch[0]);
  tft.drawLine(x, y - 2 * ((temperatura_ch[0] - y_min)), x + 30, y - 2 * ((temperatura7[1] - y_min)), col);
  x = x + 30;


  for (byte i = 1; i <= 4; i++)
  {
    col = color_t(temperatura7[i]);

    if (temperatura7[i] >= t_max || temperatura7[i] <= t_min)
      tft.fillCircle(x, y - 2 * ((temperatura7[i] - y_min)), 3, col);

    if (temperatura7[i + 1] == -100) break;


    //col=TFT_YELLOW;

    tft.drawLine(x, y - 2 * ((temperatura7[i] - y_min)), x + 60, y - 2 * ((temperatura7[i + 1] - y_min)), col);


    x = x + 60;

  }
}




//-------------------------------------------------------------
//                  S1_graf_davlenie
///-------------------------------------------------------------

void S1_graf_davlenie()
{


  int y = 317;
  int x = 240;
  int x_pmin = x;
  int davlenie_x = 1000;

  if (davlenie7[0] == -100 || davlenie0 == -100) return;
  tft.drawLine(x, y - ((davlenie0 - davlenie7[0])), x + 30, y - (davlenie7[1] - davlenie7[0]), 0x07E0);
  x = x + 30;

  for (byte i = 1; i <= 4; i++)
  {



    if (davlenie7[i] >= p_max || davlenie7[i] <= p_min)
      tft.fillCircle(x, y - ((davlenie7[i] - davlenie7[0])), 2, 0x07E0);

    if (davlenie_x > davlenie7[i])
    {
      davlenie_x = davlenie7[i];
      x_pmin = x;
    }
    if (davlenie7[i + 1] == -100) break;




    tft.drawLine(x, y - ((davlenie7[i] - davlenie7[0])), x + 60, y - (davlenie7[i + 1] - davlenie7[0]), 0x07E0);

    x = x + 60;
  }
  tft.setTextColor(0x07E0);
  tft.setFreeFont(v6);
  tft.setCursor(x_pmin, y - 20);
  tft.print(p7_max);
  tft.setCursor(x_pmin, y - 5);
  tft.print(p7_min);


}


#if defined (two_tft) && defined (tft2_S1)
#undef tft
#endif





#endif //S1
