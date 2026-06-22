
//======================================================================================
//                   control_timer
//======================================================================================

void control_timer() //таймер на остановку на экране проигр.
{
  // static uint32_t t_ref = 0;
  // static byte mnt_old = 99;
  //  static byte scr_n = 99;
  if (millis() < t_timer)   return;


  // only stop
  //  if (millis() >= t_timer) //STOP timer
  //  {
  // t_timer = 0;
  //  t_ref = 0;
  //  scr_n = 99;
  if (play)
  {
    fn_cmd = "stop";
  }
  else if (play_fm)
  {
    fn_cmd = "stop_fm";
  }
  else if (play_mp3)
  {
    fn_cmd = "stop_mp3";
  }

  timer_stop();
#ifdef tlg
  if (WiFi.status() == WL_CONNECTED)
  {
    String str = F("TIMER ");
    str += fn_cmd;
    bot.sendMessage(str);
  }
#endif

  do_fn_btn_ir();

  //  }


}


#if defined (ESP32) && defined(AUDIO)

//=====================================================================
//                            timer_start
//=====================================================================
void timer_start(int minute_count, byte id)
{

  t_timer = millis() + 60000 * minute_count;
  timer_id = id;
  draw_num(minute_count, 'T');
  draw_num(0, 'C'); //clear num
#ifdef debug
  Serial.print(F("t_timer  start Id:")); Serial.print(id);
  Serial.print(F(" min:")); Serial.println(minute_count);
#endif
#ifdef tlg
  if (WiFi.status() == WL_CONNECTED)
  {
    bot.sendMessage(F("TIMER START"));
  }
#endif
}

#endif //32

//=====================================================================
//                            timer_stop
//=====================================================================
void timer_stop()
{
  t_timer = 0;
  timer_id = 99;
  draw_num(0, 'T');
#ifdef debug
  Serial.println(F("t_timer  STOP"));
#endif
}
