#if defined( def_core0)

void audioInit() {
  xTaskCreatePinnedToCore(
    audioTask,                /* Функция для задачи  MAIN LOOP */
    "audioplay",              /* Имя задачи */
    stack_size,               /* Размер стека */
    NULL,                     /* Параметр задачи */
    priority,                 /* Приоритет */
    &audioplay,               /* Выполняемая операция */
    play_on_core              /* Номер ядра, на котором она должна выполняться */
  );
}

void audioTask( void * pvParameters ) {
  // esp_task_wdt_init(10, false);
  // disableCore0WDT();
#ifdef debug
  Serial.print(F("audio on core:"));
  Serial.println(xPortGetCoreID());
#endif
  CreateQueues();
  if (!audioSetQueue || !audioGetQueue) {
    Serial.println(F("ERR: queues are not init"));
    return;
  }
  struct audioMessage audioRxTaskMessage;
  struct audioMessage audioTxTaskMessage;

#if defined (PCM5102)
  audio.setPinout(bclkPin, wclkPin, doutPin);
#endif

#ifdef vs1053
  audio.begin(); // Initialize VS1053 player
#endif
  audio.setVolume(0);
#ifdef set_audio_mono
  audio.forceMono(true);
#endif
 // audio.setConnectionTimeout(1000, 4000);

  while (true) {
    micros(); //update overflow
    if (xQueueReceive(audioSetQueue, &audioRxTaskMessage, 1) == pdPASS) {
      if (audioRxTaskMessage.cmd == SET_VOLUME) {
        audioTxTaskMessage.cmd = SET_VOLUME;
        audio.setVolume(audioRxTaskMessage.value);
        audioTxTaskMessage.ret = 1;
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      }
      //----------------HOST
      else if (audioRxTaskMessage.cmd == CONNECTTOHOST) {
        audioTxTaskMessage.cmd = CONNECTTOHOST;
        audioTxTaskMessage.ret = audio.connecttohost(audioRxTaskMessage.txt);
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      }
      //----------------SD
      else if (audioRxTaskMessage.cmd == CONNECTTOFS) {
        audioTxTaskMessage.cmd = CONNECTTOFS;
#ifdef pin_sd
        audioTxTaskMessage.ret = audio.connecttoFS(SD, audioRxTaskMessage.txt);
#else //spiffs
        audioTxTaskMessage.ret = audio.connecttoFS(SPIFFS, audioRxTaskMessage.txt);
        //     audioTxTaskMessage.ret = audio.connecttoFS(SD, audioRxTaskMessage.txt, audioRxTaskMessage.value);
#endif
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      }
      else if (audioRxTaskMessage.cmd == GET_VOLUME) {
        audioTxTaskMessage.cmd = GET_VOLUME;
        audioTxTaskMessage.ret = audio.getVolume();
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      }
      //----------------STOP
      else if (audioRxTaskMessage.cmd == STOPSONG) {
        audioTxTaskMessage.cmd = STOPSONG;
#ifdef audio_lib2
        audioTxTaskMessage.ret = audio.stopSong();
#else
        audioTxTaskMessage.ret =  audio.stop_mp3client(); //vs
#endif
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      }
      //----------------BUFF
      else if (audioRxTaskMessage.cmd == INBUFF_FILLED) {
        audioTxTaskMessage.cmd = INBUFF_FILLED;
#ifdef audio_lib2
        audioTxTaskMessage.ret = audio.inBufferFilled();
#else
        audioTxTaskMessage.ret = audio.bufferFilled(); //vs
#endif
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      }
      else if (audioRxTaskMessage.cmd == INBUFF_FREE) {
        audioTxTaskMessage.cmd = INBUFF_FREE;
#ifdef audio_lib2
        audioTxTaskMessage.ret = audio.inBufferFree();
#else
        audioTxTaskMessage.ret = audio.bufferFree(); //vs
#endif
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      }
      else if (audioRxTaskMessage.cmd == ISRUNNING) {
        audioTxTaskMessage.cmd = ISRUNNING;
        audioTxTaskMessage.ret = audio.isRunning();
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      }
      else if (audioRxTaskMessage.cmd == HIGHWATERMARK) {
        audioTxTaskMessage.cmd = HIGHWATERMARK;
        audioTxTaskMessage.ret = uxTaskGetStackHighWaterMark(NULL);
        xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
      }

    }
    if (playing)  audio.loop();
    else
      vTaskDelay(10);

    //  if (!play && !play_mp3) vTaskDelay(uint16_t(200 / portTICK_PERIOD_MS));
    //if (play_mp3) vTaskDelay(10);
    //  vTaskDelay(uint16_t(200 / portTICK_PERIOD_MS));
    //vTaskDelay(20);


    //    vTaskDelay(uint16_t(1000 / portTICK_PERIOD_MS));
  }
}
//-------------------------------------------------------------------
boolean audioConnecttohost(const char* host) {
  //  Serial.print("audioConnecttohost "); Serial.println(host);
  audioTxMessage.cmd = CONNECTTOHOST;
  audioTxMessage.txt = host;
  audioMessage RX = transmitReceive(audioTxMessage);
  return RX.ret;
}

uint32_t audioStopSong() {
  //  Serial.println("audioStopSong ");// Serial.println(filename);
  audioTxMessage.cmd = STOPSONG;
  audioMessage RX = transmitReceive(audioTxMessage);
  return RX.ret;
}

boolean audioConnecttoFS(const char* filename, uint32_t resumeFilePos) {
  //  Serial.print("audioConnecttoFS "); Serial.println(filename);

  audioTxMessage.cmd = CONNECTTOFS;
  audioTxMessage.txt = filename;
  audioTxMessage.value = resumeFilePos;
  audioMessage RX = transmitReceive(audioTxMessage);
  return RX.ret;
}
uint32_t audioInbuffFilled() {
  audioTxMessage.cmd = INBUFF_FILLED;
  audioMessage RX = transmitReceive(audioTxMessage);
  return RX.ret;
}

uint32_t audioInbuffFree() {
  audioTxMessage.cmd = INBUFF_FREE;
  audioMessage RX = transmitReceive(audioTxMessage);
  return RX.ret;
}


void audioSetVolume(uint8_t vol) {
  audioTxMessage.cmd = SET_VOLUME;
  audioTxMessage.value = vol;
  audioMessage RX = transmitReceive(audioTxMessage);
  (void)RX;
}

boolean audioIsRunning() {
  audioTxMessage.cmd = ISRUNNING;
  audioMessage RX = transmitReceive(audioTxMessage);
  return RX.ret;
}

uint32_t audioGetStackHighWatermark() {
  audioTxMessage.cmd = HIGHWATERMARK;
  audioMessage RX = transmitReceive(audioTxMessage);
  return RX.ret;
}


audioMessage transmitReceive(audioMessage msg) {
  xQueueSend(audioSetQueue, &msg, portMAX_DELAY);
  if (xQueueReceive(audioGetQueue, &audioRxMessage, portMAX_DELAY) == pdPASS) {
    if (msg.cmd != audioRxMessage.cmd) {
      Serial.println(F("Error: wrong reply from message queue"));
    }
  }
  return audioRxMessage;
}


#endif
