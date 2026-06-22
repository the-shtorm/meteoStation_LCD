
#ifdef openw
void do_openw()
{
  static uint32_t t_next = 0;

  if (millis() < t_next || fn_cmd != "") return; //команда главнее
  t_next = millis() + 5000UL;

  if (millis() < lastConnectionTime_now) lastConnectionTime_now = 0;
  if (millis() - lastConnectionTime_now > postingInterval_now || lastConnectionTime_now == 0)
  {
    if (do_parse_now())  //zapros ch
    {
      ref_wh_now = true;
      //      return; //один запрос с ходу при старте
    }

  }

  delay(500);


  // wh 3 hour--------------------------------------
  if (millis() < lastConnectionTime_ch) lastConnectionTime_ch = 0;
  if (millis() - lastConnectionTime_ch > postingInterval_ch || lastConnectionTime_ch == 0) {

    if (do_parse_ch())  //zapros ch if entries>0
    {
      ref_wh = true;
    }
  }


}



//===================================================================================================
//            do_draw_rain_mm
//===================================================================================================

void do_draw_rain_mm(int x, int y, float mm, int color)
{


#if defined (two_tft)
  if (tft2_act)
  {
    tft2.setTextColor(color);
    tft2.setFreeFont(v6);
    tft2.setCursor(x, y);
    char outstr[5];
    dtostrf(mm, 4, 1, outstr);
    tft2.print(outstr);
  }
  else
  {
    tft.setTextColor(color);
    tft.setFreeFont(v6);
    tft.setCursor(x, y);
    char outstr[5];
    dtostrf(mm, 4, 1, outstr);
    tft.print(outstr);

  }

#else
  tft.setTextColor(color);
  tft.setFreeFont(v6);
  tft.setCursor(x, y);
  char outstr[5];
  dtostrf(mm, 4, 1, outstr);
  tft.print(outstr);

#endif




}




//===================================================================================================
//            do_draw_rain
//===================================================================================================

void do_draw_rain(int x, int y, float mm, int color)
{

  if (mm == 0)  return;
  byte step = 8;

#if defined (two_tft)
  if (tft2_act)
  {
    tft2.fillCircle(x, y, 2, color);
    if (mm >= rain_l) tft2.fillCircle(x, y + step, 2, color);
    if (mm >= rain_m) tft2.fillCircle(x, y + step * 2, 2, color);

  }
  else
  {
    tft.fillCircle(x, y, 2, color);
    if (mm >= rain_l) tft.fillCircle(x, y + step, 2, color);
    if (mm >= rain_m) tft.fillCircle(x, y + step * 2, 2, color);


  }
#else
  {
    tft.fillCircle(x, y, 2, color);
    if (mm >= rain_l) tft.fillCircle(x, y + step, 2, color);
    if (mm >= rain_m) tft.fillCircle(x, y + step * 2, 2, color);


  }
#endif



}





//===================================================================================================
//            S_show_uv
//===================================================================================================
//показ значка uv


