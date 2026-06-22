#if defined (S5) && defined(tft_480_320)&& defined (openw)



//-----------------------------------------------------------------------
//                        S5_time
//-----------------------------------------------------------------------
void S5_time()
{

  static bool fill_sec = false;


  if (new_tick && !fill_sec)
  {
    fill_sec = true;
    S5_alert(false);
    tft.fillCircle(135, 135, 4, color_sec);
  } //sec black------------------------------------
  else if (!new_tick && fill_sec)
  {
    S5_alert(true);
    fill_sec = false;
    //sec
    tft.fillCircle(135, 135, 4, TFT_BLACK);

  }



  if (ref_temp_u == true || ref_wh_now == true || refresh_all == true) //есть данные now, Th
    S5_show_now();

  if (ref_wh == true || refresh_all == true) //обновилась погода
    S5_show_wh();
  else if (new_minute_b)  draw_line_Tu(120, 196, 123 );

#ifdef def_th
  if (ref_kv_th == true || refresh_all == true)
    S5_show_th();
  else if (new_minute_b)
    tft.drawRoundRect(0, 275, 119, 45, 2, c_th_frame);
#endif

  if   ( refresh_all == true) draw_Calendar(270, 10, 200, 0);



  if (!new_minute_b && refresh_all == false) {
    return;
  }





  //////////////hour///hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
  String str1 = "";
  String fileName;

  if (refresh_all || now_min == 0)
  {

    if (now_hour < 10)
    {
      fileName = "n0_112";
      drawJpeg(fileName, 0, 30); //220
      fileName = "n" + String(now_hour) + "_112";
      drawJpeg(fileName, 64, 30); //220
    }
    else
    {
      str1 = String(now_hour);
      fileName = "n" + str1.substring(0, 1) + "_112";
      drawJpeg(fileName, 0, 30); //220
      fileName = "n" + str1.substring(1) + "_112";
      drawJpeg(fileName, 64, 30); //220

    }


  }



  //////////////////min

  if (now_min < 10)
  {
    fileName = "n0_112";
    drawJpeg(fileName, 140, 30); //220
    fileName = "n" + String(now_min) + "_112";
    drawJpeg(fileName, 204, 30); //220
  }
  else
  {
    str1 = String(now_min);
    fileName = "n" + str1.substring(0, 1) + "_112";
    drawJpeg(fileName, 140, 30); //220
    fileName = "n" + str1.substring(1) + "_112";
    drawJpeg(fileName, 204, 30); //220

  }





  //дата--------data/////////ddddddddddddddddddddddddddddddddddddddddddddddd
  if (refresh_all == false && year(t) > 2015) //день недели может совпасть(01-01-), добавил год
    return;



  ////dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd
  //tft.fillRect(0,195,103,60,TFT_BLACK); //data
  tft.drawFastHLine(0, 194, 480, TFT_CYAN) ;
  tft.drawFastHLine(0, 195, 480, TFT_CYAN) ;


  String str2 = "";
  /*
    if (day(t) < 10)
    str = "0" + String(day(t));
    else
    str = String(day(t));
  */
  str2 = String(str_num(now_day));

  tft.setFreeFont(v24b);
  tft.setTextColor(c_date);

#ifdef def_th
  byte b = 0;
#else
  byte b = 20;
#endif

  tft.setCursor(5, 240 + b);
  tft.print(str2);

  tft.setFreeFont(v12);
  /*
    if (month(t) < 10)
    str = ".0" + String(month(t));
    else
    str = '.' + String(month(t));
  */
  str2 = String(str_num(now_month));
  tft.setTextColor(c_month);
  tft.print(str2);
  //#endif


  //день недели-------------------------
  draw_week_day(40, 241 + b);

  //moon
  fileName = do_phasemoon() + "_32";
  drawJpeg(fileName, 10, 155);

  //zzzzzzzzzzzzzzzzzzz
  tft.setFreeFont(v12);
  tft.setTextColor(TFT_YELLOW);
  tft.setCursor(70, 180);
  tft.print(t_sun_v);
  //zahod
  tft.setTextColor(TFT_ORANGE);
  tft.setCursor(170, 180);
  tft.print(t_sun_z);

}




//====================================================
//                S5_alert
//====================================================

void S5_alert(bool sh)
{

  if (sh == true)
  {

    //alert  ttttttttttttttttttttttttttttttttttttttttttt
    if (temperatura_ch[0] != -100)
    {

      if (temperatura_ch[0] >= t_max || temperatura_ch[1] >= t_max || temp_ch_n >= t_max) //t alert
        tft.drawFastHLine(232, 249, 60, TFT_YELLOW) ;

      if (temperatura_ch[0] <= t_min || temperatura_ch[1] <= t_min || temperatura_ch[2] <= t_min || temp_ch_n <= t_min) //t alert
        tft.drawFastHLine(232, 249, 60, TFT_CYAN) ;

    }

    //vvvvvvvvvvvvvvvvvvvvvvvvvvv
    if (v_ch[0] != -100)
    {
      if (v_ch[0] >= v_max || v_ch[1] >= v_max) //1,2 период по 3ч
        tft.fillRect(170, 316, 20, 3, TFT_CYAN);
    }

    //ppppppppppppppppppppppppppppppppppp
    int pp;
    if (pr_kv != -100) pp = pr_kv;
    else
      pp = davlenie0;
    if (pp != -100)
    {
      if (pp >= p_max || pp <= p_min)
        tft.fillRect(240, 316, 42, 3, TFT_GREEN);

    }


    //////uv
    if (uv >= uv_max)
      S_show_uv( false, 122, 198);
  }
  else //------------------------------------------------------
  {

    //uuuuuuuuuuuuuuuuuuu
    S_show_uv( true, 122, 198);


    //ppppppppppppppppppppppppppppppppp
    tft.fillRect(240, 316, 42, 3, TFT_BLACK);
    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    tft.fillRect(170, 316, 20, 3, TFT_BLACK);

    //tttttttttttt
    tft.drawFastHLine(232, 249, 60, TFT_BLACK) ;


  }

}





#endif
