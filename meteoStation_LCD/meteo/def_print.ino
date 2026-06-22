//------------------------------------------------------------------------------------------
//                              do_show_define
//------------------------------------------------------------------------------------------
void do_show_define()
{
  String str = "";
  //#if defined (wf_send)
  if (send_def_wf == false) //not wifi send info
  {
#ifdef tft_320_240
    tft.setFreeFont(v6);
#endif
#ifdef tft_480_320
    tft.setFreeFont(v9mono);
#endif
#ifdef tft_400_300
    tft.setFreeFont(v7);
#endif
  }

  /*
    #ifdef debug
    Serial.println( "Compiled: " __DATE__ ", " __TIME__ ", " __VERSION__);
    Serial.print(F( "Arduino IDE version: ")); Serial.println( ARDUINO, DEC);
    Serial.print("SdkVersion: ");
    Serial.println(ESP.getSdkVersion());
    #endif
  */
#if defined (wf_send)||defined (debug)
  if (send_def_wf)
  {
    str = "Compiled: " + String( __DATE__) + ", " + String( __TIME__) + ", " + String(__VERSION__);
    print_define (str.c_str(), -100);
    str = F("Arduino IDE version:"); str += String(ARDUINO);
    print_define (str.c_str(), -100);
    str = F("SdkVersion: "); str += String(ESP.getSdkVersion());
    print_define (str.c_str(), -100);
    str = "";
  }
#endif
  str = String(ota_name);
  str += ":V:";
  str += String(ver);

#ifdef ESP8266
  //  print_define ( (char *)"ESP8266", -1);
  str = str + ";E8266";
#endif

#ifdef ESP32
  //  print_define ( (char *)"ESP32", -1);
  str = str + ";E32";
#endif


#ifdef debug
  //  print_define ((char *)"debug", -1);
  str = str + ";dbg";
#endif

#ifdef ftp
  //  print_define ((char *)"ftp", -1);
  str = str + ";ftp";
#endif


#if defined (iot_send)
  str = str + ";IoT_S";
#endif
#if defined (iot_read)
  str = str + ";IoT_R";
#endif

#if defined (mqtt)
  str = str + ";mqtt";
#ifdef mqtt_read
  str = str + "/R";
#endif
#ifdef mqtt_send
  str = str + "/S";
#endif
#endif//mqtt

#if defined (mqtt2)
  str = str + ";mqtt2";
#ifdef mqtt_read
  str = str + "/R";
#endif
#ifdef mqtt_send
  str = str + "/S";
#endif
#endif//mqtt2

#if defined (mqtt3)
  str = str + ";mqtt3";
#ifdef mqtt_read
  str = str + "/R";
#endif
#ifdef mqtt_send
  str = str + "/S";
#endif
#endif//mqtt3

#ifdef def_core0
  str = str + ";core0";
#endif

#ifdef wifi_multi
  str = str + ";WFmulti";
#endif

  print_define (str.c_str(), -1);
  str = "";

  //---------------------------

#ifdef openw
  str = str + "openW;";
#else
  str = str + "NO openW;";
#endif

#ifdef tft_320_240
#ifdef AUDIO
  str = str + "TFT 320X240;AUDIO;";
#else
  str = str + "320X240 #NO AUDIO;";
#endif
#endif

#ifdef tft_480_320
#ifdef AUDIO
  str = str + "TFT 480X320;AUDIO;";
#else
  str = str + "480X320 #NO AUDIO;";
#endif
#endif
  print_define (str.c_str(), -1);
  str = "";
  //----------

  //1TFT
#if !defined(ESP32_PARALLEL)
#ifdef TFT_MOSI
  str = "MOSI " + String(TFT_MOSI);
#endif
#ifdef TFT_SCLK
  str = str + ";SCLK " + String(TFT_SCLK);
#endif
#ifdef TFT_CS
  str = str + ";CS " + String(TFT_CS);
#endif

#ifdef TFT_DC
  str = str + ";DC " + String(TFT_DC);
#endif
#ifdef TFT_RST
  str = str + ";RST " + String(TFT_RST);
#endif
#ifdef TFT_MISO
  str = str + ";MISO " + String(TFT_MISO);
#endif
  print_define (str.c_str(), -1);

  //----------------------
#if defined (two_tft)
  //2TFT
#ifdef TFT_MOSI2
  str = "MOSI2 " + String(TFT_MOSI2);
#endif
#ifdef TFT_SCLK2
  str = str + ";SCLK2 " + String(TFT_SCLK2);
#endif
#ifdef TFT_CS2
  str = str + ";CS2 " + String(TFT_CS2);
#endif

#ifdef TFT_DC2
  str = str + ";DC2 " + String(TFT_DC2);
#endif
#ifdef TFT_RST2
  str = str + ";RST2 " + String(TFT_RST2);
#endif
#ifdef TFT_MISO2
  str = str + ";MISO2 " + String(TFT_MISO2);
#endif
  print_define (str.c_str(), -1);



#if defined (tft2_S1)
  print_define ((char *)"two_tft tft2_S1", -1);
#endif

#if defined (tft2_S2)
  print_define ((char *)"two_tft tft2_S2", -1);
#endif


#if defined (tft2_S3)
  print_define ((char *)"two_tft tft2_S3", -1);
#endif


#if defined (tft2_S4)
  print_define ((char *)"two_tft tft2_S4", -1);
#endif


#if defined (tft2_S5)
  print_define ((char *)"two_tft tft2_S5", -1);
#endif


#if defined (tft2_S6)
  print_define ((char *)"two_tft tft2_S6", -1);
#endif


#if defined (tft2_Sg)
  print_define ((char *)"two_tft tft2_Sg", -1);
#endif


#if defined (tft2_Sf)
  print_define ((char *)"two_tft tft2_Sf", -1);
#endif
#endif //2tft
#endif // !defined(ESP32_PARALLEL)  


#ifdef TFT_ESPI_VERSION
  //  Serial.print(F("TFT_ESPI ver:")); Serial.println(TFT_ESPI_VERSION);
  str = "espi:" + String(TFT_ESPI_VERSION);
  print_define (str.c_str(), -1);
#endif

#if defined(ESP32_PARALLEL)
  str = "";
#ifdef TFT_CS      // Write strobe control pin - must use a pin in the range 0-31
  str = str + "CS:" + TFT_CS;
#endif
#ifdef TFT_DC      // Write strobe control pin - must use a pin in the range 0-31
  str = str + ";DC:" + TFT_DC;
#endif
#ifdef TFT_RST      // Write strobe control pin - must use a pin in the range 0-31
  str = str + ";RST:" + TFT_RST;
#endif

#ifdef TFT_WR      // Write strobe control pin - must use a pin in the range 0-31
  // Serial.print(F(" TFT_WR:")); Serial.print(TFT_WR);
  str = str + ";WR:" + TFT_WR;
#endif

#ifdef TFT_RD
  //  Serial.print(F(" TFT_RD:")); Serial.print(TFT_RD);
  str = str + ";RD:" + TFT_RD;
#endif

#ifdef TFT_D0     // Must use pins in the range 0-31 for the data bus
  //  Serial.print(F(" TFT_D0:")); Serial.print(TFT_D0);
  str = str + ";D0:" + TFT_D0;
#endif

#ifdef TFT_D1     // Must use pins in the range 0-31 for the data bus
  //  Serial.print(F(" TFT_D1:")); Serial.print(TFT_D1);
  str = str + ";D1:" + TFT_D1;

#endif

#ifdef TFT_D2     // Must use pins in the range 0-31 for the data bus
  //  Serial.print(F(" TFT_D2:")); Serial.print(TFT_D2);
  str = str + ";D2:" + TFT_D2;
#endif

#ifdef TFT_D3     // Must use pins in the range 0-31 for the data bus
  //  Serial.print(F(" TFT_D3:")); Serial.print(TFT_D3);
  str = str + ";D3:" + TFT_D3;
#endif

#ifdef TFT_D4     // Must use pins in the range 0-31 for the data bus
  // Serial.print(F(" TFT_D4:")); Serial.print(TFT_D4);
  str = str + ";D4:" + TFT_D4;
#endif

#ifdef TFT_D5     // Must use pins in the range 0-31 for the data bus
  // Serial.print(F(" TFT_D5:")); Serial.print(TFT_D5);
  str = str + ";D5:" + TFT_D5;
#endif

#ifdef TFT_D6     // Must use pins in the range 0-31 for the data bus
  // Serial.print(F(" TFT_D6:")); Serial.print(TFT_D6);
  str = str + ";D6:" + TFT_D6;
#endif

#ifdef TFT_D7     // Must use pins in the range 0-31 for the data bus
  // Serial.print(F(" TFT_D7:")); Serial.print(TFT_D7);
  str = str + ";D7:" + TFT_D7;
#endif

#ifdef tft_480_320
  if (str.length() > 40)
  {
    tft.setFreeFont(v6);
    print_define (str.c_str(), -1);
    tft.setFreeFont(v9mono);
  } else
#endif
    print_define (str.c_str(), -1);
  str = "";
#endif //parall




#ifdef tft_400_300
  print_define ((char *)"tft_400_300", -1);
  print_define ((char *)"EPD_BUSY", EPD_BUSY);
  print_define ((char *)"EPD_CS", EPD_CS);
  print_define ((char *)"EPD_RST", EPD_RST);
  print_define ((char *)"EPD_DC", EPD_DC);
  print_define ((char *)"EPD_SCK", EPD_SCK);
  print_define ((char *)"EPD_MISO", EPD_MISO);
  print_define ((char *)"EPD_MOSI", EPD_MOSI);
#endif

#ifdef epd
#ifdef S1epd
  str = str + "S1epd;";
#endif
#ifdef S2epd
  str = str + "S2epd;";
#endif
#ifdef S3epd
  str = str + "S3epd;";
#endif
#ifdef S4epd
  str = str + "S4epd;";
#endif
#ifdef Sfepd
  str = str + "Sfepd;";
#endif
  if (str != "")
    print_define ((char *)str, -1);

#endif //epd





#ifdef only_radio
  print_define ((char *)"only_radio", -1);
#endif


#ifdef pin_sd
  print_define ((char *)"pin_sd", pin_sd);
#endif


#ifdef ota_update
  print_define ((char *)"ota_update", -1);
#endif




#ifdef http_update
  print_define ((char *)"http_update", -1);
#endif




  //-------------------------
#ifdef pin_btn1
  print_define ((char *)"pin_btn1", pin_btn1);
#endif

#ifdef pin_btn2
  print_define ((char *)"pin_btn2", pin_btn2);
#endif

#ifdef pin_btn3
  print_define ((char *)"pin_btn3", pin_btn3);
#endif

#ifdef pin_btn4
  print_define ((char *)"pin_btn4", pin_btn4);
#endif


#ifdef pin_btn_touch
  print_define ((char *)"btn_touch", pin_btn_touch);
#endif

#ifdef pin_ir
  print_define ((char *)"pin_ir", pin_ir);
#endif


#ifdef pin_r433_rx
  print_define ((char *)"pin_r433_rx", pin_r433_rx);
#endif
#ifdef pin_r433_tx
  print_define ((char *)"pin_r433_tx", pin_r433_tx);
#endif

#ifdef nrf
  print_define ((char *)"pin_nrf_CE", pin_nrf_CE);
  print_define ((char *)"pin_nrf_CS", pin_nrf_CS);
  str = "";

#ifdef nrf_read_Tu  //temp_u Темп улица //street temp
  str = "Tu;";
#endif
#ifdef nrf_read_Tk  //temp_u Темп улица //street temp
  str += "Tk;";
#endif
#ifdef nrf_read_Hh  //temp_u Темп улица //street temp
  str += "Hg;";
#endif
#ifdef nrf_read_Pr  //temp_u Темп улица //street temp
  str += "Pr;";
#endif
  str = "NRF_r:" + str;
  print_define (str.c_str(), -1);
  str = "";
  //SSSSSSSSSSSSSSS

#ifdef SEND_tm  //temp_u Темп улица //street temp
  str = str + "tm;";
#endif
#ifdef SEND_tm_sun   //temp_u Темп улица //street temp
  str = str + "sun;";
#endif
#ifdef SEND_moon  //temp_u Темп улица //street temp
  str = str + "moon;";
#endif
#ifdef SEND_sensor_data  //temp_u Темп улица //street temp
  str = str + "sensor;";
#endif
#ifdef SEND_W0
  str = str + "W0;";
#endif
#ifdef SEND_W1
  str = str + "W1;";
#endif
#ifdef SEND_W2
  str = str + "W2;";
#endif
#ifdef SEND_H3
  str = str + "H3;";
#endif
#ifdef SEND_H6
  str = str + "H6;";
#endif
#ifdef SEND_H9
  str = str + "H9;";
#endif
  str = "NRF_s:" + str;
  print_define (str.c_str(), -1);
  str = "";
#endif //nrf



#ifdef gpio_out
  str = "pin_out:";
#ifdef pin_out1
  str = str + "p1:" + String(pin_out1);
#endif
#ifdef pin_out1_color_leds
  str = str + "p1_color:" + String(pin_out1_color_leds);
#endif
#ifdef pin_out2
  str = str + " p2:" + String(pin_out2);
#endif
#ifdef pin_out3
  str = str + " p3:" + String(pin_out3);
#endif
#ifdef pin_out4
  str = str + " p4:" + String(pin_out4);
#endif
#ifdef pin_out5
  str = str + " p5:" + String(pin_out5);
#endif
  print_define (str.c_str(), -1);
  str = "";
#endif //pin_out


  //---------------------WIFI
#if defined(AsyncUDP_lib)&&(defined(wf_send)||defined(wf_read))
  str = "WF Async:";
#else
  str = "";
#endif

#ifdef wf_read
  str = str + "WF read:";
#ifdef wf_read_Tu  //temp_u Темп улица //street temp
  str = str + "Tu;";
#endif
#ifdef wf_read_Tk  //temp_u Темп улица //street temp
  str = str + "Tk;";
#endif
#ifdef wf_read_Hh  //temp_u Темп улица //street temp
  str = str + "Hg;";
#endif
#ifdef wf_read_P  //temp_u Темп улица //street temp
  str = str + "Pr;";
#endif
  //  print_define (str.c_str(), -1);
#endif //wifi

#ifdef wf_send
  str = str + "WF send:";
#ifdef wf_send_Tu  //temp_u Темп улица //street temp
  str = str + "Tu;";
#endif
#ifdef wf_send_Tk  //temp_u Темп улица //street temp
  str = str + "Tk;";
#endif
#ifdef wf_send_Hh  //temp_u Темп улица //street temp
  str = str + "Hg;";
#endif
#ifdef wf_send_P  //temp_u Темп улица //street temp
  str = str + "Pr;";
#endif
  print_define (str.c_str(), -1);
  str = "";
#endif //wifi


#ifdef pin_ds1820
  print_define ((char *)"pin_ds1820", pin_ds1820);
#endif

#ifdef si7021
  print_define ((char *)"si7021", -1);
#endif

#ifdef bme280
  print_define ((char *)"bme280", -1);
#endif


#ifdef pin_dht
  print_define ((char *)"pin_dht", pin_dht);
#endif



#ifdef pin_beep
#ifdef beep_pass
  print_define ((char *)"pin_beep_pass", pin_beep);
#endif
#ifdef beep_act
  print_define ((char *)"pin_beep_act", pin_beep);
#endif

#endif



#ifdef encoder
  str = "enc_CLK "; // + String(CLK);
  str += String(CLK);
  str = str + ";enc_DT " + String(DT) + ";enc_SW " + String(SW) + ";enc_TYPE " + String(TYPE);
  print_define (str.c_str(), -1);
#endif



#if defined (encoder_lib2)
  str = "enc_CLK2 "; // + String(CLK);
  str += String(CLK);
  str = str + ";enc_DT2 " + String(DT) + ";enc_SW2 " + String(SW);
  print_define (str.c_str(), -1);
#endif

#if defined (audio_lib2)
  print_define ((char *)"audio_lib2", -1);
#endif

#if defined (vs1053)
  str = "VS1053 MO:" + String(VS1053_MOSI) + ";MI:" + String(VS1053_MISO) + ";SC:" + String(VS1053_SCK) + \
        ";CS:" + String(VS1053_CS) + ";DCS:" + String(VS1053_DCS) + ";DREQ:" + String(VS1053_DREQ);
  print_define (str.c_str(), -1);
#endif

#ifdef PCM5102
  print_define ((char *)"PCM5102 bclk", bclkPin);
#endif



#ifdef pin_led
  print_define ((char *)"pin_led", pin_led);
#endif

#ifdef pin_pir
  print_define ((char *)"pin_pir", pin_pir);
#endif

#ifdef pin_pir_out_hight
  print_define ((char *)"pir_out_hight", pin_pir_out_hight);
#endif


#ifdef pin_pir_out_low
  print_define ((char *)"pir_out_low", pin_pir_out_low);
#endif

#if defined (fmSi4703)
  str = "Si4703 rst " + String(pin_fm_rst) + ";clk " + String(pin_fm_sclk) + ";io " + String(pin_fm_sdio);
  print_define (str.c_str(), -1);
#endif

#if defined (fmSi4703_lib2)
  str = "Si4703/2 rst " + String(pin_fm_rst) + ";clk " + String(pin_fm_sclk) + ";io " + String(pin_fm_sdio);
  print_define (str.c_str(), -1);
#endif


#if defined (fmRDA5807)
  str = "RDA5807 io " + String(pin_fm_sdio) + ";clk " + String(pin_fm_sclk);
  print_define (str.c_str(), -1);

#endif

#if defined (pin_playWWW_out_hight)
  print_define ((char *)"playWWW_out_hight", pin_playWWW_out_hight);
#endif
#if defined (pin_playFM_out_hight)
  print_define ((char *)"playFM_out_hight", pin_playFM_out_hight);
#endif
#if defined (pin_playMP3_out_hight)
  print_define ((char *)"playMP3_out_hight", pin_playMP3_out_hight);
#endif
#if defined (pin_playALL_out_hight)
  print_define ((char *)"playALL_out_hight", pin_playALL_out_hight);
#endif

#if defined (pin_analog_key1)
  //  print_define ((char *)"analog_key1", pin_analog_key1);
  str = "A_key1";
#ifdef key1btn_1
  str = str + ";1";
#endif
#ifdef key1btn_2
  str = str + ";2";
#endif
#ifdef key1btn_3
  str = str + ";3";
#endif
#ifdef key1btn_4
  str = str + ";4";
#endif
#ifdef key1btn_5
  str = str + ";5";
#endif
  print_define (str.c_str(), pin_analog_key1);
#endif

#if defined (pin_analog_key2)
  str = "A_key2";
#ifdef key2btn_1
  str = str + ";1";
#endif
#ifdef key2btn_2
  str = str + ";2";
#endif
#ifdef key2btn_3
  str = str + ";3";
#endif
#ifdef key2btn_4
  str = str + ";4";
#endif
#ifdef key2btn_5
  str = str + ";5";
#endif
  print_define (str.c_str(), pin_analog_key2);
#endif


#if defined (timechange_summer_winter)
  print_define ((char *)"ch_summer_winter", -1);
#endif


#if defined (lcd_display)
  print_define ((char *)"lcd_display", -1);
#endif

#ifdef aht10
  print_define ((char *)"aht10", aht10);
#endif

#ifdef aht20
  print_define ((char *)"aht20", aht20);
#endif

#ifdef def_th
  print_define ((char *)"def T&&H", -1);
#endif

#ifdef tlg
  print_define ((char *)"telegram", -1);
#endif

#ifdef alert_read
  print_define ((char *)"alert_read", -1);
#endif

#ifdef vcc_read
  print_define ((char *)"vcc_read", -1);
#endif

  //-------------------SCR------
  str = "";
#ifdef S0
  str = str + "S0;";
#endif



#ifdef S1
  str = str + "S1;";
#endif

#ifdef S2
  str = str + "S2;";
#endif

#ifdef S3
  str = str + "S3;";
#endif

#ifdef S4
  str = str + "S4;";
#endif

#ifdef S5
  str = str + "S5;";
#endif


#ifdef S6
  str = str + "S6;";
#endif

#ifdef Sf
  str = str + "Sf;";
#endif



#ifdef Sg
  str = str + "Sg;";
#endif


#ifdef Sn //last
  str = str + "Sn;";
#endif
  if (str != "")
  {
    print_define (str.c_str(), -3);
  }



  //-----ssssssssssssssss----------only---------SERIAL
#if defined (debug) || defined (wf_send)
#ifdef read_remote_sensor_onplay
  // Serial.println(F("read_remote_sensor_onplay"));
  str = F("read_remote_sensor_onplay");
  print_define (str.c_str(), -100);
#endif

  //-----------------hight
#ifdef AUDIO
#ifdef pin_play_WWW_MP3_out_hight
  // Serial.print(F("pin_play_WWW_MP3_out_hight:")); Serial.println(pin_play_WWW_MP3_out_hight);
  str = F("pin_play_WWW_MP3_out_hight:");
  str += String(pin_play_WWW_MP3_out_hight);
  print_define (str.c_str(), -100);
#endif //
#ifdef pin_play_MP3_out_hight
  //  Serial.print(F("pin_play_MP3_out_hight:")); Serial.println(pin_play_MP3_out_hight);
  str = F("pin_play_MP3_out_hight:");
  str += String(pin_play_MP3_out_hight);
  print_define (str.c_str(), -100);
#endif //
#ifdef pin_play_WWW_out_hight
  //  Serial.print(F("pin_play_WWW_out_hight:")); Serial.println(pin_play_WWW_out_hight);
  str = F("pin_play_WWW_out_hight:");
  str += String(pin_play_WWW_out_hight);
  print_define (str.c_str(), -100);
#endif //

#endif //audio

#ifdef pin_playFM_out_hight
  // Serial.print(F("pin_playFM_out_hight:")); Serial.println(pin_playFM_out_hight);
  str = F("pin_playFM_out_hight:");
  str += String(pin_playFM_out_hight);
  print_define (str.c_str(), -100);
#endif //

#ifdef pin_playALL_out_hight
  //  Serial.print(F("pin_playALL_out_hight:")); Serial.println(pin_playALL_out_hight);
  str = F("pin_playALL_out_hight:");
  str += String(pin_playALL_out_hight);
  print_define (str.c_str(), -100);
#endif //
#ifdef pin_night_out_hight
  //  Serial.print("pin_night_out_hight:")); Serial.println(pin_night_out_hight);
  str = F("pin_night_out_hight:");
  str += String(pin_night_out_hight);
  print_define (str.c_str(), -100);
#endif

#ifdef pin_sunset_out_hight
  //  Serial.print("pin_sunset_out_hight:")); Serial.println(pin_sunset_out_hight);
  str = F("pin_sunset_out_hight:");
  str += String(pin_sunset_out_hight);
  print_define (str.c_str(), -100);
#endif


  //-----------------LOW
#ifdef AUDIO
#ifdef pin_play_WWW_MP3_out_low
  //  Serial.print(F("pin_play_WWW_MP3_out_low:")); Serial.println(pin_play_WWW_MP3_out_low);
  str = F("pin_play_WWW_MP3_out_low:");
  str += String(pin_play_WWW_MP3_out_low);
  print_define (str.c_str(), -100);
#endif //
#ifdef pin_play_WWW_out_low
  //  Serial.print(F("pin_play_WWW_out_low:")); Serial.println(pin_play_WWW_out_low);
  str = F("pin_play_WWW_out_low:");
  str += String(pin_play_WWW_out_low);
  print_define (str.c_str(), -100);
#endif //
#ifdef pin_play_MP3_out_low
  //  Serial.print(F("pin_play_MP3_out_low:")); Serial.println(pin_play_MP3_out_low);
  str = F("pin_play_MP3_out_low:");
  str += String(pin_play_MP3_out_low);
  print_define (str.c_str(), -100);
#endif //
#endif //audio

#ifdef pin_playFM_out_low
  //  Serial.print(F("pin_playFM_out_low:")); Serial.println(pin_playFM_out_low);
  str = F("pin_playFM_out_low:");
  str += String(pin_playFM_out_low);
  print_define (str.c_str(), -100);
#endif //
#ifdef pin_playALL_out_low
  //  Serial.print(F("pin_playALL_out_low:")); Serial.println(pin_playALL_out_low);
  str = F("pin_playALL_out_low:");
  str += String(pin_playALL_out_low);
  print_define (str.c_str(), -100);
#endif //
#ifdef pin_night_out_low
  //  Serial.print(F("pin_night_out_low:")); Serial.println(pin_night_out_low);
  str = F("pin_night_out_low:");
  str += String(pin_night_out_low);
  print_define (str.c_str(), -100);
#endif
#ifdef pin_sunset_out_low
  //  Serial.print(F("pin_sunset_out_low:")); Serial.println(pin_sunset_out_low);
  str = F("pin_sunset_out_low:");
  str += String(pin_sunset_out_low);
  print_define (str.c_str(), -100);
#endif

#ifdef    pin_vcc_low_alert
  // Serial.print(F("pin_vcc_low_alert:")); Serial.println(pin_vcc_low_alert);
  str = F("pin_vcc_low_alert:");
  str += String(pin_vcc_low_alert);
  print_define (str.c_str(), -100);
#endif


#ifdef    openw_use_http_select
  //  Serial.println(F("#def openw_use_http_select"));
  str = F("openw_use_http_select:");
  str += String(openw_use_http_select);
  print_define (str.c_str(), -100);
#endif
  str = F("--------end info");
  print_define (str.c_str(), -100);
#endif //debug ||wf_send
}

