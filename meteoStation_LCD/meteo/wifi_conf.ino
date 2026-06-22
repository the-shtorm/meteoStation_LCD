//==================================================================
//                        wifiConnect
//==================================================================

void wifiConnect()
{

  static bool conn = false; //при повторных на экран не показываю
  static uint8_t wifi_count_recon = 0; //счетчик reconn;
  static uint32_t t_next = 0;
  String str = "";
  bool file_wifi = false;//open file
  bool file_multi = false;
  byte count_ssid = 0;

  if ( millis() < t_next) return;
  Serial.println("-----------------------wifiConnect()");
  serial_time_start();
  WiFi.persistent(false);

#ifdef plotter
  do_plotter('W');
#endif
#ifdef debug
  Serial.println(F("WIFI disconnect"));
#endif

  WiFi.disconnect(true);
  delay(500);
  const char * _ssid = "", *_pass = "";
  String filename = "/wifi";
#ifdef wifi_multi
  const byte count_file = 2; //data/wifi+multiwf.txt
#else
  const byte count_file = 1; //data/wifi
#endif

  if (!conn || count_file == 1) //ESP boot or !multi
  {

    for (int i = 1; i <= count_file; i++) {
      if (i == 1) filename = "/wifi";
#ifdef wifi_multi
      if (i == 2) filename = "/multiWF.txt";
#endif

#ifdef pin_sd
      Serial.print(F("SD "));
      Serial.println(filename);
      File  configFile = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
#else
      Serial.print(F("Spiffs "));
      Serial.println(filename);
      fs::File   configFile = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
#endif

      if (configFile) {
        //#ifdef wifi_multi
        while (configFile.available()) {
          //#endif
          str = configFile.readStringUntil('\n');
          //   Serial.print("str");
          //      Serial.println(str);
          str.trim();
          if (str[0] == '/') continue;
          if (str.length() > 0)
          {
            byte ind = str.indexOf(";");
            String s = str.substring(0, ind);
            byte ind2 = str.indexOf(";", ind + 1);
            String p = str.substring(ind + 1, ind2);
            _ssid = s.c_str();
            _pass = p.c_str();
            Serial.print(F("Add ssid/passw ")); Serial.print(_ssid);
            Serial.print(F("/")); Serial.println(_pass);
#ifdef wifi_multi
            wifiMulti.addAP(_ssid, _pass);
#else
            WiFi.begin(_ssid, _pass);
            //          Serial.println(F(" WiFi.begin"));
#endif
            count_ssid++;
          }
        }

        // if (i == 1) file_wifi = true;
        // if (i == 2) file_multi = true;
        configFile.close();
      }
      else
        Serial.print(F("ERR open ")); Serial.println(filename);
    } //for

    //  if (!file_wifi && !file_multi)
    if (count_ssid == 0)
      print_info("ERR WIFI");

  } //!conn



#if !defined(no_tft)
  if (!conn) //ESP boot
  {

#ifdef tft_320_240
    tft.setCursor(0, 239);
#endif
#ifdef tft_480_320
    tft.setCursor(0, 329);
#endif
#ifdef tft_400_300
    tft.setCursor(0, 299);
#endif
  }
#endif //no tft

  unsigned long startTime = millis();
  //------------------------------------------
#ifdef wifi_multi
  while (wifiMulti.run() != WL_CONNECTED)
#else
  while (WiFi.status() != WL_CONNECTED)
#endif
  {
#if !defined(no_tft)
    if (!conn)
    {
      tft.print("=");
    }
#endif
    Serial.print('.');
    if ((unsigned long)(millis() - startTime) >= wifi_rep_t * count_ssid) break;
    delay(200);
  }

#ifdef wifi_multi
  if (wifiMulti.run() == WL_CONNECTED)
#else
  if (WiFi.status() == WL_CONNECTED)
#endif
  {
#ifdef debug
    Serial.printf(" connected to %s\n", WiFi.SSID().c_str());
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
#endif
    if (!conn)
    {
#if defined (tlg)
#if defined (menu_main)
      bot.showMenu(menu_main);
#endif
      bot.sendMessage(String(ota_name) + " ESP IP:" + WiFi.localIP().toString());
#endif// tlg

#if defined (mqtt) || defined(mqtt2)|| defined(mqtt3)
      String esp_ip = WiFi.localIP().toString();
      byte n = esp_ip.lastIndexOf('.');
      esp_ip = esp_ip.substring(n + 1);
      mqtt_esp_name = mqtt_esp_name  + "-IP-" + esp_ip;
      Serial.print(F("mqtt_esp_name: "));
      Serial.println(mqtt_esp_name);
#if defined (mqtt) || defined(mqtt2)
      reconn_mqtt();
#endif
#ifdef mqtt3
      client_mqtt.setClientId(mqtt_esp_name.c_str());
      client_mqtt.setServer(mqtt_server.c_str(), mqtt_port);
      client_mqtt.setCredentials(mqtt_user.c_str(), mqtt_password.c_str());
#endif //mqtt

#endif //mqtt
    }
#ifdef mqtt3
    connectToMqtt();
#endif
    conn = true;
    wifi_count_recon = 0;
#ifdef debug
    Serial.println(F("WIFI connect Ok"));
#endif
  }

  //SOFT------AP
  //------------------------------------------------------------------------------------
#ifdef wifi_multi
  if (wifiMulti.run() != WL_CONNECTED)
#else
  if (WiFi.status() != WL_CONNECTED)
#endif
  {
#ifdef mqtt3
    xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
#endif

    if (conn) //уже было соед-нение
    {
      wifi_count_recon++;
#ifdef plotter
      do_plotter('w');
#endif

#ifdef debug
      Serial.print(F("wifi_count_recon:")); Serial.println(wifi_count_recon);
#endif




      if (wifi_count_recon >= wifi_count_rep )
      {
#ifdef debug
        Serial.println(F("wifi_count_recon>wifi_count_rep Reboot"));
#endif
        ESP.restart();
      }
      t_next = millis() + t_wifi_force_connect * 1000UL;
      WiFi.disconnect();
      return;
    }

    Serial.println(F("ERR WIFI set WIFI_AP"));
    WiFi.persistent(false);
    //WiFi.softAPdisconnect(true);
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP);
    delay(1000);
    Serial.println(WiFi.softAPConfig(local_ip, gateway, netmask) ? "Ready" : "Failed!");
    Serial.println(WiFi.softAP(ssid_ap, password_ap) ? "Ready" : "Failed!");
    //WiFi.persistent(true);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

#if !defined(no_tft)
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(0x07E0);
    tft.setCursor(0, 40);
    tft.setTextColor(0x07E0);
    tft.print(F("connect to AP <ESP_METEO> "));
    tft.println(F(" passw=11111111 "));
    tft.print(F("from the phone to come http://"));
    tft.setTextColor(TFT_YELLOW);
    tft.println(myIP);
#ifdef debug
    Serial.println(F("WIFI ERR no connection"));
#endif
#endif //no tft
  }

  t_next = millis() + t_wifi_force_connect * 1000UL;
}






