#ifdef mqtt //PubSubClient
#ifdef mqtt_send
//------------------------------------------------------------------------
//                          do_send_mqtt_pin_out()
//------------------------------------------------------------------------
#if defined(gpio_out)

#if defined(pin_out1)&& defined(pin_out1_color_leds)//color led
void do_send_mqtt_pin_out_color(uint8_t pin, uint8_t val)
{
  bool ret;
  if (mqtt_out1_topic_send_color != "" && pin == pin_out1 )
  {
    ret = client_mqtt.publish(mqtt_out1_topic_send_color.c_str(), String(val).c_str(), mqtt_retain);
#ifdef debug
    Serial.print(F("out1 color send:")); Serial.print(val);
    if (!ret) Serial.println(" ERR"); else  Serial.println(" Ok");
#endif
  }
}
#endif //color

void do_send_mqtt_pin_out(uint8_t pin, uint8_t val)
{
  bool ret = false;
  if (!client_mqtt.connected()) return;
  //out1
#if defined(pin_out1)
  if (mqtt_out1_topic_send != "" && pin == pin_out1 )
  {
    ret = client_mqtt.publish(mqtt_out1_topic_send.c_str(), String(val).c_str(), mqtt_retain);
#ifdef debug
    Serial.print(F("out1 send:")); Serial.print(val);
    if (!ret) Serial.println(" ERR"); else  Serial.println(" Ok");
#endif
  }
#endif //out1



  //out2
#if defined(pin_out2)
  if (mqtt_out2_topic_send != "" && pin == pin_out2 )
  {
    ret = client_mqtt.publish(mqtt_out2_topic_send.c_str(), String(val).c_str(), mqtt_retain);
#ifdef debug
    Serial.print(F("out2 send:")); Serial.print(val);
    if (!ret) Serial.println(" ERR"); else  Serial.println(" Ok");
#endif
  }
#endif //out2

  //out3
#if defined(pin_out3)
  if (mqtt_out3_topic_send != "" && pin == pin_out3 )
  {
    ret = client_mqtt.publish(mqtt_out3_topic_send.c_str(), String(val).c_str(), mqtt_retain);
#ifdef debug
    Serial.print(F("out3 send:")); Serial.print(val);
    if (!ret) Serial.println(" ERR"); else  Serial.println(" Ok");
#endif
  }
#endif //out3

  //out4
#if defined(pin_out4)
  if (mqtt_out4_topic_send != "" && pin == pin_out4 )
  {
    ret = client_mqtt.publish(mqtt_out4_topic_send.c_str(), String(val).c_str(), mqtt_retain);
#ifdef debug
    Serial.print(F("out4 send:")); Serial.print(val);
    if (!ret) Serial.println(" ERR"); else  Serial.println(" Ok");
#endif
  }
#endif //out4


  //out5
#if defined(pin_out5)
  if (mqtt_out5_topic_send != "" && pin == pin_out5 )
  {
    ret = client_mqtt.publish(mqtt_out5_topic_send.c_str(), String(val).c_str(), mqtt_retain);
#ifdef debug
    Serial.print(F("out5 send:")); Serial.print(val);
    if (!ret) Serial.println(" ERR"); else  Serial.println(" Ok");
#endif
  }
#endif //out5
}
#endif //gpio out



//------------------------------------------------------------------------
//                          do_send_song_mqtt()
//------------------------------------------------------------------------
#ifdef AUDIO
void do_send_song_mqtt(char ch)
{
  uint16_t ret;

#ifdef debug
  Serial.println(F("---MQTT_S"));
#endif

  if (ch == 's' && mqtt_song_topic_send.length() > 1)
  {
    ret = client_mqtt.publish(mqtt_song_topic_send.c_str(), play_song.c_str(), mqtt_retain);
#ifdef debug
    Serial.print(F("send song:")); Serial.print(play_song);
    Serial.print(" ret:"); Serial.println(ret);
#endif
    //    return; //only new song
  }

  if (ch == 'p' && mqtt_plist_topic_send.length() > 1)
  {
    ret = client_mqtt.publish(mqtt_plist_topic_send.c_str(), plist_send.c_str(), mqtt_retain);
#ifdef debug
    Serial.print(F("send Plist:")); Serial.print(plist_send);
    Serial.print(" ret:"); Serial.println(ret);
#endif
    //    return; //only new song
  }

}
#endif //audio



//------------------------------------------------------------------------
//                          do_send_mqtt()
//------------------------------------------------------------------------