//------------------------------------------------------------------------------------------
//                              print_define
//------------------------------------------------------------------------------------------
void print_define(const char * n_def, int8_t pin)
{
#ifdef debug
  Serial.print(F("#def "));
  Serial.print(n_def);
#endif

#if defined (wf_send)
  if (send_def_wf)
  {
    log_str = String(n_def);
    if (pin >= 0)
    {
      log_str += ":";
      log_str += String(pin);
    }
    do_send_str_wf(log_str);
    return;
  }
#endif
  if (pin == -100) return; //-100 SERIAL only


#if  !defined(no_tft)
  static byte x = 0;

#ifndef epd
  tft.setTextColor(TFT_LIGHTGREY);
#endif

  if (pin != -3 && strlen(n_def) > 20)  pin = - 2;

  //long string
  if (pin < -1) //end
  {

    if (x > 0)       y_onload += y_step; //new line
    tft.setCursor(0, y_onload);
    tft.print(n_def);
    if (pin == -3) //end
    {
      y_onload += 3;
#ifdef tft_320_240
      tft.drawFastHLine(0, y_onload, 320, TFT_CYAN);
#endif
#ifdef tft_480_320
      tft.drawFastHLine(0, y_onload, 480, TFT_CYAN);
#endif
#ifdef tft_400_300
      tft.drawFastHLine(0, y_onload, 400, TFT_BLACK);
#endif
    }
    y_onload += y_step;
    x = 0;
    Serial.println("");
    return;
  }

  if (x > 0) tft.fillCircle(x - 2, y_onload, 2, TFT_WHITE );

  tft.setCursor(x, y_onload);
  tft.print(n_def);



  if (pin >= 0)
  {
    Serial.print(F(" pin="));
    Serial.println(pin);
#ifndef epd
    tft.setTextColor(TFT_CYAN);
#endif
    tft.print("-");
    tft.println(pin);
  }
  else Serial.println("");




  if (x == 0)
  {
#ifdef tft_320_240
    x = 180;
#endif
#if defined (tft_480_320)||defined (tft_400_300)
    x = 240;
#endif

  }
  else
  {
    y_onload += y_step;
    x = 0;
  }

#endif //no tft
}



