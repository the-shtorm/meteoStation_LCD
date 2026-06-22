#ifdef wf_send

#if defined(wf_send_gpio)
//============================================================
//                           do_wf_send_gpio()
//============================================================
void do_wf_send_gpio_status()
{

#if defined(pin_out1)&& defined(pin_out1_color_leds)//color led
  do_send_wf_pin_out_color(pin_out1, po1_val2); //color
  do_send_wf_pin_out(pin_out1, po1_val); //bright1
#elif defined(pin_out1)
  do_send_wf_pin_out(pin_out1, po1_val); //bright1
#endif

#if defined(pin_out2)
  do_send_wf_pin_out(pin_out2, po2_val);
#endif
#if defined(pin_out3)
  do_send_wf_pin_out(pin_out3, po3_val);
#endif
#if defined(pin_out4)
  do_send_wf_pin_out(pin_out4, po4_val);
#endif
#if defined(pin_out5)
  do_send_wf_pin_out(pin_out5, po5_val);
#endif
}

#if defined(pin_out1)&& defined(pin_out1_color_leds)//color led
void do_send_wf_pin_out_color(uint8_t pin, uint8_t val)
{
  if (WiFi.status() != WL_CONNECTED) return;
  log_str = "out1Rc:";
  log_str += String(val);
  if (log_str.length() > UDP_PACKET_SIZE) log_str.substring(0, UDP_PACKET_SIZE - 40);
#ifdef debug
  Serial.print(F("---WF_SEND:")); Serial.println(log_str);
#endif
#ifdef AsyncUDP_lib
  udp_wf.broadcast(log_str.c_str());
#else
  char ch[log_str.length() + 1];
  log_str.toCharArray(ch, log_str.length() + 1);
  if (! sendPacket(broadcastAddress, (uint8_t*)&ch, sizeof(ch)))
  {
#ifdef debug
    Serial.print(F("ERR out1_c send"));
#endif
  }
#endif

}
#endif //color

void do_send_wf_pin_out(int8_t pin, uint8_t val)
{
  if (WiFi.status() != WL_CONNECTED) return;
#if defined(pin_out1)
  if (pin == pin_out1 )
  {
    log_str = "out1R:";
    log_str += String(val);
  }
#endif //out1
  //out2
#if defined(pin_out2)
  if (pin == pin_out2 )
  {
    log_str = "out2R:";
    log_str += String(val);
  }
#endif //out2

  //out3
#if defined(pin_out3)
  if (pin == pin_out3 )
  {
    log_str = "out3R:";
    log_str += String(val);
  }
#endif //out3

  //out4
#if defined(pin_out4)
  if ( pin == pin_out4 )
  {
    log_str = "out4R:";
    log_str += String(val);
  }
#endif //out4


  //out5
#if defined(pin_out5)
  if (pin == pin_out5 )
  {
    log_str = "out5R:";
    log_str += String(val);
  }
#endif //out5
#ifdef debug
  Serial.print(F("---WF_SEND:")); Serial.println(log_str);
#endif
  if (log_str.length() > UDP_PACKET_SIZE) log_str.substring(0, UDP_PACKET_SIZE - 40);

#ifdef AsyncUDP_lib
  udp_wf.broadcast(log_str.c_str());
#else
  char ch[log_str.length() + 1];
  log_str.toCharArray(ch, log_str.length() + 1);
  if (! sendPacket(broadcastAddress, (uint8_t*)&ch, sizeof(ch)))
  {
#ifdef debug
    Serial.print(F("ERR out1_c send"));
#endif
  }
#endif
  log_str = "";
}
#endif //gpio out



//------------------------------------------------------------------------
//                          do_send_str_wf
//------------------------------------------------------------------------

//#ifdef AUDIO
void do_send_str_wf(String &str_send)
{
  if (WiFi.status() != WL_CONNECTED ) return;
#ifdef debug
  Serial.print(F("---WF_SEND str:")); Serial.println(str_send);
#endif

  if (str_send.length() > UDP_PACKET_SIZE) str_send.substring(0, UDP_PACKET_SIZE - 40);


#ifdef AsyncUDP_lib
  udp_wf.broadcast(str_send.c_str());
#else
  char ch[str_send.length() + 1];
  str_send.toCharArray(ch, str_send.length() + 1);
  if (! sendPacket(broadcastAddress, (uint8_t*)&ch, sizeof(ch)))
  {
#ifdef debug
    Serial.print(F("ERR"));
#endif
  }
#endif
}
//#endif //audio



//============================================================
//                           do_wf_send()
//============================================================
void do_wf_send()
{
  static uint32_t nextTime = 0;
  static byte loc_count_send = 0;

  if (millis() < nextTime) return;
  log_str = "";

  //tttttttttttttttttttttttttttttttttttttttttttttttttttt
#ifdef wf_send_Tu
  if (temp_u != -100)    log_str = "T0:" + String(temp_u);
#endif
  //tttttttttttttttttttttkkkkkkkkkkkkkkkkkkkkkkkkkkk
#ifdef wf_send_Tk
  if (temp_kv != -100)
  { // temp_kv + delta_t убрал тк уже с поправкой, ксли надо правится локально
    if (log_str != "")  log_str = log_str + ";T1:" + String(temp_kv);
    else
      log_str = "T1:" + String(temp_kv + delta_t);
  }
#endif

  //hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
#ifdef wf_send_Hh
  if (h_kv != -100)
  {
    if (log_str != "")  log_str = log_str + ";H:" + String(h_kv);
    else
      log_str = "H:" + String(h_kv);
  }
#endif

  //pppppppppppppppppppppppppppppp
#ifdef wf_send_P
  int p = pr_kv;
  if (p == -100) p = davlenie0;
  Serial.print(F("---WF_udp p: ")); Serial.println(p);
  if (p != -100)
  {
    if (log_str != "")  log_str = log_str + ";P:" + String(p);
    else
      log_str = "P:" + String(p);
  } ;

#endif

  if (log_str == "") return;

  log_str = "=" + log_str;

#ifdef AsyncUDP_lib
  //    udp_wf.print(ch);
  //   udp_wf.broadcastTo(log_str.c_str(), wf_localPort);
  udp_wf.broadcast(log_str.c_str());

#else
  char ch[log_str.length() + 1];
  log_str.toCharArray(ch, log_str.length() + 1);
  if (! sendPacket(broadcastAddress, (uint8_t*)&ch, sizeof(ch)))
  {
#ifdef debug
    Serial.print(F("ERR udp send"));
#endif
  }
#endif

  loc_count_send++;

#ifdef debug
  Serial.print(F("---WF_udp send: "));
  Serial.println(log_str);
#endif


  if (loc_count_send < wf_count_send || log_str.length() == 0)  nextTime = millis() + 1000;
  else
    nextTime = millis() + wf_period_send * 1000L;

  if (loc_count_send == wf_count_send) loc_count_send = 0;
}


