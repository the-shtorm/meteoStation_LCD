
#ifdef tlg

//==========================================================================================================
//                        newMsg
//==========================================================================================================


void newMsg(FB_msg& msg) {
  static uint32_t boot = 0;
  log_str = "";
#ifdef debug
  Serial.print(F("TLG msg:")); Serial.println(msg.text);
  //  Serial.print("lastBotMsg ");; Serial.println(bot.lastBotMsg());
  //  Serial.print("lastUsrMsg ");; Serial.println(bot.lastUsrMsg());
#endif

  if (boot == 0) //start
  {
    boot = now() - TIMEZONE * SECS_PER_HOUR;
    Serial.print(F("boot TIME:"));    Serial.println(boot);
    Serial.print(F("msg.unix :")); Serial.println(  msg.unix);

  }
  if   (msg.unix < boot - 5) //поравка на разницу лок и тлг времени
  {
    Serial.print(F("msg.unix < t_boot SKIP msg:"));
    Serial.println(msg.text);
    return;
  }
#ifdef menu_main
  if (msg.text == "stop" || msg.text == "/start")
  {
    if (msg.text == "stop") bot.closeMenu();
    bot.deleteMessage(bot.lastBotMsg());
    bot.showMenu(menu_main);
  }
#ifdef ESP32
#ifdef menu_play
  if (msg.text == "play" && !scr_timer)
  {
    bot.closeMenu();
    bot.showMenu(menu_play);
  }
#endif

#ifdef menu_play_mp3
  if (msg.text == "play_mp3" && !scr_timer)
  {
    bot.closeMenu();
    bot.showMenu(menu_play_mp3);
  }
#endif
#endif //32

#ifdef menu_play_fm
  if (msg.text == "play_fm"  && !scr_timer)
  {
    bot.closeMenu();
    bot.showMenu(menu_play_fm);
  }
#endif

  //#ifdef menu_user
  if (msg.text == "user")
  {
    bot.closeMenu();
    bot.showMenu(menu_user);
  }
  //#endif

  bot.deleteMessage(bot.lastUsrMsg());
#endif //main

  if (msg.text == "Tin" )
  {
    if (temp_kv != -100) bot.sendMessage("Tin:" + String(temp_kv));
  }
  else if (msg.text == "Tout" )
  {
    if (temp_u != -100) bot.sendMessage("Tout sensor:" + String(temp_u));
#ifdef openw //!!!!!
    else bot.sendMessage("Tout openw:" + String(temperatura_ch[0]));
#endif
  }
  else if (msg.text == "Pr" )
  {
    if (pr_kv != -100) bot.sendMessage("Pr sensor:" + String(pr_kv));
#ifdef openw //!!!!!
    else bot.sendMessage("Pr openw:" + String(davlenie0));
#endif
  }
#if defined ESP32 && defined(AUDIO)
  else if (msg.text == "plist" && play )
    bot.sendMessage(plist_send);
#endif//32 audio

  else if (msg.text == "vl_dn" )log_str = "vl-";
  else if (msg.text == "vl_up" )log_str = "vl+";

  //---------------
  else
  {

    log_str = msg.text;
  }

  if (log_str != "") do_short_cmd(log_str);
}




#endif //tlg