//=================================================================
//    html                          handleSubmit()
//=================================================================
void handleSubmit() {
  String response = "<p>The ssid is ";
  response += server.arg("ssid");
  response += "<br>";
  response += "And the password is ";
  response += server.arg("Password");
  response += "<br>";

  response += "</P><BR>";
  response += "<H2><a href=\"/\">go home</a></H2><br>";
  server.send(200, "text/html", response);


  String s = server.arg("ssid");
  ssid_new = s.c_str();
  String p = server.arg("Password");
  passw_new = p.c_str();

  String filename = "/wifi";

#ifdef pin_sd
  Serial.print("SD ");
  Serial.println(filename);
  File  configFile = SD.open( filename, FILE_WRITE); // or, file handle reference for SD library
#else
  Serial.print("Spiffs ");
  Serial.println(filename);
  fs::File   configFile = SPIFFS.open( filename, "w");    // File handle reference for SPIFFS
#endif

  if (!configFile) {
    Serial.println(filename);
    Serial.println(F(" no open,create"));
    return ;
  }
  //----------------------------





  String str = s + ";" + p + ";";
  Serial.println("STA=");
  Serial.println(str);
  configFile.println(str);
  configFile.flush();
  configFile.close();

  delay(1000);
  ESP.restart();

}
void handleNotFound()
{
  server.send(404, "text/plain", "404 Not Found");
}



void wifi_ap() {
  if (server.hasArg("ssid") && server.hasArg("Password") )
  {
    Serial.println("handleSubmit");
    handleSubmit();
  }
  else {
    Serial.println("else handleSubmit");
    String filename = "/wifi.html";
#ifdef pin_sd
    Serial.print("SD ");
    Serial.println(filename);
    File f = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
#else
    Serial.print("Spiffs ");
    Serial.println(filename);
    fs::File f = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
#endif

    server.streamFile(f , "text/html");
    Serial.println(f.readString());
    f.close();


  }
  append_page_footer();
  //  server.send(200, "text/html", webpage);

}
//-------------------------------------------------------
//                    get_rssi
//-------------------------------------------------------
int wifi_rssi()
{
  int32_t db = WiFi.RSSI();


  if (db <= -100) {
    wifi_power = 0;
  } else if (db >= -50) {
    wifi_power = 100;
  } else {
    wifi_power = 2 * (db + 100);
  }

#ifdef debug
  Serial.print(F("wifi dBm = ")); Serial.print(db);
  Serial.print(F(" ")); Serial.print(wifi_power); Serial.println(F("%"));
#endif
  return wifi_power;
}
