#ifdef gpio_out
//-----------------------------------------------------------------------------
//               gpio_out_set_val
//-----------------------------------------------------------------------------
void gpio_out_set_val(int8_t num, uint8_t val  )
{
#ifdef ESP32
  int8_t led_ch32 = -1;
#endif
#ifdef debug
  Serial.print(F("gpio num/outval: "));
  Serial.print(num); Serial.print("/"); Serial.println(val);
#endif
  if (val > 100) val = 100;

  int  val_tmp = -1;

#if defined (pin_out1)&& !defined (pin_out1_color_leds)
  if (num == pin_out1 && po1_val != val)
  {
    val_tmp = val;
    po1_val = val;
#ifdef ESP32
    led_ch32 = 1;
#endif
    /*
      #ifdef mqtt_send
      do_send_mqtt_pin_out(pin_out1, po1_val);
      #endif
      #ifdef wf_send_gpio
      do_send_wf_pin_out(pin_out1, po1_val);
      #endif
    */
  }
#endif

#if defined (pin_out2)
  if (num == pin_out2 && po2_val != val)
  {
    val_tmp = val;
    po2_val = val;
#ifdef ESP32
    led_ch32 = 2;
#endif
    /*
      #ifdef mqtt_send
        do_send_mqtt_pin_out(pin_out2, po2_val);
      #endif
      #ifdef wf_send_gpio
        do_send_wf_pin_out(pin_out2, po2_val);
      #endif
    */
  }
#endif

#if defined (pin_out3)
  if (num == pin_out3 && po3_val != val)
  {
    val_tmp = val;
    po3_val = val;
#ifdef ESP32
    led_ch32 = 3;
#endif
    /*
      #ifdef mqtt_send
      do_send_mqtt_pin_out(pin_out3, po3_val);
      #endif
      #ifdef wf_send_gpio
      do_send_wf_pin_out(pin_out3, po3_val);
      #endif
    */
  }
#endif

#if defined (pin_out4)
  if (num == pin_out4 && po4_val != val)
  {
    val_tmp = val;
    po4_val = val;
#ifdef ESP32
    led_ch32 = 4;
#endif
    /*
      #ifdef mqtt_send
      do_send_mqtt_pin_out(pin_out4, po4_val);
      #endif
      #ifdef wf_send_gpio
      do_send_wf_pin_out(pin_out4, po4_val);
      #endif
    */
  }
#endif

#if defined (pin_out5)
  if (num == pin_out5 && po5_val != val)
  {
    val_tmp = val;
    po5_val = val;
#ifdef ESP32
    led_ch32 = 5;
#endif
    /*
      #ifdef mqtt_send
      do_send_mqtt_pin_out(pin_out5, po5_val);
      #endif
      #ifdef wf_send_gpio
      do_send_wf_pin_out(pin_out5, po5_val);
      #endif
    */
  }
#endif

  if (val_tmp == -1) return;


#ifdef mqtt_send
  do_send_mqtt_pin_out(num, val);
#endif
#ifdef wf_send_gpio
  if (WiFi.status() == WL_CONNECTED) do_send_wf_pin_out(num, val);
#endif

#ifdef ESP9266
  if (val == 0)
  {
    digitalWrite(num, LOW);
  }
  else if (val == 100)
  {
    digitalWrite(num, HIGH);
  }
  else
#endif //66
  {
    val_tmp = map(val, 0, 100, 0, 1024);
#ifdef ESP32
    ledcWrite(led_ch32, val_tmp);
#else
    analogWrite(num, val_tmp);
#endif
  }
  t_timer_pin_out = millis() + t_period_save_status * 1000UL; //reset timer
#ifdef debug
  Serial.println(F("t_timer_pin_out START"));
#endif
}