//============================================================
//                           sendPacket()
//============================================================
#ifndef AsyncUDP_lib
bool sendPacket(const IPAddress& address, const uint8_t* buf, uint8_t bufSize) {
  udp_wf.beginPacket(address, wf_localPort);
  udp_wf.write(buf, bufSize);
  return (udp_wf.endPacket() == 1);
}
#endif//!async

#endif//send


#ifdef wf_read
//=======================================================================
//                seek_str
//=======================================================================

int seek_str(char* str, char* ch, uint8_t fromIndex = 0)
{
  uint8_t len = strlen(str);
  if (fromIndex >= len) return -1;
  const char* temp = strstr(str + fromIndex, ch);
  return (temp == NULL) ? -1 : (temp - str);
}

//=======================================================================
//                do_wf_read ASYNC
//=======================================================================
#if defined(ESP32)&&defined(AsyncUDP_lib)
void do_wf_read(AsyncUDPPacket packet)
{
  uint16_t len = packet.length();
  if (len == 0 || len > UDP_PACKET_SIZE )
  {
#ifdef debug
    Serial.println(F("---WIFI_R ERR:packet >UDP_PACKET_SIZE||len==0"));
#endif
    return;
  }
  uint8_t* pdata = (uint8_t*)packet.data();

  Serial.print(F("---WIFI_R:"));  Serial.println((char*)pdata);
  // strcpy(packetBuffer_udp, "");
  strncpy (packetBuffer_udp, (const char*) pdata, len);
  packetBuffer_udp[len] = '\0';
  async_str_wf = "";
  async_str_wf = (const char*)packetBuffer_udp;

  int ind = -1;
#ifndef wf_read_Tu
  ind = async_str_wf.indexOf("T0:", 0);
  if ( ind > 0) async_str_wf[ind] = '?';
#endif

#ifndef wf_read_Tk
  ind = async_str_wf.indexOf("T1:", 0);
  if ( ind > 0) async_str_wf[ind] = '?';
#endif

#ifndef wf_read_Hh
  ind = async_str_wf.indexOf("H:", 0);
  if ( ind > 0) async_str_wf[ind] = '?';
#endif

#ifndef wf_read_Pr
  ind = async_str_wf.indexOf("P:", 0);
  if ( ind > 0) async_str_wf[ind] = '?';
#endif

  //подмена -->do_parse_str
  if (strncmp(async_str_wf.c_str(), "-out", 4) == 0 || strncmp(async_str_wf.c_str(), "-led", 4) == 0)
  {
    async_str_wf[0] = '=';
    //   Serial.println(F("async led repl = -> -"));
  }
#ifdef debug
  Serial.print(F("---WIFI_R Async:"));  Serial.println(async_str_wf);
#endif

  if (async_str_wf[0] == '=')
  {
    Serial.println(F("Async--->do_str_parse"));
    do_parse_str(async_str_wf);
    async_str_wf = "";
  }
  else
  {
    Serial.println(F("Async--->loop"));
  }


}

//=======================================================================
//                do_wf_read NOt async
//=======================================================================
#else
void do_wf_read() //sync
{
  static uint32_t nextTime = 0;
  if (millis() < nextTime) return;

  int packetSize = udp_wf.parsePacket();
  if (!packetSize) return;
  int n = udp_wf.read(packetBuffer_udp, UDP_PACKET_SIZE);
  packetBuffer_udp[n] = '\0';
  log_str = (const char*) packetBuffer_udp;
#ifdef debug
  Serial.print(F("---WIFI_R:"));  Serial.println(log_str);
#endif
  int ind = -1;

#ifndef wf_read_Tu
  ind =  seek_str(packetBuffer_udp, (char*)"T0", 0);
  if (ind > 0) packetBuffer_udp[ind] = '?';
#endif

#ifndef wf_read_Tk
  ind =  seek_str(packetBuffer_udp, (char*)"T1", 0);
  if (ind > 0) packetBuffer_udp[ind] = '?';
#endif

#ifndef wf_read_Hh
  ind =  seek_str(packetBuffer_udp, (char*)(char*)"H:", 0);
  if (ind > 0) packetBuffer_udp[ind] = '?';
#endif

#ifndef wf_read_Pr
  ind =  seek_str(packetBuffer_udp, (char*)"P:", 0);
  if (ind > 0) packetBuffer_udp[ind] = '?';
#endif

  log_str = (const char*) packetBuffer_udp;
  do_short_cmd(log_str);//do_parse_str();
  nextTime = millis() + 200;
}
#endif
#endif //wf_read