void S_show_uv(bool vis, int x, int y)
{
	#ifndef api_now_2023
  ///////////////UUUUUUUUVVVVVVVVVVVVV////////////////////////
  int u_color;
  x++;
  y++;

  if (vis == true)
    u_color = do_uvcolor();
  else
    u_color = TFT_BLACK;



#if defined (two_tft)
  if (tft2_act)
  {
    tft2.fillRoundRect(x, y, 8, 8, 3, u_color);
    tft2.drawLine(x + 2, y + 1, x + 16, y, u_color);
    tft2.drawLine(x + 2, y + 2, x + 16, y + 14, u_color);
    tft2.drawLine(x + 2, y + 1, x + 2, y + 14, u_color);
    if (int(uv) > 0)
    {
      tft2.setTextColor(TFT_YELLOW);
      tft2.setFreeFont(v6);
      tft2.setCursor(x, y + 24);
      tft2.print(int(uv));
    }


  }
  else
  {
    tft.fillRoundRect(x, y, 8, 8, 3, u_color);
    tft.drawLine(x + 2, y + 1, x + 16, y, u_color);
    tft.drawLine(x + 2, y + 2, x + 16, y + 14, u_color);
    tft.drawLine(x + 2, y + 1, x + 2, y + 14, u_color);
    if (int(uv) > 0)
    {
      tft.setTextColor(TFT_YELLOW);
      tft.setFreeFont(v6);
      tft.setCursor(x, y + 24);
      tft.print(int(uv));
    }


  }
#else
  tft.fillRoundRect(x, y, 8, 8, 3, u_color);
  tft.drawLine(x + 2, y + 1, x + 16, y, u_color);
  tft.drawLine(x + 2, y + 2, x + 16, y + 14, u_color);
  tft.drawLine(x + 2, y + 1, x + 2, y + 14, u_color);
  if (int(uv) >= 0)
  {
    tft.setTextColor(TFT_YELLOW);
    tft.setFreeFont(v6);
    tft.setCursor(x, y + 24);
    tft.print(int(uv));
  }


#endif

#endif //2023


}


//===================================================================================================
//            do_uvcolor
//===================================================================================================

int do_uvcolor()
{
  int color_uv;
  if (uv < 0) color_uv = TFT_LIGHTGREY; //нет данный закрашиваю
  else if (uv <= uv1)
    color_uv = uv1_color;
  else if (uv <= uv2)
    color_uv = uv2_color;
  else if (uv <= uv3)
    color_uv = uv3_color;
  else if (uv <= uv4)
    color_uv = uv4_color;
  else if (uv <= uv5)
    color_uv = uv5_color;

  return color_uv;
}



//==============================================================
//								do_deg
//==============================================================
//напр. ветра
byte do_deg(float gr)
{
  int8_t ret = -1;
  wind_ico = "";
#ifdef Sg
  c_wind = 0;
#endif

  if (gr >= 0 && gr <= 30) {
    wind_ico = "wind_0";
    c_wind = TFT_CYAN;
    ret = 0;
  }
  if (gr >= 31 && gr <= 60) {
    wind_ico = "wind_2";
    c_wind = TFT_CYAN;
  }

  if (gr >= 61 && gr <= 120) {
    wind_ico = "wind_4";  //восток
    c_wind = Sg_c_wing_E;
    ret = 4;
  }

  if (gr >= 121 && gr <= 150) {
    wind_ico = "wind_6";
    c_wind = TFT_YELLOW;
  }
  if (gr >= 151 && gr <= 210) {
    wind_ico = "wind_8";
    c_wind = TFT_YELLOW;
    ret = 8;
  }
  if (gr >= 211 && gr <= 240) {
    wind_ico = "wind_10";
    c_wind = TFT_YELLOW;
    ret = 10;
  }
  //if (gr>=241 && gr<=300){wind_ico="/wind_12";c_wind=0x3DE4;}
  if (gr >= 241 && gr <= 300) {
    wind_ico = "wind_12";  //запад
    c_wind = Sg_c_wing_W;
    ret = 12;
  }

  if (gr >= 301 && gr <= 330) {
    wind_ico = "wind_14";
    c_wind = TFT_CYAN;
    ret = 14;
  }
  if (gr >= 331 && gr <= 360) {
    wind_ico = "wind_0";
    c_wind = TFT_CYAN;
    ret = 0;
  }
  return ret;
}



//==============================================================
//                color_day
//==============================================================

unsigned int color_day(byte w_day)
{
  unsigned int color = 0;
  if (w_day == 1)
    color = c_holiday;
  else if (w_day == 7)
    color = c_holiday;
  else if (w_day == 2)
    color = c_monday;
  else if (w_day == 6)
    color = c_friday;
  else
    color = c_working;

  return color;
}

#endif  //openw
