#ifdef pin_r433_tx
//=======================================================
//                do_send_r433
//=======================================================
void do_send_r433(String &str_send)
{
  byte str_len = str_send.length() + 1;
  char buf[str_len];
  str_send.toCharArray(buf, str_len);
  Serial.print(F("r433_send:")); Serial.println(buf);
  r433tx.send((uint8_t *)buf, str_len);
  r433tx.waitPacketSent();

}
#endif


#ifdef pin_r433_rx
//=======================================================
//                R433
//=======================================================
void do_read_r433()
{
  uint8_t buff[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buff);
  static uint32_t t_next = 0;
  if (millis() < t_next) return;
  if (r433rx.recv(buff, &buflen)) // Non-blocking
  {
    String strth = (const char*) buff;
    Serial.println(strth);
#ifdef debug
    Serial.print(F("R433rx str =")); Serial.println((char*)buff);
#endif
    log_str = (const char*) buff;
    do_short_cmd(log_str);//   do_parse_str();
    t_next = millis() + 500;
  }

}
#endif //433 


#ifdef vcc_read
//=======================================================
//                draw_vcc
//=======================================================
void draw_vcc()
{
  if (vcc == 0) return;
  vcc_alert = false;
  uint16_t color = 0;
  bool ref = true;
  if (millis() > t_next_read_vcc) ref = false; //долго не обновл.

  if (vcc > vcc_full)
    color = TFT_GREEN;
  else if (vcc > vcc_low)
    color = TFT_YELLOW;
  else if (vcc > 0)
  {
    color = TFT_RED;
    vcc_alert = true;
  }
  else
    color = TFT_BLACK; //==0,no data
  tft.fillTriangle(x1_vcc, y1_vcc, x2_vcc, y2_vcc, x3_vcc, y3_vcc, color);

  if (!ref)  tft.drawTriangle(x1_vcc, y1_vcc, x2_vcc, y2_vcc, x3_vcc, y3_vcc, TFT_RED);


}
#endif //vcc_read


//================================================================
//                        do_alert
//================================================================
void do_alert() //id alert
{
  static uint32_t t_next = 0;
  bool alert_all = false;

  if (millis() < t_next) return;
  if (t % 2 == 0) //on
  {
    for (byte i = 0; i < alert_max; i++)
    {
      if (alert_arr_id[i] > 0)
      {
        draw_alert(i, true);
        alert_arr_id[i] -= 1;
        alert_all = true;
        //      Serial.print(F("i=" )); Serial.println(i);
        //      Serial.print(F("[i]= " )); Serial.println( alert_arr_id[i]);
      }
    }
  }
  else //off
  {
    for (byte i = 0; i < alert_max; i++)
    {
      if (alert_arr_id[i] > 0)
      {
        draw_alert(i, false);
        alert_all = true;
      }
    }


  }


  alert = alert_all;
  if (!alert)
  {
    t_next = 0;
    //    refresh_all = true;
    Serial.println(F("alert FALSE" ));
  }
  else
    t_next = millis()  + 1000;

}

