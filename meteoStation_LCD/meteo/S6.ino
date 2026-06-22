#if defined (S6) && defined(tft_320_240)&& defined (openw)
//time///////////////////////////////////////tttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt


#if defined (two_tft) && defined (tft2_S6)
#define tft tft2
#define refresh_all ref_tft2
#endif

void S6_time()
{

  static bool fill_sec = false;

  if (new_tick && !fill_sec)
  {
    fill_sec = true;
    S6_alert(false);
  }
  else if (!new_tick && fill_sec)
  {
    S6_alert(true);
    //sec
    fill_sec = false;
  }



  if (ref_temp_u || ref_wh_now || refresh_all) //есть данные с улицы
    S6_show_now();


#ifdef def_th
  if ((ref_kv_th || new_minute_b) && !refresh_all)   S_show_th_room(58, 191); //ref // S2_show_th();
#endif


  if (ref_temp_u || ref_wh_now || refresh_all) //есть данные с улицы
    S6_show_now();
  else if (new_minute_b) draw_line_Tu(104, 180, 60 );



  //если мин не изм  и не надо все обновить выходим
  //-----------------


#if (S6 == 1)
  {

    if (!new_minute_b && !refresh_all)       return;
    S6_draw_clock(210, 20);
  }
#endif


  //--------------------------------------------------------------------------------

  //дата--------data/////////ddddddddddddddddddddddddddddddddddddddddddddddd
  if (!refresh_all && now_year > 2015) //день недели может совпасть(01-01-), добавил год
    return;


#if (S6 == 0) //no clock
  draw_Calendar(10, 10, 200, 0);
  S6_show_moon(230, 40);

#elif (S6 == 1) //lit clock
  draw_Calendar(0, 10, 200, 0);
  S6_show_moon(230, 60);

#endif

  ////dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd

  tft.fillRect(0, 178, 103, 61, TFT_BLACK); //data
  tft.drawFastHLine(0, 176, 320, TFT_CYAN) ;
  tft.drawFastHLine(0, 178, 320, TFT_CYAN) ;

  String str = String(str_num(now_day));
  tft.setFreeFont(v18b);
  tft.setTextColor(c_date);
  tft.setCursor(5, 207);
  tft.print(str);


  //tttttttttthhhhhhhhhhhhhhhhhhhhh

#ifdef def_th
  S_show_th_room(58, 191);
#else
  //mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
  tft.setFreeFont(v12);
  str = '.' + String(str_num(now_month));
  tft.setTextColor(c_month);
  tft.print(str);
#endif

  //день недели-------------------------
  draw_week_day(10, 208);






}

//----------------------------------------------------------------------------
//                      S6_show_now()
//------------------------------------------------------------================
void S6_show_now()
{

  int temp = -100;
  tft.fillRect(106, 179, 214, 61, TFT_BLACK);



  //ttttttttttttttttttttttttttttttttttttttttt
  if (temperatura_ch[0] == -100 && temp_u == -100) return; //!=данные мз инета

  temp = temperatura_ch[0];

  if (temp_u != -100)
  {
    temp = temp_u;

  }

  draw_line_Tu(104, 180, 60 );

  if (temp != -100)
  {
    tft.setFreeFont(v24b);
    uint32_t color_temp = color_t(temp);
    tft.setTextColor(color_temp);
    if (temp < -9 || temp > 9)
    {
      tft.setCursor(163, 225);
      if (temp < -9)
      {
        tft.fillRect(150, 207, 13, 7, color_temp);
        tft.drawRect(150, 207, 13, 7, TFT_BLUE);
      }
    }
    else
    {
      tft.setCursor(173, 225);
      if (temp < 0)
      {
        tft.fillRect(160, 207, 13, 7, color_temp);
        tft.drawRect(160, 207, 13, 7, TFT_BLUE);
      }
    }
    //  tft.print((int)(temp));
    tft.print(abs(int(temp)));

    //=====icon

    if (temp < -9)drawJpeg(icon_ch[0] + "32", 115, 180);
    else
      drawJpeg(icon_ch[0] + "48", 115, 180); //220


    //nnnnnnnnnnnnnnnnnnnnnnnn
    if (temp_ch_n != -100)
    {
      color_temp = color_t(temp_ch_n);
      tft.setTextColor(color_temp);
      if (abs(temp) > 9) tft.setFreeFont(v9);
      else tft.setFreeFont(v12);

      tft.print(temp_ch_n);


    }

    tft.setFreeFont(v6);
  }


  //////////icon  vvvvvv----------------------------------------
  float gr = int(v_gr_ch[0]);
  do_deg(gr);
  if (wind_ico != "")
  {
    drawJpeg(wind_ico, 240, 180); //220
  }


  //тенденция ветра
  if (v_ch[0] != -100) //есть данные мз инета
  {

    if (v_ch[0] < v_ch[1])
      tft.drawTriangle(283, 200, 280, 190, 277, 200, TFT_YELLOW);
    if (v_ch[0] > v_ch[1])
      tft.drawTriangle(283, 190, 277, 190, 280, 200, TFT_CYAN);


    tft.setFreeFont(v12);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setCursor(287, 202);
    tft.print(v_ch[0]);
    tft.setFreeFont(v6);
    if (v_ch[0] < 10)
    {
      tft.setTextColor(TFT_WHITE);
      tft.print("m");
    }

  }


  //ppppppppppppppppppppppppppppppppppppppppppppppppppppppp
  if (davlenie0 != -100)
  {

    if (davlenie0 < davlenie7[1])
      tft.drawTriangle(283, 225, 280, 215, 277, 225, TFT_YELLOW);

    if (davlenie0 > davlenie7[1])
      tft.drawTriangle(283, 215, 277, 215, 280, 225, TFT_CYAN);



#ifndef hPa
    tft.setFreeFont(v9);
#else
    tft.setFreeFont(v6);
#endif

    tft.setTextColor(c_pr_ow);
    tft.setCursor(285, 225);

    if (pr_kv != -100)
    {
      tft.setTextColor(c_pr_bme);
      tft.print(pr_kv);
    }
    else
      tft.print(davlenie0);
    tft.setFreeFont(v6);


  }



  //prim///////////////////
#ifdef EN
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setCursor(125, 238);
  tft.setFreeFont(v6);
  tft.print(descript_now);
#endif



  //rrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
  do_draw_rain(110, 200, rain_ch[0], TFT_CYAN);

}