//------------------------------------------------------------------------------------------
//                              print_info
//------------------------------------------------------------------------------------------

void print_info(const char * par)
{
#if !defined(no_tft)
  static byte x = 0;
  static byte n = 1;
  if (!send_def_wf)
  {
    if (String(par) == "")
    {
      x = 0;
      n = 1;
      return;
    }
    //  Serial.print("par---->");
    //  Serial.println(par);

#ifdef tft_320_240
    tft.setFreeFont(v6);
#endif
#ifdef tft_480_320
    tft.setFreeFont(v9mono);
#endif
#ifdef tft_400_300
    tft.setFreeFont(v7);
#endif
    tft.setCursor(x, y_onload);

    if (strcmp(par, "ERR WIFI") == 0)
    {
      tft.setTextColor(TFT_RED);
      tft.print(par); tft.print(" file");
      return;
    }


#ifdef pin_sd
    if (strcmp(par, "SD_ERR") == 0)
    {
      tft.setTextColor(TFT_RED);
      tft.print(par);
      return;
    }
    if (strcmp(par, "SD_OK") == 0)
    {
      tft.setTextColor(TFT_GREEN);
      tft.print("SD");
      return;
    }

    if (strcmp(par, "SD") == 0  )
    {
      tft.setTextColor(TFT_GREEN);
      tft.print(par); tft.print(" ");
#ifdef Sf
      tft.setTextColor(TFT_YELLOW); tft.print("FOTO=");
      tft.setTextColor(TFT_CYAN);
      tft.print(max_jpg_num); tft.print(" ");
#endif //sf
#if defined (pin_sd) && defined (ESP32)
      tft.setTextColor(TFT_YELLOW);
      tft.print(" MP3=");
      tft.setTextColor(TFT_CYAN);
      tft.print(max_num_mp3);
#endif //sd esp32

    } //sd


    if (strcmp(par, "NO dir foto") == 0)
    {
      tft.setTextColor(TFT_RED);
      tft.print(par);
    }

    if (strcmp(par, "NO dir mp3") == 0)
    {
      tft.setTextColor(TFT_RED);
      tft.print(par);
    }
#endif //sd

#if defined ESP32 && defined(AUDIO)
    if (strcmp(par, "PL") == 0)
    {
      tft.setTextColor(TFT_YELLOW);
      tft.print("Playlist=");
      tft.setTextColor(TFT_CYAN);
      tft.print(pl_list_max);
    }
#endif //esp32

    if (strcmp(par, "config OK") == 0)
    {
      tft.setTextColor(TFT_GREEN);
      tft.print(par);
    }
    if (strcmp(par, "config.txt ERR") == 0)
    {
      tft.setTextColor(TFT_RED);
      tft.print(par);
    }

#ifdef bme280
    if (strcmp(par, "BME280") == 0)
    {
      if (status_bme) tft.setTextColor(TFT_GREEN); else
        tft.setTextColor(TFT_RED);
      tft.print(par);
    }
#endif

#if defined(aht10)||defined(aht20)
    if (strcmp(par, "AHTxx") == 0)
    {
      if (status_aht) tft.setTextColor(TFT_GREEN); else
        tft.setTextColor(TFT_RED);
      tft.print(par);
    }
#endif

#ifdef nrf
    if (strcmp(par, "NRF24") == 0)
    {
      if (status_nrf) tft.setTextColor(TFT_GREEN); else
        tft.setTextColor(TFT_RED);
      tft.print(par);
    }
#endif


#ifdef pin_r433_rx
    if (strcmp(par, "R433rx") == 0)
    {
      if (status_r433rx) tft.setTextColor(TFT_GREEN); else
        tft.setTextColor(TFT_RED);
      tft.print(par);
    }
#endif

#ifdef pin_r433_tx
    if (strcmp(par, "R433tx") == 0)
    {
      if (status_r433tx) tft.setTextColor(TFT_GREEN); else
        tft.setTextColor(TFT_RED);
      tft.print(par);
    }
#endif


    if (strcmp(par, "NTP") == 0)
    {
      if (sync_t)
      {
        tft.setTextColor(TFT_GREEN);
        tft.print("NTP");
      }
      else
      {
        tft.setTextColor(TFT_RED);
        tft.print("NTP");
      }
    }
  } //not wf_send

  //--------------
  if (strcmp(par, "IP") == 0)
  {
#if defined (wf_send)
    if (send_def_wf)
    {
      log_str = "IP:";
      log_str += WiFi.localIP().toString();
      do_send_str_wf(log_str);
      return;
    }
#endif //wf_send

    tft.setTextColor(TFT_GREEN); tft.print(WiFi.localIP()); tft.print(" ");
    tft.setTextColor(TFT_CYAN); tft.print(String( wifi_rssi())); tft.print(F("%"));
  }

  if (strcmp(par, "tm_ntp") == 0)
  {
    uint32_t t_ = 0;
    if (tm_ntp > 0)
    {
      t_ = millis() - tm_ntp;
      str_millis(&t_); //ret log_str
#if defined (wf_send)
      if (send_def_wf)
      {
        log_str = "NTP sync passed:" + log_str;
        do_send_str_wf(log_str);
        return;
      }
#endif //wf_send
    }

    tft.setTextColor(TFT_WHITE);
    tft.print("NTP sync ");
    if (tm_ntp > 0)
    {
      tft.setTextColor(TFT_CYAN); tft.print(log_str);
    }
  }

  if (strcmp(par, "tm_long") == 0)
  {
    uint32_t t_ = millis();
    str_millis(&t_); //ret log_str
#if defined (wf_send)
    if (send_def_wf)
    {
      log_str = "ESP start passed:" + log_str;
      do_send_str_wf(log_str);
      return;
    }
#endif //wf_send
    tft.setTextColor(TFT_WHITE);
    tft.print("ESP start ");
    tft.setTextColor(TFT_CYAN); tft.print(log_str);
  }


#ifdef openw
  if (strcmp(par, "tm_wh_now") == 0)
  {
    uint32_t t_ = 0;
    if (lastConnectionTime_now > 0)
    {
      t_ = millis() - lastConnectionTime_now;
      str_millis(&t_); //ret log_str
#if defined (wf_send)
      if (send_def_wf)
      {
        log_str = "OpenW now passed:" + log_str;
        do_send_str_wf(log_str);
        return;
      }
#endif //wf_send
    }

    tft.setTextColor(TFT_WHITE);
    tft.print("OpenW now ");
    if (lastConnectionTime_now > 0)
    {
      tft.setTextColor(TFT_CYAN); tft.print(log_str);
    }
  }

  if (strcmp(par, "tm_wh_ch") == 0)
  {
    uint32_t t_ = 0;
    if (lastConnectionTime_ch > 0)
    {
      t_ = millis() - lastConnectionTime_ch;
      str_millis(&t_); //ret log_str

#if defined (wf_send)
      if (send_def_wf)
      {
        log_str = "OpenW 4day passed:" + log_str;
        do_send_str_wf(log_str);
        return;
      }
#endif //wf_send
    }
    tft.setTextColor(TFT_WHITE);
    tft.print("OpenW 4d ");
    if (lastConnectionTime_ch > 0)
    {
      tft.setTextColor(TFT_CYAN); tft.print(log_str);
    }
  }
#endif //openw

  if (strcmp(par, "mem") == 0)
  {
#if defined (wf_send)
    if (send_def_wf)
    {
      log_str = "MEM:";
      log_str += String( ESP.getFreeHeap());
      do_send_str_wf(log_str);
      return;
    }
#endif //wf_send


    tft.setTextColor(TFT_GREEN);
    tft.print(F("MEM:"));
    tft.setTextColor(TFT_CYAN); tft.print(ESP.getFreeHeap());
  }

  //------------------------------------------------------
  n++;
  if (n % 2 == 0)
  {
#ifdef tft_320_240
    x = 180;
#endif
#if defined (tft_480_320)||defined (tft_400_300)
    x = 240;
#endif
  }
  else
  {
    y_onload += y_step;
    x = 0;
  }
#endif
}

//------------------------------------------------------------------------------------------
//                              str_millis
//------------------------------------------------------------------------------------------
void str_millis(uint32_t *tt)
{
  log_str = "";
  int mls = *tt / 1000;
  int tmp = 0;
  tmp = mls / 60 / 60;

  log_str = log_str +  String(str_num(tmp));
  tmp = mls / 60 % 60;
  log_str = log_str + ":"  + String(str_num(tmp));
  tmp = mls  % 60;
  log_str = log_str + ":"  +  String(str_num(tmp));

}


//------------------------------------------------------------------------------------------
//                              str_num
//------------------------------------------------------------------------------------------
const char* str_num(byte num)
{
  static mString<5> str;
  str.clear();
  // Serial.println(num);
  if (num < 10)
  {
    str = "0";
    str += num;
  }
  else
    str = num;


  return str.buf;

}
