#if defined (pin_sd) && defined (tft2_Sf)&&!defined (epd)
//-------------------------------------------------------------
//              Sf2_time
//-------------------------------------------------------------
void Sf2_time()
{
  static time_t next_time = 0;
  if (ref_tft2) next_time = 5000;


  if (millis() < next_time)      return;


  if (max_jpg_num == 0)
  {
    Serial.println(F("count Foto=0"));
    tft2.setCursor(40, 40);
    tft2.setTextColor(TFT_ORANGE);
    tft2.setFreeFont(v9mono);
    tft2.print("count Foto=0");
    next_time = millis() + 2000 * 1000L;
    return;
  }




  //ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
  unsigned int r_num = random(1, max_jpg_num + 1);
  String fname = F("/foto/");
  fname += String(r_num);
  tft2.fillScreen(TFT_BLACK);
  if (!drawJpeg(fname, 0, 0))
  {
    tft2.setTextColor(TFT_RED);
    tft2.setFreeFont(v9mono);
    tft2.setCursor(40, 40);
    tft2.print("ERR foto "); tft2.print(fname);
    next_time = millis() + 2 * 1000L;
    return;
  }





#ifdef tft_320_240
  tft2.drawFastVLine(0, 0, 239, Sf_c_scr_frame) ;
  tft2.drawFastVLine(319, 0, 239, Sf_c_scr_frame) ;
  tft2.drawFastHLine(0, 0, 319, Sf_c_scr_frame) ;
  tft2.drawFastHLine(0, 239, 319, Sf_c_scr_frame) ;
#endif

#ifdef tft_480_320
  tft2.drawFastVLine(0, 0, 319, Sf_c_scr_frame) ;
  tft2.drawFastVLine(479, 0, 319, Sf_c_scr_frame) ;
  tft2.drawFastHLine(0, 0, 479, Sf_c_scr_frame) ;
  tft2.drawFastHLine(0, 319, 479, Sf_c_scr_frame) ;

#endif

  next_time = millis() + Sf_t_slide * 1000L;

}

#endif //pin_sd