//====================================================
//                S6_draw_clock
//====================================================
#if (S6 == 1)
void S6_draw_clock(int x, int y)
{
  tft.fillRect(x, y, 108, 33, TFT_BLACK);
  //////////////hour///hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh

  tft.setTextColor(S6_color_clock);
  tft.setFreeFont(v18b);
  String st = String(str_num(now_hour));

  tft.setCursor(x + 1, y + 29);
  tft.print(st);
  tft.setTextColor(TFT_CYAN);
  tft.setFreeFont(v9b);
  tft.print(":");
  // st = st + ":";
  tft.setTextColor(0xFFF1);
  tft.setFreeFont(v18b);

  ///min mmmmmmmmmmmmmmmmmmmmmmmmmmm
  st = String(str_num(now_min));
  tft.print(st);
  tft.drawRoundRect(x, y, 108, 33, 2, TFT_LIGHTGREY);
}
#endif

//====================================================
//                S6_show_moon
//====================================================

void S6_show_moon(int x, int y) //66x110
{
  tft.fillRect(x, y, 66, 110, TFT_BLACK);
  //z-v luna
  //----------------------------------------------
  ////////////////восход зах.///////////////////////////////////
  tft.setFreeFont(v9);
  //  tft.fillTriangle(0, 115, 10, 115, 5, 110, TFT_YELLOW);
  tft.fillTriangle(x + 2, y + 18, x + 10, y + 18, x + 6, y + 8, TFT_YELLOW);

  tft.setTextColor(TFT_YELLOW);
  //  tft.setCursor(10, 118);
  tft.setCursor(x + 11, y + 18);
  tft.print(t_sun_v);
  //zahod
  //  tft.fillTriangle(0, 135, 10, 135, 5, 140, TFT_RED);
  tft.fillTriangle(x + 2, y + 30, x + 10, y + 30, x + 6, y + 40, TFT_RED);
  tft.setTextColor(TFT_ORANGE);
  //tft.setCursor(10, 141);
  tft.setCursor(x + 11, y + 41);
  tft.print(t_sun_z);
  tft.setFreeFont(v6);

  //---------------moon
  String fileName;
  fileName = do_phasemoon() + "_64";

  //  tft.drawFastHLine(0, 157, 65, TFT_YELLOW);
  // drawJpeg(fileName, 0, 160);
  drawJpeg(fileName, x, y + 45);
  tft.drawRoundRect(x, y, 66, 110, 5, TFT_LIGHTGREY);




}


//====================================================
//                S6_alert
//====================================================

void S6_alert(bool sh)
{

  if (sh == true)
  {
    //rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
    if (rain_ch[0] != 0)
      if (rain_ch[1] > rain_l)
        tft.fillCircle(112, 230, 3, TFT_CYAN);

    if (rain_ch[0] >= rain_m * 2)
      do_draw_rain(110, 200, rain_ch[0], TFT_CYAN);


    //alert  ttttttttttttttttttttttttttttttttttttttttttt
    if (temperatura_ch[0] != -100 && temperatura_ch[1] != -100)
    {

      if (temperatura_ch[0] >= t_max || temperatura_ch[1] >= t_max || temp_ch_n >= t_max) //t alert
        tft.fillRect(166, 182, 57, 3, TFT_YELLOW);


      if (temperatura_ch[0] <= t_min || temperatura_ch[1] <= t_min || temperatura_ch[2] <= t_min || temp_ch_n <= t_min) //t alert
        tft.fillRect(166, 182, 57, 3, TFT_CYAN);
    }

    //vvvvvvvvvvvvvvvvvvvvvvvvvvv
    if (v_ch[0] != -100)
    {
      if (v_ch[0] >= v_max || v_ch[1] >= v_max) //1,2 период по 3ч
        tft.fillRect(285, 207, 30, 2, TFT_CYAN);
    }

    //ppppppppppppppppppppppppppppppppppp
    int pp;
    if (pr_kv != -100) pp = pr_kv;
    else
      pp = davlenie0;
    if (pp != -100)
    {
      if (pp >= p_max || pp <= p_min)
        tft.fillRect(285, 230, 32, 2, TFT_GREEN);

    }


    //////uv
    if (uv >= uv_max)
      S_show_uv( false, 108, 181);
  }
  else //------------------------------------------------------
  {

    //uuuuuuuuuuuuuuuuuuu
    S_show_uv( true, 108, 181);


    //ppppppppppppppppppppppppppppppppp
    tft.fillRect(285, 230, 32, 2, TFT_BLACK);

    //vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    tft.fillRect(285, 207, 30, 2, TFT_BLACK);

    //rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
    tft.fillCircle(112, 230, 3, TFT_BLACK); //BLACK
    if (rain_ch[0] >= rain_m * 2)
      do_draw_rain(110, 200, rain_ch[0], TFT_BLACK);


    //tttttttttttt
    tft.fillRect(166, 182, 57, 3, TFT_BLACK);



  }

}



#if defined (two_tft) && defined (tft2_S6)
#undef tft //tft2
#undef refresh_all
#endif





#endif
