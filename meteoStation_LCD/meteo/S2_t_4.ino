#if defined (S2) && defined(tft_480_320)&& defined (openw)


#if defined (two_tft) && defined (tft2_S2)
#define tft tft2
#endif



//-----------------------------------------------------------------------
//                        S2_time
//-----------------------------------------------------------------------
void S2_time()
{

  static bool fill_sec = false;

#if defined (two_tft) && defined (tft2_S2)

  if (fn_cmd_tmp == "prev" || fn_cmd_tmp == "next") //&& (play || play_mp3 || play_fm))
  {
    Serial.print("fn_cmd_tmp  "); Serial.println(fn_cmd_tmp);
    return;
  }
#endif

  if (refresh_all)  tft.fillScreen(TFT_BLACK);

  if (new_tick && !fill_sec)
  {
    fill_sec = true;
    S2_alert(false);
    if (S2 != 0)
    {
      tft.fillRoundRect(232, 170, 10, 20, 1, color_sec);
    } //sec black------------------------------------
  }
  else if (!new_tick && fill_sec)
  {
    fill_sec = false;

    S2_alert(true);
    //sec
    if (S2 != 0)
      tft.fillRoundRect(232, 170, 10, 20, 1, TFT_BLACK);
  }



  if (S2 == 0 && refresh_all)
  {
    draw_Calendar(0, 10, 220, 0);
    draw_Calendar(235, 10, 220, 1);
  }

  if (ref_temp_u == true || ref_wh_now == true || refresh_all == true) //есть данные now, Th
    S2_show_now();

  if (ref_wh == true || refresh_all == true) //обновилась погода
    S2_show_wh();
  else if (new_minute_b)  draw_line_Tu(120, 196, 123 );

#ifdef def_th
  if (ref_kv_th == true || refresh_all == true)
    S2_show_th();
  else if (new_minute_b)
    tft.drawRoundRect(0, 278, 119, 42, 2, c_th_frame);
#endif

#ifndef tft2_S2
  if (S2 != 0) //!calend

  {
    //если мин не изм  и не надо все обновить выходим
    //-----------------

    if (!new_minute_b && refresh_all == false) {
      return;
    }


    //////////////hour///hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
    String str1 = "";
    String fileName = "";

    if (refresh_all || now_min == 0)
    {
      if (now_hour < 10)
      {
        fileName = "n0_192";
        drawJpeg(fileName, 0, 0); //220
        fileName = "n" + String(now_hour) + "_192";
        drawJpeg(fileName, 115, 0); //220
      }
      else
      {
        str1 = String(now_hour);
        fileName = "n" + str1.substring(0, 1) + "_192";
        drawJpeg(fileName, 0, 0); //220
        fileName = "n" + str1.substring(1) + "_192";
        drawJpeg(fileName, 115, 0); //220

      }


    }



    //////////////////min

    if (now_min < 10)
    {
      fileName = "n0_192";
      drawJpeg(fileName, 250, 0); //220
      fileName = "n" + String(now_min) + "_192";
      drawJpeg(fileName, 365, 0); //220
    }
    else
    {
      str1 = String(now_min);
      fileName = "n" + str1.substring(0, 1) + "_192";
      drawJpeg(fileName, 250, 0); //220
      fileName = "n" + str1.substring(1) + "_192";
      drawJpeg(fileName, 365, 0); //220
    }

  }//S2 != 0

#endif // tft2_S2

  //дата--------data/////////ddddddddddddddddddddddddddddddddddddddddddddddd
  if (refresh_all == false && year(t) > 2015) //день недели может совпасть(01-01-), добавил год
    return;



  ////dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd

  tft.drawFastHLine(0, 194, 480, TFT_CYAN) ;
  tft.drawFastHLine(0, 195, 480, TFT_CYAN) ;

  String str2 = String(str_num(now_day));
  tft.setFreeFont(v24b);
  tft.setTextColor(c_date);


  tft.setCursor(5, 234);
  tft.print(str2);

  tft.setFreeFont(v12);
  str2 = '.' + String(str_num(now_month));
  tft.setTextColor(c_month);
  tft.print(str2);
  //#endif


  //день недели-------------------------
  draw_week_day(64, 239);

  print_sun(1, 242, false);
}

//====================================================
//                S2_alert
//====================================================

void S2_alert(bool sh)
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



#if defined (two_tft) && defined (tft2_S2)
#undef tft
#endif



#endif
