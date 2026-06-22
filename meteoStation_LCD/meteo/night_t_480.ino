#if defined (Sn) && defined(tft_480_320)

void Sn_time()
{
  static bool fill_sec = false;


  if (new_tick && !fill_sec)
  {
    tft.fillRect(220, 60, 20, 20, color_sec);
    tft.fillRect(220, 180, 20, 20, color_sec);
    fill_sec = true;
  }
  else if (!new_tick && fill_sec)
  {
    tft.fillRect(220, 60, 20, 20, TFT_BLACK);
    tft.fillRect(220, 180, 20, 20, TFT_BLACK);
    fill_sec = false;
  }



  if (!new_minute_b && refresh_all == false && !ref_temp_u && !ref_kv_th)
  {
    return;
  }

  String st;
  String fileName;
  //////////////hour
  if (refresh_all || now_min == 0)
  {
    if (now_hour < 10)
    {
      fileName = F("n0n");
      drawJpeg(fileName, 0, 0); //220

      fileName = "n" + String(now_hour) + F("n");
      drawJpeg(fileName, 100, 0);
    }
    else
    {
      st = String(now_hour);
      fileName = "n" + st.substring(0, 1) + F("n");
      drawJpeg(fileName, 0, 0); //220
      fileName = "n" + st.substring(1) + F("n");
      drawJpeg(fileName, 100, 0);
    }
  }

  //////////////////min///////////////////////////
  if (now_min < 10)
  {
    fileName = F("n0n");
    drawJpeg(fileName, 250, 0); //220

    fileName = "n" + String(now_min) + "n";
    drawJpeg(fileName, 350, 0); //220

  }
  else
  {
    st = String(now_min);
    fileName = "n" + st.substring(0, 1) + F("n");
    drawJpeg(fileName, 250, 0); //220

    fileName = "n" + st.substring(1) + F("n");
    drawJpeg(fileName, 350, 0);

  }


  if (refresh_all || ref_temp_u || ref_kv_th || ref_wh_now)
  {
    if (ref_wh_now)  refresh_all = true; //ref all
    do_show_th_n();

  }


}
#endif




#if defined(tft_480_320)
//-------------------------------------------------------------------
//          do_show_th_n
//-------------------------------------------------------------------
void do_show_th_n()
{

  if (refresh_all)
  {
    tft.fillRect(0, 254, 479, 65, TFT_BLACK);
    tft.drawFastHLine(0, 254, 479, TFT_GREENYELLOW) ;
  }


#ifdef def_th
  S_show_th_room(421, 257);
#endif
  draw_line_Tu(225, 256, 63 );
  if (new_minute_b && !refresh_all && !ref_kv_th && !ref_temp_u)
    return;//ref line only

  int  temp = -100;

#ifdef openw
  temp = temperatura_ch[0];
#endif //openw

  int color = TFT_LIGHTGREY;

  if (temp_u != -100)
  {
    temp = temp_u;
    //   color = c_tu_frame;
  }



  if (temp != -100)
  {
    //   tft.drawRect(225, 256, 4, 63, color) ;
    tft.setFreeFont(v24b);
    tft.setTextColor(color_t(temp));
#ifdef openw
    if (!refresh_all) tft.fillRect(305, 255, 105, 64, TFT_BLACK);
    tft.setCursor(305, 309);
#else
    if (!refresh_all) tft.fillRect(262, 255, 105, 64, TFT_BLACK);
    tft.setCursor(262, 309);
#endif
    tft.print(temp);
  }




  if (!refresh_all) return;

  //---------------------------------------------

#ifdef openw
  if (rain_ch[0] > 0)
    do_draw_rain(244, 274, rain_ch[0], TFT_CYAN);
  //icon
  drawJpeg(icon_ch[0] + "48", 250, 269); // текущая погода
  //uuuuuuuuuu
  if (uv >= 0) S_show_uv(true, 230, 255);
  //vvvvvvvvvvv
#ifndef def_th
  int gr = int(v_gr_ch[0]);
  do_deg(gr);
  drawJpeg(wind_ico, 429, 259);
  tft.setCursor(429, 312);
  tft.setTextColor(TFT_CYAN);
  tft.setFreeFont(v12);
  tft.print(v_ch[0]);
#endif //th 
#endif //openw


  //pppppppppppppppppppppppppppppp
#ifndef openw
  if (pr_kv != -100)
  {
    tft.setTextColor(c_pr_bme);
    tft.setFreeFont(v12);
    tft.setCursor(367, 309);
    tft.print(pr_kv);
  }
#endif



  //dddddddddddddddddddddddddddddddddddddddddddddddddddd
  String st = String(str_num(now_day));
  /*
    if (day(t) < 10)
      st = "0" + String(day(t));
    else
      st = String(day(t));
  */

  tft.setFreeFont(v24b);
  tft.setTextColor(c_date);
  tft.setCursor(0, 314);
  tft.print(st);
  /*
    if (month(t) < 10)
      st = "0" + String(month(t));
    else
      st = String(month(t));
  */
  st = String(str_num(now_month));
  tft.setFreeFont(v18b);
  st = '.' + st;
  tft.setTextColor(c_month);
  tft.print(st);

  //moon
  String fileName = do_phasemoon() + "_32";;
  drawJpeg(fileName, 180, 255);

  byte wd = weekday(t) - 1;
  if (wd == 0) wd = 7;

#ifdef EN
  drawJpeg('w' + String(wd) + 'e', 150, 287); //220
#else
  drawJpeg('w' + String(wd), 150, 287); //220
#endif


  //------------------------------


}



#endif