//================================================================
//                        draw_alert
//================================================================
void  draw_alert(byte id, bool draw)
{
#if !defined(no_tft)
  uint16_t color = c_alr_id_def;
#if defined(tft_480_320)
  const  byte h_rect = 32;
  const  int w_rect = 479;
#endif
#if defined(tft_320_240)

  const  byte  h_rect = 24;
  const int w_rect = 319;
#endif


  if (draw)
  {
#ifdef c_alr_id0
    if (id == 0) color = c_alr_id0;
#endif
#ifdef c_alr_id1
    if (id == 1) color = c_alr_id1;
#endif
#ifdef c_alr_id2
    if (id == 2) color = c_alr_id2;
#endif
#ifdef c_alr_id3
    if (id == 3) color = c_alr_id3;
#endif
#ifdef c_alr_id4
    if (id == 4) color = c_alr_id4;
#endif
#ifdef c_alr_id5
    if (id == 5) color = c_alr_id5;
#endif
#ifdef c_alr_id6
    if (id == 6) color = c_alr_id6;
#endif
#ifdef c_alr_id7
    if (id == 7) color = c_alr_id7;
#endif
#ifdef c_alr_id8
    if (id == 8) color = c_alr_id8;
#endif
#ifdef c_alr_id9
    if (id == 9) color = c_alr_id9;
#endif
  }
  else
    color = TFT_BLACK;

  int  y = h_rect * id;
  tft.fillRect(0, y, w_rect, h_rect, color) ;
  if (draw)
  {
    tft.setTextColor(TFT_BLACK);
#ifdef tft_480_320
    tft.setFreeFont(v24);
#endif
#ifdef tft_320_240
    tft.setFreeFont(v18);
#endif

    tft.setCursor(20, y + h_rect);
    tft.print(id);
  }
#endif //no tft
}




//================================================================
//                        ds1820
//================================================================
#ifdef pin_ds1820
void do_read_ds1820()
{
  static uint32_t t_period_ds = 0;

  if (millis() < t_period_ds) return;
  t_period_ds = millis() + 10000; //on ERR



  ds.requestTemperatures();                               // считываем температуру с датчиков, на это требуется 750мс
  int tmp = int(round(ds.getTempCByIndex(0)));

  if (tmp < 60 && tmp > -60)
  {
#if ds1820_place //room
    if (temp_kv != tmp) ref_kv_th = true;
    t_read_tk = millis();
    temp_kv = tmp + delta_t;
#else
    //street
    if (temp_u != tmp) ref_temp_u = true;
    temp_u = tmp + delta_t0;
    period_ulica = millis();
#endif
    t_period_ds = millis() + t_ds_ref * 1000; //5min

#ifdef debug
    Serial.print(F("ds1820=")); Serial.println(tmp);
#endif
  }
  else
  {
#ifdef debug
    Serial.print(F("ds1820 ERR temp_u=")); Serial.println(tmp);
#endif
  }

}
#endif



//================================================================
//                    bme280
//================================================================
#ifdef bme280
void do_read_bme()
{
  if (!status_bme) return;
  static time_t t_period_bme = 0;

  if (millis() < t_period_bme && temp_kv != -100) return;
  static time_t t_err = 0;
  if (millis() < t_err) return;
  t_err = millis() + t_bme_err * 1000;




  //float temp(NAN), hum(NAN), pres(NAN);
  float temp, hum, pres;

  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_hPa);
  sensor_bme.read(pres, temp, hum, tempUnit, presUnit);


#ifdef debug
  Serial.println(F("---BME"));
  Serial.println(pres);
  Serial.println(temp);
  Serial.println(hum);
#endif
  temp = round(temp) + delta_t;
  hum = round(hum);

  if (pres > 10000 || pres < 500 || isnan(pres))
  {
    Serial.println("bme280 ERR P=" + String(pres));
    //   pr_kv = 200; //err
  }
  else
  {
#ifndef hPa
    pres = round(pres * 0.75);
#endif
    pr_kv = pres + delta_p;
  }

  if (temp < -50 || temp > 70 || isnan(temp))
  {
#ifdef debug
    Serial.print("bme280 ERR temp_kv="); Serial.println(temp);
#endif
  }
  else
  {
    temp_kv = temp;
    ref_kv_th = true;
    t_read_tk = millis();
    t_period_bme = millis() + t_bme_ref * 1000; //5min
  }
#ifndef nrf_hum_out_TO_hum_in
  if (hum < 1 || hum > 100 || isnan(hum))
  {

#ifdef debug
    Serial.println("bme280 ERR h_kv=" + String(hum));
#endif
    //    h_kv = 200;
  }
  else
    h_kv = hum;
#endif





}
#endif