//-----------------------------------------------------------------------------
//               gpio_out_timer_control
//-----------------------------------------------------------------------------
void gpio_out_timer_control()
{
  String s_new_val = "";

  if (millis() < t_timer_pin_out) return;
#if defined (pin_out1)
#ifdef pin_out1_color_leds
  s_new_val = F("out1:");
  s_new_val += String(po1_val);
  s_new_val += F(":");
  s_new_val += String(po1_val2);
#else
  s_new_val = F("out1:");
  s_new_val += String(po1_val);
#endif
#endif

#if defined (pin_out2)
  if (s_new_val != "") s_new_val = s_new_val + ";";
  s_new_val = s_new_val + "out2:" + String(po2_val);
#endif

#if defined (pin_out3)
  if (s_new_val != "") s_new_val = s_new_val + ";";
  s_new_val = s_new_val + "out3:" + String(po3_val);
#endif

#if defined (pin_out4)
  if (s_new_val != "") s_new_val = s_new_val + ";";
  s_new_val = s_new_val + "out4:" + String(po4_val);
#endif

#if defined (pin_out5)
  if (s_new_val != "") s_new_val = s_new_val + ";";
  s_new_val = s_new_val + "out5:" + String(po5_val);
#endif
  s_new_val = "=" + s_new_val;
  s_new_val.trim(); s_out_old_val.trim();
  Serial.print("s_new_val "); Serial.println(s_new_val);
  Serial.print("s_out_old_val "); Serial.println(s_out_old_val);
  if (s_new_val != s_out_old_val)
  {
    Serial.println("s_out_old_val!= s_new_val");
    do_fwrite_pin_out(s_new_val); //save gpio
    s_out_old_val = s_new_val;
  }

  t_timer_pin_out = 0;




}



//-----------------------------------------------------------------------------
//               do_fread_pin_out
//-----------------------------------------------------------------------------

void do_fread_pin_out()
{
  File f;

#ifdef debug
  Serial.println("read /p_out.tmp");
#endif
#ifdef pin_sd
  f = SD.open( "/p_out.tmp", FILE_READ);  // or, file handle reference for SD library
#else
  f = SPIFFS.open( "/p_out.tmp", "r");    // File handle reference for SPIFFS
#endif

  if (!f) {
    Serial.println(F("ERR open p_out.tmp"));
    return;
  }
  log_str = f.readStringUntil('\n');
  if (log_str != "")
  {
    s_out_old_val = log_str;
#ifdef debug
    Serial.print(F("s_out_old_val:")); Serial.println(s_out_old_val);
#endif
    do_parse_str(log_str);
  }
  f.close();
}

//-----------------------------------------------------------------------------
//               do_fwrite_pin_out
//-----------------------------------------------------------------------------
void do_fwrite_pin_out(String &str)
{

  //if (log_str == "") return;
  File f;


#ifdef pin_sd
#ifdef ESP32
  fs::FS &fs = SD;
  if (fs.exists("/p_out.tmp")) fs.remove("/p_out.tmp");
  f = fs.open("/p_out.tmp", FILE_APPEND);
#endif //32
#ifdef ESP8266
  if (SD.exists("/p_out.tmp")) SD.remove("/p_out.tmp");
  f = SD.open( "/p_out.tmp", FILE_WRITE);  // or, file handle reference for SD library
#endif //66
#else //pin_sd
#ifdef ESP32
  fs::FS &fs = SPIFFS;
  if (fs.exists("/p_out.tmp")) fs.remove("/p_out.tmp");
  f = fs.open( "/p_out.tmp", "a+");    // File handle reference for SPIFFS
#endif //32
#ifdef ESP8266
  // fs::FS &fs = SPIFFS;
  if (SPIFFS.exists("/p_out.tmp")) SPIFFS.remove("/p_out.tmp");
  f = SPIFFS.open( "/p_out.tmp", "a+");    // File handle reference for SPIFFS
#endif

#endif

  if (!f) {
    Serial.println(F(" ERR open p_out.tmp"));
    return;
  }


#ifdef debug
  Serial.print(F("Save p_out.tmp:")); Serial.println(str);
#endif

  f.println(str);
  f.close();

}


#endif //out