void do_send_mqtt()
{
  static uint32_t t_next = 0;
  bool ret = false;

  if (millis() < t_next) return;

#ifdef debug
  Serial.println(F("---mqtt--send--"));
#endif


  // периодич. отправка для контроля текущих знач.
#if defined(pin_out1)
  do_send_mqtt_pin_out(pin_out1, po1_val);
#if defined(pin_out1_color_leds)//color led
  do_send_mqtt_pin_out_color(pin_out1, po1_val2);
#endif
#endif //out1
#if defined(pin_out2)
  do_send_mqtt_pin_out(pin_out2, po2_val);
#endif //out2
#if defined(pin_out3)
  do_send_mqtt_pin_out(pin_out3, po3_val);
#endif //out3
#if defined(pin_out4)
  do_send_mqtt_pin_out(pin_out4, po4_val);
#endif //out4
#if defined(pin_out5)
  do_send_mqtt_pin_out(pin_out5, po5_val);
#endif //out5

  if (play_bg) return; //дальше тормоза
  //TTTTTTTTTTTTTTTTTTTT
  if (mqtt_T1_topic_send != "" && temp_kv != -100)
  {
    ret = client_mqtt.publish(mqtt_T1_topic_send.c_str(), String(temp_kv).c_str(), mqtt_retain);
#ifdef debug
    Serial.print(F("temp_kv=")); Serial.print(temp_kv);
    if (!ret) Serial.println(" ERR"); else  Serial.println(" Ok");
#endif
  }

  //hhhhhhhhhhhhhhhhhhh
  if (mqtt_H_topic_send != "" && h_kv != -100)
  {
    ret = client_mqtt.publish(mqtt_H_topic_send.c_str(), String(h_kv).c_str(), mqtt_retain);
#ifdef debug
    Serial.print(F("h_kv="));
    Serial.print(h_kv);
    if (!ret) Serial.println(" ERR"); else  Serial.println(" Ok");
#endif


  }

  //Tu-----------
  if (mqtt_T0_topic_send != "" && temp_u != -100)
  {
    ret = client_mqtt.publish(mqtt_T0_topic_send.c_str(), String(temp_u).c_str(), mqtt_retain);
#ifdef debug
    Serial.print(F("temp_u=")); Serial.print(temp_u);
    if (!ret) Serial.println(" ERR"); else  Serial.println(" Ok");
#endif
  }

  //ppppppppppppppppppppppppppppp  давление(pressure)
  int P = -100;
#ifdef openw
  P = davlenie0;
#endif

#ifdef bme280
  P = pr_kv;
#endif

  if (mqtt_P_topic_send != "" && (P != -100))
  {
    ret = client_mqtt.publish(mqtt_P_topic_send.c_str(), String(P).c_str(), mqtt_retain);
#ifdef debug
    Serial.print(F("P=")); Serial.print(P);
    if (!ret) Serial.println(" ERR"); else  Serial.println(" Ok");
#endif
  }


  t_next = millis() + mqtt_period_send * 1000L;

#ifdef debug
  Serial.println(F("send--end---"));
#endif

}
#endif //send

