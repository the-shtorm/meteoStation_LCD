#if defined(audio_lib2)||defined(vs1053)
void   do_play_beep(String &filename)
{
  if (play || play_mp3) return; //not support
  mp3_eof = false;

  // String track_name_str = "";
  //const char * track="/";
  // if (id == 0) track = "/beep.mp3";
  // if (id == 1) track = "/timer.mp3";
  String  track = "/" + filename;
#ifdef debug
  Serial.print(F("play:")); Serial.println(track);
#endif
  do_pin_on_play(2);

  int   volume_mp3_tmp = beep_mp3_vol;
  volume_mp3_tmp = map(beep_mp3_vol, 0, 100, 0, 21); //21 max


#ifdef def_core0
#ifdef debug
  Serial.println(F("core0 beep"));
#endif
  //  play_mp3 = true; //OFF delay on core0
  audioSetVolume(volume_mp3_tmp);
  play_beep = true;
  playing = audioConnecttoFS(track.c_str(), 0); //play
  return; //in loop=play_beep
#endif //c0


#ifndef def_core0
#ifdef debug
  Serial.println(F("beep lib2 or vs1053"));
#endif
  audio.setVolume(volume_mp3_tmp);
#ifdef pin_sd
  playing = audio.connecttoSD(track.c_str());
#else
  playing = audio.connecttoFS(SPIFFS, track);
#endif

#if defined(vs1053)
  uint32_t sz = audio.getFileSize();
  while (audio.getFilePos() < sz)
  {
    audio.loop();
  }
  audio.stop_mp3client();
#endif //vs

#if defined(audio_lib2)
  while (audio.isRunning())
  {
    audio.loop();
  }
  audio.stopSong();
#endif //ib

#endif //nC0

  do_pin_off_play(2);
  mp3_eof = false;
  play_song = ""; //mqtt
  new_song = false;
}

#endif //lib vs



#if defined(audio_lib2)

//-------------------------------------------------------------------------------------
//                           audio_showstreamtitle
//-------------------------------------------------------------------------------------

void audio_showstreamtitle(const char *info) {

  if (strlen(info))
  {
    String sinfo = info;
    sinfo.replace(" | ", "\n");
    sinfo.replace("| ", "\n");
    sinfo.replace("|", "\n");
    sinfo.trim();
    //   sinfo = sinfo.substring(0, 60);
    sinfo[80] = '\0';
#ifdef debug
    Serial.print(F("audio_showstreamtitle - "));
    Serial.println(info);
#endif
    if (sinfo != play_song)
    {
      play_song = sinfo;
      new_song = true;
    }
  }


}

/*
  //-------------------------------------------------------------------------------------
  //                           audio_info
  //-------------------------------------------------------------------------------------
  #ifdef debug
  void audio_info(const char *info) {
  Serial.print(F("audio_info: ")); Serial.println(info);
  }
  #endif
*/
//-------------------------------------------------------------------------------------
//                            audio_id3data  29 40 err
//-------------------------------------------------------------------------------------
#ifdef pin_sd
void audio_id3data(const char *info) { //id3 metadata

  if (!play_mp3) return;
#ifdef debug
  Serial.print("id3data     "); Serial.println(info);
#endif
  String name = "";
  String str = "";
  // static String sng = "";
  String sinfo = String(info);
  int tz = sinfo.indexOf(":") ;
  if (tz == -1) return;
  name = sinfo.substring(0, tz + 1);
  str = sinfo.substring(tz + 1);
  str.trim();
  name.trim();

#ifdef debug
  Serial.print(F("name     ")); Serial.println(name);
  Serial.print(F("str     ")); Serial.println(str);
#endif
  if (name == "Year:") //song
  {
    play_year_mp3 =  str;
    //    Serial.print("Year     "); Serial.println(str);
  }
  else if (name == "Artist:") //song
  {
    play_song_g_mp3 =  str;
    //    Serial.print("Artist     "); Serial.println(str);

  }
  else if (name == "Album:") //song
  {
    play_album_mp3 =  str;
    //    Serial.print("Album     "); Serial.println(str);

  }
  else

    if (name == "Title:") //song
    {
      play_song =  str;
    }

  new_song = true;


}

#endif
//-------------------------------------------------------------------------------------
//                            void audio_eof_mp3
//-------------------------------------------------------------------------------------

void audio_eof_mp3(const char *info) { //end of file
  playing = false;
#ifdef debug
  Serial.print(F("eof_mp3 ---------  ")); Serial.println(info);
#endif
#if defined(def_core0)
  mp3_eof = true;
#else
  if (play_mp3)
  {
    fn_cmd = "next";
    do_fn_btn_ir();
    draw_num(0, 'C');
  }
#endif
}

/*
  #ifdef debug
  void audio_showstation(const char *info) {
  Serial.print(F("showstation     ")); Serial.println(info);
  }

  void audio_lasthost(const char *info) { //stream URL played
  Serial.print(F("lasthost    ")); Serial.println(info);
  }

  #endif //db
*/


#endif // audio_lib2