//================================================================
//                    AHTxx
//================================================================
#if defined (aht10) || defined (aht20)
void  do_read_aht()
{
  static time_t t_period_bme = 0;

  if (millis() < t_period_bme && temp_kv != -100) return;

  static time_t t_err = 0;
  if (millis() < t_err) return;
  t_err = millis() + t_bme_err * 1000;

  float temp = sensor_aht.readTemperature();
  float hum = sensor_aht.readHumidity();
  temp = round(temp) + delta_t;
  hum = round(hum);

#ifdef debug
  Serial.println(F("---AHT"));
  Serial.println(temp);
  Serial.println(hum);
#endif debug
  if (temp < -50 || temp > 70)
  {
#ifdef debug
    Serial.println("AHTxx ERR temp_kv=" + String(temp));
#endif
    //    temp_kv = 200;
  }
  else
  {
    temp_kv = temp;
    ref_kv_th = true;
    t_read_tk = millis();
    t_period_bme = millis() + t_bme_ref * 1000; //5min
  }

#ifndef nrf_hum_out_TO_hum_in
  if (hum < 1 || hum > 100)
  {
#ifdef debug
    Serial.println("AHTxx ERR H=" + String(hum));
#endif
    //    h_kv = 200;
  }
  else
    h_kv = hum;

#endif


}
#endif



//================================================================
//                        si7021
//================================================================
#ifdef si7021
void do_read_si7021()
{
  static time_t t_period_bme = 0;

  if (millis() < t_period_bme && temp_kv != -100) return;

  static time_t t_err = 0;
  if (millis() < t_err) return;
  t_err = millis() + t_bme_err * 1000;


  float hum = sensor_si.getRH();
  //float temp = (sensor_si.getTempF()-32)/1.8;
  float temp = sensor_si.getTemp();
  temp = round(temp) + delta_t;
  hum = round(hum);

#ifdef debug
  Serial.println(F("---SI7021"));
  Serial.println(temp);
  Serial.println(hum);
#endif debug


  if (temp < -70 || temp > 70)
  {
#ifdef debug
    Serial.println("si7021 ERR temp_kv=" + String(temp));
#endif
    //    temp_kv = 200;
  }
  else
  {
    temp_kv = temp;
    ref_kv_th = true;
    t_read_tk = millis();
    t_period_bme = millis() + t_bme_ref * 1000; //5min
  }

#ifndef nrf_hum_out_TO_hum_in
  if (hum < 1 || hum > 100)
  {
#ifdef debug
    Serial.println("si7021 ERR H=" + String(hum));
#endif
    //    h_kv = 200;
  }
  else
    h_kv = hum;
#endif


  //    sensor_si.reset();


}
#endif





//================================================================
//                        DHT
//================================================================
#ifdef pin_dht
void do_read_dht()
{
  static time_t t_period_bme = 0;

  if (millis() < t_period_bme && temp_kv != -100) return; //5min

  static time_t t_err = 0;
  if (millis() < t_err) return;
  t_err = millis() + t_bme_err * 1000;

  float hum = dht.readHumidity();
  float temp = dht.readTemperature();
  temp = round(temp) + delta_t;
  hum = round(hum);

#ifdef debug
  Serial.println(F("---DHT"));
  Serial.println(temp);
  Serial.println(hum);
#endif debug


  if (temp < -70 || temp > 70)
  {
#ifdef debug
    Serial.println( "DHT  ERR temp_kv=" + String(temp));
#endif
    //    temp_kv = 200;
  }
  else
  {
    temp_kv = temp;
    ref_kv_th = true;
    t_read_tk = millis();
    t_period_bme = millis() + t_bme_ref * 1000; //5min
  }

#ifndef nrf_hum_out_TO_hum_in
  if (hum < 1 || hum > 100)
  {
#ifdef debug
    Serial.println( "DHT ERR H=" + String(hum));
#endif
    //   h_kv = 200;
  }
  else
    h_kv = hum;

#endif


}
#endif