//---------------------------------------------------------------------------------
//                                        callback_mqtt
//---------------------------------------------------------------------------------
void callback_mqtt(char* topic, byte* payload, unsigned int length)
{
#ifdef mqtt_read

  if (length == 0) return;
  char buf[20];
  strlcpy(buf, (char*)payload, length + 1);


#ifdef debug
  Serial.print(F("---MQTT_R /#:"));
  Serial.print(topic); Serial.print(":"); Serial.println(buf);
#endif

  //------------CMD
  if (mqtt_CMD_topic_read == String(topic))
  {
#ifdef debug
    Serial.print(F("mqtt_CMD_topic_read:"));   Serial.println(buf);
#endif
    log_str = String(buf) ;
    if (strncmp(buf, "-out", 4) == 0 || strncmp(buf, "-led", 4) == 0) //
    {
      log_str[0] = '=';
      do_parse_str(log_str) ;
    }
    else
      do_short_cmd(log_str);
    return;
  }


#ifndef read_remote_sensor_onplay
  if (play) return;
#endif




  int pay_int = atol(buf) ;

  //                Read_mqtt_pin_out
#if defined(gpio_out)
  //out1
#if defined(pin_out1)
  //color
#ifdef pin_out1_color_leds //color led
  if (mqtt_out1_topic_read_color == String(topic) )
  {
    log_str = "=out1::";
    log_str += buf;
#ifdef debug
    Serial.print(F("out1 color read:")); Serial.print(log_str);
#endif
    do_parse_str(log_str) ;
    return;
  }
  else if (mqtt_out1_topic_read == String(topic) ) //яркость
  {
    log_str = "=out1:";
    log_str += buf;
    do_parse_str(log_str) ;
#ifdef debug
    Serial.print(F("out1 read:")); Serial.print(log_str);
#endif
    return;
  }
#else //E color

  if (mqtt_out1_topic_read == String(topic) )
  {
    gpio_out_set_val(pin_out1, pay_int );
#ifdef debug
    Serial.print(F("out1 read:")); Serial.print(pay_int);
#endif
    return;
  }
#endif //color
#endif //out1


  //out2
#if defined(pin_out2)
  if (mqtt_out2_topic_read == String(topic) )
  {
    gpio_out_set_val(pin_out2, pay_int );
#ifdef debug
    Serial.print(F("out2 read:")); Serial.print(pay_int);
#endif
    return;
  }
#endif //out2

  //out3
#if defined(pin_out3)
  if (mqtt_out3_topic_read == String(topic) )
  {
    gpio_out_set_val(pin_out3, pay_int );
#ifdef debug
    Serial.print(F("out3 read:")); Serial.print(pay_int);
#endif
    return;
  }
#endif //out3


  //out4
#if defined(pin_out4)
  if (mqtt_out4_topic_read == String(topic) )
  {
    gpio_out_set_val(pin_out4, pay_int );
#ifdef debug
    Serial.print(F("out4 read:")); Serial.print(pay_int);
#endif
    return;
  }
#endif //out4


  //out5
#if defined(pin_out5)
  if (mqtt_out5_topic_read == String(topic) )
  {
    gpio_out_set_val(pin_out5, pay_int );
#ifdef debug
    Serial.print(F("out5 read:")); Serial.print(pay_int);
#endif
    return;
  }
#endif //out5



#endif //gpio out

  //hhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
  if (mqtt_H_topic_read == String(topic))
  {
    if (h_kv != pay_int )
    {
      h_kv = pay_int ;
      ref_kv_th = true;

#ifdef debug
      Serial.print(F("ref "));    Serial.println(h_kv);
#endif
    }
    t_read_tk = millis();
    do_color();
    return;
  }
  //ttttttttttttttttttttttttttttttt
  if (mqtt_T1_topic_read == String(topic))
  {
    pay_int += delta_t;
    if (temp_kv != pay_int )
    {
      Serial.print(F("temp_kv "));    Serial.println(temp_kv);
      Serial.print(F("pay_int "));    Serial.println(pay_int);
      Serial.print(F("delta_t "));    Serial.println(delta_t);
      temp_kv = pay_int ;
      ref_kv_th = true;
#ifdef debug
      Serial.print(F("ref "));    Serial.println(temp_kv);
#endif
    }
    t_read_tk = millis();
    do_color();
    return;
  }

  //T2
  if (mqtt_T0_topic_read == String(topic))
  {
    pay_int += delta_t0;
    if (temp_u != pay_int)
    {
      Serial.print(F("temp_u "));    Serial.println(temp_u);
      Serial.print(F("pay_int "));    Serial.println(pay_int);
      Serial.print(F("delta_t0 "));    Serial.println(delta_t0);
      temp_u = pay_int;
      ref_temp_u = true;

#ifdef debug
      Serial.print(F("ref ")); Serial.println(temp_u);
#endif

    }
    period_ulica = millis();
    do_color();
    return;
  }

  //ppppppppppppppppppppppppp
  if (mqtt_P_topic_read == String(topic))
  {
    if (pr_kv != pay_int)
    {
      pr_kv = pay_int ;
#ifdef debug
      Serial.print(F("ref "));   Serial.println(pr_kv);
#endif

    }
    do_color();
    return;
  }


#ifdef vcc_read
  //VCC
  if (mqtt_VCC_topic_read == topic)
  {
    vcc = pay_int;
#ifdef debug
    Serial.print(F("ref "));   Serial.println(vcc);
#endif
    t_next_read_vcc = millis() + t_vcc_ref * 1000L;
    draw_vcc();
  }
#endif



#endif  //read

}
//----------------------------------------------------------------------
//                             reconn_mqtt
//----------------------------------------------------------------------
bool reconn_mqtt() {
  bool  conn = false;
  bool ret_sub = false;
  static time_t t_next = 0;
  if (millis() < t_next) return false;

  if (client_mqtt.connected())return true;
  // else if (!new_tick) return false; //pause 3sec
  // if  (play) return false; // on disconn. при игре больше не conn
  Serial.print(F("MQTT connecting "));
  conn = client_mqtt.connect(mqtt_esp_name.c_str(), mqtt_user.c_str(), mqtt_password.c_str());

  if (conn)
  {
#ifdef debug
    Serial.println(F(" OK"));
#endif
#ifdef mqtt_read


    //    ------------------GPIO read OUT(1-5)
#if defined(pin_out1)
    if (mqtt_out1_topic_read != "")
    {
      ret_sub =  client_mqtt.subscribe(mqtt_out1_topic_read.c_str());
#ifdef debug
      Serial.print(mqtt_out1_topic_read + F(" subscribe:"));
      if (ret_sub) Serial.println(F(" Ok")); else  Serial.println(F(" ERR"));
#endif
    }
#ifdef pin_out1_color_leds
    if (mqtt_out1_topic_read_color != "")
    {
      ret_sub =  client_mqtt.subscribe(mqtt_out1_topic_read_color.c_str());
#ifdef debug
      Serial.print(mqtt_out1_topic_read_color + F(" subscribe:"));
      if (ret_sub) Serial.println(F(" Ok")); else  Serial.println(F(" ERR"));

#endif
    }
    if (mqtt_out1_topic_send_color != "")
    {
      ret_sub =  client_mqtt.subscribe(mqtt_out1_topic_send_color.c_str());
#ifdef debug
      Serial.print(mqtt_out1_topic_send_color + F(" subscribe:"));
      if (ret_sub) Serial.println(F(" Ok")); else  Serial.println(F(" ERR"));

#endif
    }
#endif//color
#endif //1
#if defined(pin_out2)
    if (mqtt_out2_topic_read != "")
    {
      ret_sub =  client_mqtt.subscribe(mqtt_out2_topic_read.c_str());
#ifdef debug
      Serial.print(mqtt_out2_topic_read + F(" subscribe:"));
      if (ret_sub) Serial.println(F(" Ok")); else  Serial.println(F(" ERR"));
#endif
    }
#endif
#if defined(pin_out3)
    if (mqtt_out3_topic_read != "")
    {
      ret_sub =  client_mqtt.subscribe(mqtt_out3_topic_read.c_str());
#ifdef debug
      Serial.print(mqtt_out3_topic_read + F(" subscribe:"));
      if (ret_sub) Serial.println(F(" Ok")); else  Serial.println(F(" ERR"));
#endif
    }
#endif
#if defined(pin_out4)
    if (mqtt_out4_topic_read != "")
    {
      ret_sub =  client_mqtt.subscribe(mqtt_out4_topic_read.c_str());
#ifdef debug
      Serial.print(mqtt_out4_topic_read + F(" subscribe:"));
      if (ret_sub) Serial.println(F(" Ok")); else  Serial.println(F(" ERR"));
#endif
    }
#endif
#if defined(pin_out5)
    if (mqtt_out5_topic_read != "")
    {
      ret_sub =  client_mqtt.subscribe(mqtt_out5_topic_read.c_str());
#ifdef debug
      Serial.print(mqtt_out5_topic_read + F(" subscribe:"));
      if (ret_sub) Serial.println(F(" Ok")); else  Serial.println(F(" ERR"));
#endif
    }
#endif
    //----------E GPIO out
    //---------------------CMD
    if (mqtt_CMD_topic_read != "")
    {
      ret_sub =  client_mqtt.subscribe(mqtt_CMD_topic_read.c_str());
#ifdef debug
      Serial.print(mqtt_CMD_topic_read + F(" subscribe:"));
      if (ret_sub) Serial.println(F(" Ok")); else  Serial.println(F(" ERR"));
#endif
    }


    if (mqtt_H_topic_read != "")
    {
      ret_sub =  client_mqtt.subscribe(mqtt_H_topic_read.c_str());
#ifdef debug
      Serial.print(mqtt_H_topic_read + F(" subscribe:"));
      if (ret_sub) Serial.println(F(" Ok")); else  Serial.println(F(" ERR"));
#endif
    }

    if (mqtt_T1_topic_read != "")
    {
      ret_sub =  client_mqtt.subscribe(mqtt_T1_topic_read.c_str());
#ifdef debug
      Serial.print(mqtt_T1_topic_read + F(" subscribe:"));
      if (ret_sub) Serial.println(F(" Ok")); else  Serial.println(F(" ERR"));
#endif
    }


    if (mqtt_T0_topic_read != "")
    {
      ret_sub =  client_mqtt.subscribe(mqtt_T0_topic_read.c_str());
#ifdef debug
      Serial.print(mqtt_T0_topic_read + F(" subscribe:"));
      if (ret_sub) Serial.println(F(" Ok")); else  Serial.println(F(" ERR"));
#endif
    }

    if (mqtt_P_topic_read != "")
    {
      ret_sub =  client_mqtt.subscribe(mqtt_P_topic_read.c_str());
#ifdef debug
      Serial.print(mqtt_P_topic_read + F(" subscribe:"));
      if (ret_sub) Serial.println(F(" Ok")); else  Serial.println(F(" ERR"));
#endif
    }



    if (mqtt_VCC_topic_read != "")
    {
      ret_sub =  client_mqtt.subscribe(mqtt_VCC_topic_read.c_str());
#ifdef debug
      Serial.print(mqtt_VCC_topic_read + F(" subscribe:"));
      if (ret_sub) Serial.println(F(" Ok")); else  Serial.println(F(" ERR"));
#endif
    }

#endif //read 
  }
  else
  {
#ifdef debug
    Serial.println(F(" ERR"));
#endif
    t_next = millis() + t_reconn_mqtt * 1000L;
  }
  return conn;
}



#endif //mqtt
