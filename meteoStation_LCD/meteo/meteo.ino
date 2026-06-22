#include "h\color.h"
#include "settings.h"

#include "mString.h"

#if defined (pin_out1_color_leds) ||defined(pin_out2_color_leds)\
|| (pin_out3_color_leds) || defined(pin_out4_color_leds) || defined(pin_out5_color_leds)
#include <FastLED.h>
#define COLOR_ORDER GRB

#ifdef pin_out1_color_leds
CRGB leds[pin_out1_color_leds];
#endif
#endif //led

#include <GParser.h> //test parser
//=========================================
#ifdef tlg
//=========================================
String tlg_token = "";
String chat_id = "";
byte m_num = 1;

#include <FastBot.h>
FastBot bot(tlg_token);
#endif //tlg

//=========================================
#ifdef pin_r433_rx
//=========================================
#include <RH_ASK.h>
//RH_ASK(uint16_t speed = 2000, uint8_t rxPin = 11, uint8_t txPin = 12, uint8_t pttPin = 10, bool pttInverted = false);
RH_ASK r433rx(2000, pin_r433_rx, 0 , 0);
bool status_r433rx = false;
#endif
//=========================================
#ifdef pin_r433_tx
//=========================================
#include <RH_ASK.h>
//RH_ASK(uint16_t speed = 2000, uint8_t rxPin = 11, uint8_t txPin = 12, uint8_t pttPin = 10, bool pttInverted = false);
RH_ASK r433tx(2000, 0, pin_r433_tx,  0);
bool status_r433tx = false;
#endif



#if defined(def_core0)
#include <esp_task_wdt.h>
TaskHandle_t audioplay;
enum : uint8_t {SET_VOLUME, GET_VOLUME, CONNECTTOHOST, CONNECTTOFS, STOPSONG, SETTONE, INBUFF_FILLED, INBUFF_FREE, ISRUNNING, HIGHWATERMARK};

struct audioMessage {
  uint8_t     cmd;
  const char* txt;
  uint32_t    value;
  uint32_t    ret;
} audioTxMessage, audioRxMessage;

QueueHandle_t audioSetQueue = NULL;
QueueHandle_t audioGetQueue = NULL;

void CreateQueues() {
  audioSetQueue = xQueueCreate(10, sizeof(struct audioMessage));
  audioGetQueue = xQueueCreate(10, sizeof(struct audioMessage));
}

#endif //c0


#define ServerVersion "1.0"
String webpage = "";
#include "css.h"

#ifdef pin_sd
bool    SPIFFS_present = false;
bool   SD_present = true;
#else
bool    SPIFFS_present = true;
bool   SD_present = false;
#endif

#ifdef plotter
bool Plotter = false;
#endif

#ifdef timechange_summer_winter
Timezone TZ(DST, STD);
#endif
bool new_minute_b = false; //new minute start---> end loop
bool new_tick = false;
byte now_min = 99;
byte now_hour = 99;
byte now_day = 99;
byte now_month = 99;
byte now_weekday = 99;
int now_year = 0;
String now_date = "";
String now_time = "";
int y_onload = 15;
uint32_t tm_ntp = 0;
bool ntp_err = true; //err on sync
uint16_t color_sec = 0;
#ifdef tft_320_240
const byte y_step = 14;
#endif
#ifdef tft_480_320
const byte y_step = 17;
#endif

//---------------------------------LCD------------------------------
#ifdef lcd_display
bool lcd_on = true;
String  lcd_val = "";

String lcd_txt1 = "";
int8_t lcd_x = 0;
int8_t lcd_y = 0;
String lcd_txt2 = "";
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#if (lcd_type == 1)
LiquidCrystal_I2C lcd(lcd_addr, 20, 4);
#else
LiquidCrystal_I2C lcd(lcd_addr, 16, 2);
#endif

//num_l
#include "h\lcd_num_m.h"
//num_b
#include "h\lcd_num_b.h"

#endif // lcd_display

#ifdef EN
const char *dayNames[]  = {
  "Su",
  "Mo",
  "Tu",
  "We",
  "Th",
  "Fr",
  "Sa",
};
#else

const char *dayNames[]  = {
  "BC",
  "\x9F\x9D",
  "BT",
  "CP",
  "\xA7\xA2",
  "\x9F\xA2",
  "\xA1\x91"
} ;
/*
   //const String dayNames[7]  = {

  "BC",
  "ПН",
  "BT",
  "CP",
  "ЧТ",
  "ПТ",
  "СБ",
  };
*/

#endif //EN


#if defined( pin_btn1)|| defined( pin_btn2)||defined( pin_btn3)|| defined( pin_btn4)
#include "GyverButton.h"
uint8_t press_count = 0;
#if defined( pin_btn1)
GButton btn1(pin_btn1, btn1_type);
#endif
#if defined( pin_btn2)
GButton btn2(pin_btn2, btn2_type);
#endif
#if defined( pin_btn3)
GButton btn3(pin_btn3, btn3_type);
#endif
#if defined( pin_btn4)
GButton btn4(pin_btn4, btn4_type);
#endif


#endif //btnt



#ifdef ESP32

#if defined( pin_analog_key1)
#include "GyverButton.h"
uint16_t key1_debounce = 65; // установка времени антидребезга
uint16_t key1_timeout = 300; // установка таймаута удержания
uint16_t key1_clicktimeout = 500; // установка таймаута между кликами
uint16_t key1_steptimeout = 400; // установка таймаута между инкрементами
uint16_t key1btn1_min = 9999, key1btn1_max = 9999;
uint16_t key1btn2_min = 9999, key1btn2_max = 9999;
uint16_t key1btn3_min = 9999, key1btn3_max = 9999;
uint16_t key1btn4_min = 9999, key1btn4_max = 9999;
uint16_t key1btn5_min = 9999, key1btn5_max = 9999;

//byte key1_test_min_max = 0;

#ifdef key1btn_1
GButton key1btn1;
#endif
#ifdef key1btn_2
GButton key1btn2;
#endif
#ifdef key1btn_3
GButton key1btn3;
#endif
#ifdef key1btn_4
GButton key1btn4;
#endif
#ifdef key1btn_5
GButton key1btn5;
#endif
#endif//5key



#if defined( pin_analog_key2)
#include "GyverButton.h"
uint16_t key2_debounce = 65; // установка времени антидребезга
uint16_t key2_timeout = 300; // установка таймаута удержания
uint16_t key2_clicktimeout = 500; // установка таймаута между кликами
uint16_t key2_steptimeout = 400; // установка таймаута между инкрементами
uint16_t key2btn1_min = 9999, key2btn1_max = 9999;
uint16_t key2btn2_min = 9999, key2btn2_max = 9999;
uint16_t key2btn3_min = 9999, key2btn3_max = 9999;
uint16_t key2btn4_min = 9999, key2btn4_max = 9999;
uint16_t key2btn5_min = 9999, key2btn5_max = 9999;

//byte key1_test_min_max = 0;

#ifdef key2btn_1
GButton key2btn1;
#endif
#ifdef key2btn_2
GButton key2btn2;
#endif
#ifdef key2btn_3
GButton key2btn3;
#endif
#ifdef key2btn_4
GButton key2btn4;
#endif
#ifdef key2btn_5
GButton key2btn5;
#endif
#endif//5key




#endif//32



//---------------------------------audio------------------------------
bool play = false;
bool play_bg = false;

#if defined (ESP32) && defined(AUDIO)
#if defined (tlg) || defined(mqtt_send)|| defined(wf_send)
String plist_send;
#endif

byte fav_ch[fav_count] = {0};
byte fav_count_real = 0; //после упаковки если меньше чем задано

uint8_t pl_list = 0;
String pl_list_name = "";
uint8_t pl_list_max = 0;
String pl_list_file_name = "/i_stat.txt";
int volume = 50;
int volume_mp3 = 50;
byte ch_min = 254; //номер п.п 1 станции
byte ch_max = 0; //номер п.п последней станции
byte ch_next = 0; //
byte ch_prev = 0; //
bool new_song = false;
bool new_song_mp3 = false;
String ch_name = "";
String ch_url = "";
String play_song = "";
const byte rep_count = 2; //сколько попыток при ошибке открытия сайта
int retryms = 0;
bool b_draw_song = false; //метка отрисовки песни
bool playing = false;
String stat_num = ";"; //список станций для быстрого поиска +num1;+num2;

#if defined (audio_lib2) ||defined (vs1053)
bool mp3_eof = false;
#ifdef pin_sd
//String play_song_mp3 = "";
String play_song_g_mp3 = "";
String play_album_mp3 = "";
String play_year_mp3 = "";
#endif //sd
#endif

#ifdef vs1053
#include "vs1053_ext.h"
#if SPI_bus==0
VS1053 audio(VS1053_CS, VS1053_DCS, VS1053_DREQ, VSPI, VS1053_MOSI, VS1053_MISO, VS1053_SCK);
#endif
#if SPI_bus==1
VS1053 audio(VS1053_CS, VS1053_DCS, VS1053_DREQ, HSPI, VS1053_MOSI, VS1053_MISO, VS1053_SCK);
#endif

#elif defined (audio_lib2)&& defined (PCM5102)
#include "Audio.h"
Audio audio;
#elif defined (audio_lib2)&& !defined (PCM5102)
#include "Audio.h"
Audio audio(true, I2S_DAC_CHANNEL_BOTH_EN );// I2S_DAC_CHANNEL_LEFT_EN  I2S_DAC_CHANNEL_RIGHT_EN  I2S_DAC_CHANNEL_DISABLE
#else
#include "h\iradio.h"
#endif //lib2

#endif //ESP32

//------------------------------------------------------------


#if defined( beep_pass)&&defined( ESP32) &&defined( pin_beep)
#include <Tone32.h>
#endif


#if defined (pin_sd)&&defined (AUDIO)
String track_name = "";
int track_find = 0;
unsigned int curr_track = 0;
byte vol_def_mp3 = 80 ; //1-100
bool mp3_random = false; //0 as is 1 -random
#endif
#if defined (AUDIO)
byte vol_def = 80 ; //1-100 www
String logo_www = "";
#endif

#ifdef fmRDA5807
//#include <RDA5807.h>
#include <Wire.h>
#include <radio.h>
#include <RDA5807M.h>
//RDA5807 FM;
RDA5807M FM;
#endif //fmRDA5807



#ifdef  fmSi4703
#include <Wire.h>
#include "SparkFunSi4703rds.h"
Si4703_Breakout FM(pin_fm_rst, pin_fm_sdio, pin_fm_sclk); //fmSi4703
#endif

#ifdef  fmSi4703_lib2
#include <Wire.h>
#include <SI470X.h>
SI470X FM;
#endif



#if defined (fmSi4703)|| defined (fmRDA5807)||defined (fmSi4703_lib2)
int freq_fm = 924; // 94.5MHz
int volume_fm = 10;
int vol_def_fm = 15 ;
byte fav_ch_fm[fav_count_fm] = {0};
byte fav_count_real_fm = 0; //после упаковки если меньше чем задано
byte ch_min_fm = 254; //номер п.п 1 станции
byte ch_max_fm = 0; //номер п.п последней станции
byte ch_next_fm = 0; //
byte ch_prev_fm = 0; //
//bool new_song=true;
String ch_name_fm = "";
#endif
byte ch_fm = 1; //ном канала FM №1 начальный
byte ch = 1; //ном канала WWW №1 начальный
uint8_t ch_fnd = 1; //ном канала WWW №1 начальный

int ch_mp3 = 1; //ном трека начальный


//---------------------------------TIMER-------------------------------------
byte tm_alarm_count = 0;
byte tm_alarm_period = 0;

#ifdef pin_beep
byte tm_alarm_count_beep = 0;
byte tm_alarm_period_beep = 0;
#endif

uint32_t  t_timer_scr = 0; // screen timer
uint32_t t_timer_boot = 0; //после сбоя чс
uint32_t t_timer = 0; //play  timer on -hand
uint32_t t_timer_b = 0;

byte timer_id = 99; //0-www,1-fm, 2=mp3,3-beep
bool scr_timer = false;
bool  timer_alarm = false; //St timer play beep
int h_timer = 0;
int m_timer = 0;
signed char h1 = 0, h2 = 0, m1 = 0, m2 = 0;
const byte col_count = 6;
#ifdef tft_480_320//---------------480--------------
const byte x_onload = 240;
const byte y_begin = 85;
const byte col_step = 75;
const byte col_1 = 0;
const byte col_2 = 130;//hh
const int col_3 = col_2 + col_step; //hh
const int col_4 = 40 + col_3 + col_step; //mm
const int col_5 = col_4 + col_step; //mm
const int col_6 = col_5 + col_step + 5; //mm
const int y_jpeg = y_begin + 50; //mm
#endif
#ifdef tft_320_240 //---------------320--------------
const byte x_onload = 160;
const byte y_begin = 82;
const byte col_step = 56;
const byte col_1 = 0;
const byte col_2 = 63;//hh
const int col_3 = col_2 + col_step; //hh
const int col_4 = 20 + col_3 + col_step; //mm
const int col_5 = col_4 + col_step; //mm
const int col_6 = col_5 + col_step + 5; //mm
const int y_jpeg = y_begin + 20; //mm
#endif
#ifdef tft_400_300 //---------------400--------------
const byte x_onload = 200;
const byte y_begin = 82;
const byte col_step = 56;
const byte col_1 = 0;
const byte col_2 = 63;//hh
const int col_3 = col_2 + col_step; //hh
const int col_4 = 20 + col_3 + col_step; //mm
const int col_5 = col_4 + col_step; //mm
const int col_6 = col_5 + col_step + 5; //mm
const int y_jpeg = y_begin + 20; //mm
#endif




bool play_fm = false;
bool play_mp3 = false;
bool play_beep = false;
bool start_on_alarm = false; //метка если запущен по расписанию, для выхода после или остаться, если уже играло

const char* ssid     = "";  //точка доступа
const char* password = "";
int wifi_power = 0;



//clock
const int CX = 63;
const int CY = 122;
const byte R = 40;
//----------
#ifdef vcc_read
int vcc = 0;
uint32_t t_next_read_vcc = 0;
bool vcc_alert = false;
#endif
byte t_b = 99, t_e = 99; //ночные часы беру из config
int8_t delta_p = 0; //поправка на давление
int8_t delta_t = 0; //поправка на температуру в комнате
int8_t delta_t0 = 0; //поправка на температуру улица
int t_max = 28, t_min = -10, v_max = 7; //сигнализация на экран предельных значений темп и скорости.ветра, уфо
byte uv_max = 6; //ультрофиолет
unsigned int p_max = 760, p_min = 720 ; //давление для сигнализации в config.txt если есть там первично
byte beep_b = 99; // время "с" беру в config.txt
byte beep_e = 99; //"до" беру в config.txt
byte last_hour = 99;
byte last_minute = 99; //alarm.txt shed

static String log_str = "";

#if defined(mqtt3)
String async_str_mqtt = ""; //async mqtt3
#endif

#if defined (wf_send)||  defined (wf_read)
const uint8_t UDP_PACKET_SIZE = 200;
String async_str_wf = "";
#endif

bool send_def_wf = false; //send info def wifi
//SCR-------------------------------
bool s_info = false; //show #defines
const byte scr_count = 15;
byte scr_count_pack = 0;
signed char scr_number = 0;
char *scr_name[scr_count];
char scr_name_def[4];

byte scr_number_prev;
byte scr_show_count = 0; //если по кругу экраны >1 config.txt
uint16_t t_S1_show = 0;
uint16_t t_S2_show = 0;
uint16_t t_S3_show = 0;
uint16_t t_S4_show = 0;
uint16_t t_S5_show = 0;
uint16_t t_S6_show = 0;
uint16_t t_Sf_show = 0;
uint16_t t_S0_show = 0;
uint16_t t_Sg_show = 0;
bool scr_round = true;

//NTP--------------------------
int8_t TIMEZONE = 99;
bool sync_t = false;
bool sync_force = false;
time_t t_ntp_update = 0;

byte t_next_shed = 0; //T чтения план-ка
byte next_shed_ch = 0;
char next_label;

byte ch_prev_alarm_www = 0; //возврат на этот канал после таймера, если уже играло
byte pl_list_prev_alarm_www = 0;
byte ch_prev_alarm_fm = 0; //возврат на этот канал после таймера, если уже играло
int ch_prev_alarm_mp3 = 0; //возврат на этот канал после таймера, если уже играло

bool  vol_rotate = false; //если громеость менял, оставляю для всех каналов , до выхода их экр. радио
//byte ref_tft_last = 99;
bool show_song = true;



struct alarm_structure {
  //bool  active = false; //volume
  char  label; //тип I inet,m mp3,f fm
  byte  h = 99; //hour
  int  m = 99; //min
  byte  ch_num = 0; // ch
  //String  fname = "";
  unsigned int t_long = 0; //length T millisec
  //byte  count=0; //повторов
  //byte  interval=0; //min
  //  byte  vol = 50; //volume

};
alarm_structure al;
bool play_alarm = false;
byte  beep_mp3_vol = 0; //volume

#ifdef pin_ir
//const byte fn_count = 9;
unsigned long code[fn_count] = {0};
//const char * fn[] = {  "play", "next", "prev", "vl+", "vl-", "mute", "play_fm", "play_mp3", "fav"};
unsigned long code_btn[10] = {0};
String id_ir = "";
#endif

int8_t temp_kv = -100; //T_IN
int pr_kv = -100; //PRES_in
int8_t h_kv = -100; //Hum_in
bool ref_kv_th = false;
bool ref_wh = false;
bool ref_wh_now = false;
int8_t temp_u = -100; //T_OUT
unsigned long period_ulica = 0; //во сколько принял Темп ул.
uint32_t t_read_tk = 0; //во сколько принял Темп кв
unsigned int c_tu_frame = TFT_CYAN;
unsigned int c_th_frame = TFT_CYAN;

String appid   = "";  //ключ ген. беру из config.txt
String lat     = "";
String lon     = "";
byte max_ch3 = 40; //&cnt=40 беру из conf, количество 3 часовых записей на 5дней,
// не больше 40 , можно меньше если зависает, просто уменьшится кол-во дней в прогнозе




#include <TimeLib.h>  // day of the week (1-7), Sunday is day 1
#include <TimeLord.h> //moon
TimeLord myLord;


#if (defined (pin_led)||defined (gpio_out)) && defined (ESP32)
#include "esp32-hal-ledc.h"
const uint32_t freq = 5000;
const uint8_t resolution = 10; ///Resolution 8, 10, 12, 15
#if defined (pin_led)
const uint8_t ledChanne0 = 0;
#endif //led
#endif //led+out


#if defined (gpio_out)
time_t  t_timer_pin_out = 0;
String s_out_old_val = "";

#if defined (pin_out1)
#ifdef pin_out1_color_leds
uint8_t po1_val2 = 0;
#endif
#ifdef ESP32
const uint8_t ledChanne1 = 1;
#endif
uint8_t po1_val = 0;
#endif

#if defined (pin_out2)
#ifdef ESP32
const uint8_t ledChanne2 = 2;
#endif
uint8_t po2_val = 0;
#endif

#if defined (pin_out3)
#ifdef ESP32
const uint8_t ledChanne3 = 3;
#endif
uint8_t po3_val = 0;
#endif

#if defined (pin_out4)
#ifdef ESP32
const uint8_t ledChanne4 = 4;
#endif
uint8_t po4_val = 0;
#endif

#if defined (pin_out5)
#ifdef ESP32
const uint8_t ledChanne5 = 5;
#endif
uint8_t po5_val = 0;
#endif

#endif //gpio_out




#if defined (pin_led)
int8_t led_val = 100;
byte led_def = 100; // яркость TFT 0-100,TFT brightness 0-100
byte led_night = 100; // яркость ночью ,brightness at night
byte led_sunset = 100; // яркость после захода ,after sunset
#endif

bool led_on = true;
#if defined (pin_pir)
uint16_t t_pir_period = 0;
int8_t pir_status = -1;//для cron pir on 1, off 0 else -1
#endif

//--------------------encoder
#if defined (encoder)
#include "GyverEncoder.h"
Encoder enc(CLK, DT, SW, TYPE);

#if defined (encoder_int)
ICACHE_RAM_ATTR   void isrCLK() {
  enc.tick();  // отработка в прерывании
  // Serial.println("isrCLK");
}
ICACHE_RAM_ATTR   void isrDT() {
  enc.tick();  // отработка в прерывании
  // Serial.println("isrDT");
}

#endif
#endif //enc

//--------------------encoder LIB 2
#if defined (encoder_lib2) && defined (ESP32)
#include <ESP32Encoder.h>
ESP32Encoder enc;
#include "GyverButton.h"
GButton enc_btn(SW);
void IRAM_ATTR isr_enc_btn() {
  enc_btn.tick();  // отработка в прерывании
  //  Serial.println("isrCLK");
}
#endif //enc 2


//=============openw=============================openw================================================
#ifdef openw
#include "openWeather.h"
OWMfiveForecast    owF5;
#include "OpenWeatherCurrent.h"
OpenWeatherMapCurrent owNOW;
#include "JsonStreamingParser.h"
#include "JsonListener.h"
JsonStreamingParser pars;

#ifdef ESP32
const String   url_b = F("https://api.openweathermap.org/data/2.5/");
#else
const String   url_b = "https://api.openweathermap.org/data/2.5/";
#endif
int8_t delta_tz = 0; //для выравнивания т зоны к 3 часам запроса
String url = "";


float rain_mm = 0; //количество осадков за день
int davlenie7[8];
int8_t temperatura7[8];
int8_t temperatura7n[8];
int p7_max = 200, p7_min = 2000;

int8_t day7[8];//
String icon7[8];//
//mString<10> icon7[8];

int8_t holiday7[8];//выходные
float rain7[8];//осадки
int8_t veter7[8]; //скор ветра
String wind_ico = "";
int v_gr_7[8];
int8_t weekday7[8];



byte entries = 0; //count ret ch openW

// 3-часовой
#if defined (Sg) ||((defined (S4)||defined (tft2_S4)) && defined(tft_480_320))||defined (epd)
const byte ch_count = 41;
#elif (defined (S4) &&  defined(tft_320_240))||((defined (S3)||defined (tft2_S3)) && defined(tft_480_320))
const byte ch_count = 27; //
#else
const byte ch_count = 17;
#endif

int8_t temperatura_ch[ch_count];
int8_t day_ch[ch_count];//
String icon_ch[ch_count];//
int v_gr_ch[ch_count];
int8_t v_ch[ch_count];
float rain_ch[ch_count]; //данные о дожде мм

uint32_t t_ch_b = 0;


#if defined (Sg)||(defined (S4) && defined(tft_480_320))||(defined (tft2_S4) && defined(tft_480_320))
int p_ch[ch_count];//давление
int8_t h_ch[ch_count];//влажность
#endif

#if defined (epd)
int p_ch[ch_count];//давление
int8_t h_ch[ch_count];//влажность
#endif


int c_wind = 0; //color wind
#ifdef EN
String descript_now = "";
#endif
int8_t temp_ch_n = 100;//init 100
uint8_t last_7day = 0;
int davlenie0 = -100;


unsigned long lastConnectionTime_now = 0;
unsigned long postingInterval_now = 0;

unsigned long lastConnectionTime_ch = 0;
unsigned long postingInterval_ch = 0;
unsigned long lastConnectionTime_uv = 0;
unsigned long postingInterval_uv = 0;

int8_t uv = -1;
//String httpData_uv,httpData_ch;
#endif



//FTP================================
#ifdef ftp
#if defined (pin_sd) && defined (ESP32)
#include "ESP32FtpServer.h"
#else
#include <ESP8266FtpServer.h>
#endif

FtpServer ftpSrv;
#endif





#define FS_NO_GLOBALS //allow spiffs to coexist with SD card, define BEFORE including FS.h
#include <FS.h> //spiff file system

#ifdef pin_sd
#include "SD.h"
unsigned int max_jpg_num = 0;
uint16_t Sf_t_slide = 30; //config.txt
uint16_t max_num_mp3 = 0; //номер max treck
uint16_t min_num_mp3 = 0; //номер max treck
bool status_sd = false;
#endif

#ifdef ESP32
#include "SPIFFS.h" // Needed for ESP32 only
#endif



//==========ESP8266============ESP8266===============ESP8266==================ESP8266=========ESP8266
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#ifdef wifi_multi
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#endif

#include <ESP8266HTTPClient.h>
ESP8266WebServer  server(80);
#ifdef http_update
#include <ESP8266HTTPUpdateServer.h>
ESP8266HTTPUpdateServer httpUpdater;
#define update_path "/update"
#endif
#endif//ESP8266



//===========ESP32=======================ESP32==================ESP32=================ESP32
#ifdef ESP32
#include <WebServer.h>
#ifdef wifi_multi
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#else
#include <WiFi.h>
#endif

#include <HTTPClient.h>
WebServer server(80);

#ifdef http_update
#include <Update.h>
#define update_path "/update"
//const char* serverIndex =
const char* serverIndex PROGMEM =
  "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
  "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
  "<input type='file' name='update'>"
  "<input type='submit' value='Update'>"
  "</form>"
  "<div id='prg'>progress: 0%</div>"
  "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')"
  "},"
  "error: function (a, b, c) {"
  "}"
  "});"
  "});"
  "</script>";
#endif

#endif

///--------------------mqtt--------------------------------------------------
#ifdef mqtt
#include <PubSubClient.h>
WiFiClient wifi_mqtt;
PubSubClient client_mqtt(wifi_mqtt);
#endif //mqtt

#if defined (mqtt2)
#include <MQTT.h>
WiFiClient wifi_mqtt;
MQTTClient client_mqtt;
#endif //mqtt2

#if defined (mqtt3)
extern "C" {
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
}
#include <AsyncMqttClient.h>

AsyncMqttClient client_mqtt;
TimerHandle_t mqttReconnectTimer;
//TimerHandle_t wifiReconnectTimer;
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) ;
void onMqttConnect(bool sessionPresent) ;
//void onMqttSubscribe(uint16_t packetId, uint8_t qos);
//void onMqttUnsubscribe(uint16_t packetId);
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
//void onMqttPublish(uint16_t packetId);
#endif //mqtt3

#if defined (mqtt) || defined(mqtt2)|| defined(mqtt3)
String  mqtt_esp_name = "";
uint16_t t_reconn_mqtt = 5;
uint16_t mqtt_period_send = 20; //period send sec>=20
String mqtt_server = "192.168.100.222";
unsigned int mqtt_port = 1883;
String mqtt_user = "";
String mqtt_password = "";

#ifdef mqtt_send
String mqtt_H_topic_send = "";
String  mqtt_T0_topic_send = "";
String mqtt_T1_topic_send = "";
String  mqtt_P_topic_send = "";

#ifdef AUDIO
String  mqtt_song_topic_send = "";
String  mqtt_plist_topic_send = "";
#endif


#if defined(pin_out1)
String mqtt_out1_topic_send = "";
#endif //ou11

#if defined(pin_out2)
String mqtt_out2_topic_send = "";
#endif //ou12
#if defined(pin_out3)
String mqtt_out3_topic_send = "";
#endif //ou13
#if defined(pin_out4)
String mqtt_out4_topic_send = "";
#endif //ou14
#if defined(pin_out5)
String mqtt_out5_topic_send = "";
#endif //ou15
#endif //send

#ifdef mqtt_read
String mqtt_H_topic_read = "";
String mqtt_T1_topic_read = "";
String mqtt_T0_topic_read = "";
String mqtt_P_topic_read = "";
String mqtt_VCC_topic_read = "";
String mqtt_CMD_topic_read = "";
#if defined(pin_out1)
String mqtt_out1_topic_read = "";
#ifdef pin_out1_color_leds //color led
String mqtt_out1_topic_read_color = "";
String mqtt_out1_topic_send_color = "";
#endif
#endif //ou11
#if defined(pin_out2)
String mqtt_out2_topic_read = "";
#endif //ou12
#if defined(pin_out3)
String mqtt_out3_topic_read = "";
#endif //ou13
#if defined(pin_out4)
String mqtt_out4_topic_read = "";
#endif //ou14
#if defined(pin_out5)
String mqtt_out5_topic_read = "";
#endif //ou15
#endif //read
#endif //mqtt



///--------------------IoT---------------------------------------------------
#if defined (iot_send)||defined (iot_read)
#include "ThingSpeak.h"
WiFiClient  client_iot;
unsigned long iot_id = 0;
String iot_key = "";
uint16_t iot_period_send = 0; //period send sec>=20
byte iot_T1_fld_send = 0; //temp квартира(room);ном поля(num field) bme280
byte iot_H_fld_send = 0; //humidity квартира(room) ;ном поля(num field) bme280
byte iot_T0_fld_send = 0; //temp улица(street)
byte iot_P_fld_send = 0; //давление(pressure)
byte iot_W_fld_send = 0; //скорость ветра (wind speed)
byte iot_R_fld_send = 0; //дождь мм(rain)

//read
unsigned long iot_id_read = 0;
String iot_key_read = "";
uint16_t iot_period_read = 0; //period send sec>=20
byte iot_T1_fld_read = 0; //temp квартира(room);ном поля(num field) bme280
byte iot_H_fld_read = 0; //humidity квартира(room) ;ном поля(num field) bme280
byte iot_T0_fld_read = 0; //temp улица(street)
byte iot_P_fld_read = 0; //давление(pressure)
byte iot_VCC_fld_read = 0; //vcc
#endif


//=================================================================================================



#ifdef ota_update
#include <ArduinoOTA.h>
#endif




String fn_cmd = ""; //stop play next prev
String fn_cmd_tmp = ""; //tmp 2tft
//mString<20> async_cmd; //mqtt wifi ..
//---------------------------------



//IR ESP8266========================================
#if defined(pin_ir) && defined(ESP8266)
//IR========================================
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
IRrecv irrecv(pin_ir); // указываем вывод, к которому подключен приемник
decode_results results;
unsigned long ir_code;
#endif

//IR ESP32========================================
#if defined(pin_ir) && defined(ESP32)
//IR==============================================
#include <IRremote.h>
IRrecv irrecv(pin_ir); // указываем вывод, к которому подключен приемник
decode_results results;
unsigned long ir_code;
#endif
//---------------------------------------------


#include "SPI.h"
#include <JPEGDecoder.h>
//#define FORMAT_SPIFFS_IF_FAILED true


const char* password_ap = "11111111";//ap
const char* ssid_ap = "ESP_METEO";//ap
const char* passw_new = "";//ap
const char* ssid_new = "";//ap
IPAddress local_ip(192, 168, 11, 11);
IPAddress gateway(192, 168, 11, 1);
IPAddress netmask(255, 255, 255, 0);





//=================================================================================
//              display
//=================================================================================
bool tft2_act = false; //метка что в данный момент активет tft 2 для jpeg
#if defined (tft2_Sf)&&defined (def_core0)
bool Sf2_slide_on_play = false;
#endif
#if defined (two_tft) && defined (tft_320_240)
bool ref_tft2 = false;
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
Adafruit_ILI9341 tft2 = Adafruit_ILI9341(TFT_CS2, TFT_DC2, TFT_MOSI2, TFT_CLK2, TFT_RST2, TFT_MISO2);
#define setFreeFont setFont
#elif defined (two_tft) && defined (tft_480_320)
bool ref_tft2 = false;
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <ILI9488.h>
ILI9488 tft = ILI9488(TFT_CS, TFT_DC, TFT_RST);
//Adafruit_ILI9341 tft2 = Adafruit_ILI9341(TFT_CS2, TFT_DC2, TFT_MOSI2, TFT_CLK2, TFT_RST2, TFT_MISO2);
ILI9488 tft2 = ILI9488(TFT_CS2, TFT_DC2, TFT_MOSI2, TFT_CLK2, TFT_RST2, TFT_MISO2);
#define setFreeFont setFont
#elif defined (tft_400_300)
/*
  #ifdef pin_sd
  #define ENABLE_GxEPD2_GFX 1
  #include <GxEPD2_GFX.h>
  #include <GxEPD2_BW.h>
  //#include <GxEPD2_3C.h>
  GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> tft (GxEPD2_420( EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));
  #else //sd
  #define ENABLE_GxEPD2_GFX 1
  #include <GxEPD2_4G_4G.h>
  #include <GxEPD2_4G_BW.h>
  GxEPD2_4G_4G<GxEPD2_420, GxEPD2_420::HEIGHT> tft(GxEPD2_420( EPD_CS, EPD_DC,  EPD_RST, EPD_BUSY));
  #endif //no sd
*/
#define ENABLE_GxEPD2_GFX 1
#include <GxEPD2_GFX.h>
#include <GxEPD2_BW.h>
//#include <GxEPD2_3C.h>
GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> tft (GxEPD2_420( EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));

#define setFreeFont setFont

#else
#include "TFT_eSPI.h"
#ifdef tft_480_320
TFT_eSPI tft = TFT_eSPI(320, 480);
#else
TFT_eSPI tft = TFT_eSPI(240, 320);
#endif //TFT_eSPI
#endif //ifndef (two_tft) 


#if font_v6==1
#include "h\CourierCyr6.h"
#elif font_v6==2
#include "h\TimesNRCyr6.h"
#elif font_v6==3
#include "h\FreeSans6.h"
#elif font_v6==4
#include "h\FreeMono6.h"
#else
#include "h\FreeMono6.h"
#endif

#include "h\CourierCyr9.h"
#include "h\verdana9.h"

#ifdef epd
#include "h\FreeMonoBold7.h"
#define v7b &FreeMonoBold7pt8b
#include "h\FreeMonoBold9.h"
#include "h\FreeMonoBold12.h"
#define v12b &FreeMonoBold12pt7b
#include "h\meteo12.h"
#define m12 &meteo12pt8b
#include "h\meteo18.h"
#define m18 &meteo18pt8b
#include "h\meteo26.h"
#define m26 &meteo26pt7b //icon Wh
#endif //epd

#ifdef two_tft //adafruit
#include "h\FreeMonoBold9.h"
#endif

#include "h\verdana12.h"
#include "h\verdana18.h"
#include "h\FreeMonoBold18.h"
#include "h\verdana24.h"
#include "h\verdana24b.h"


#if font_v6==1
#define v6 &CourierCyr6pt8b
#elif font_v6==2
#define v6 &TimesNRCyr6pt8b
#elif font_v6==3
#define v6 &FreeSans6pt8b
#elif font_v6==4
#define v6 &FreeMono6pt8b
#else
#define v6 &FreeMono6pt8b
#endif

#define v9mono &CourierCyr9pt8b
#define v9b &FreeMonoBold9pt7b
//#define v9 &CourierCyr9pt8b
#define v9 &verdana9pt7b
#define v12 &verdana12pt7b
#define v18 &verdana18pt7b
#define v18b &FreeMonoBold18pt8b
#define v24 &verdana24pt7b
#define v24b &verdanab24pt7b



//=========================================
#ifdef pin_dht
//=========================================
#include "DHT.h"
DHT dht(pin_dht, DHTTYPE);
#endif
//------------------------------------------



//=========================================
#ifdef si7021
//=========================================
#include "SparkFun_Si7021_Breakout_Library.h"
#include <Wire.h>
Weather sensor_si;
#endif
//------------------------------------------

//=========================================
#ifdef bme280
//=========================================
#include <Wire.h>
#include <BME280I2C.h>
BME280I2C sensor_bme;    // Default : forced mode, standby time = 1000 ms
bool status_bme = false;
#endif
//------------------------------------------
//=========================================
#ifdef aht10
//=========================================
#include <Wire.h>
#include <AHTxx.h>
AHTxx sensor_aht(aht10, AHT1x_SENSOR);
bool status_aht = false;
#endif
//------------------------------------------
//=========================================
#ifdef aht20
//=========================================
#include <Wire.h>
#include <AHTxx.h>
AHTxx sensor_aht(aht20, AHT2x_SENSOR);
bool status_aht = false;
#endif
//------------------------------------------


//=========================================
#ifdef pin_ds1820
//=========================================
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(pin_ds1820);                                       // вход датчиков 18b20, аналоговый А1, он же 15 цифровой
DallasTemperature ds(&oneWire);
#endif
//-------------------------------------------
bool ref_temp_u = false;


const byte alert_max = 10; //nrf 433
byte alert_arr_id[alert_max] = {0};
bool  alert = true;

//=========================================
#ifdef nrf
//=========================================
#include <SPI.h>

#include "nRF24L01.h"
#include "RF24.h"

#ifdef ESP32
//SPIClass * vspi = NULL;
SPIClass* hspi = nullptr;
#define nrf_SCLK  14//18
#define nrf_MISO  12//19
#define nrf_MOSI  13//23
#endif //32

RF24 radio(pin_nrf_CE, pin_nrf_CS); // "создать" модуль на пинах 9 и 10 Для Уно
bool status_nrf = false;

#if defined (nrf_read)
bool nrf_sending = true; //для открытия пип призагрузке и после передачи если есть
#else
bool nrf_sending = false;
#endif

#endif
//------------------------------------------

IPAddress timeServerIP;

///NTP===========================================
#include <WiFiUdp.h>
///NTP===========================================

unsigned int  localPort = 2390;      // local port to listen for UDP packets
unsigned long  ntp_time = 0;
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[ NTP_PACKET_SIZE];
byte ntp_num = 1;
String t_sun_v = "", t_sun_z = "";
//#if defined (pin_led)||defined(pin_sunset_out_hight)||defined(pin_sunset_out_low)
uint16_t t_sun_v_min = 99, t_sun_z_min = 99;//in min Tv Tz
//#endif

byte t_sun_v_hh = 99, t_sun_z_hh = 99;

WiFiUDP udp;


//send WIFI read--------------------------------
//=====================================
#if defined (wf_send)||  defined (wf_read)
//=====================================
char packetBuffer_udp[UDP_PACKET_SIZE + 1]; //buffer to hold incoming packet,
IPAddress broadcastAddress;

#if defined(ESP32)&&defined(AsyncUDP_lib)
#include "AsyncUDP.h"
AsyncUDP udp_wf;
void do_wf_read(AsyncUDPPacket packet);
#else
WiFiUDP udp_wf;
#endif

#endif //r_s

//---------------------------------------

bool nigth = false;
time_t t = 0;
bool refresh_all = true; //флаг что есть новые данными надо перерисовать

//SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void setup() {

  Serial.begin(115200);

  log_str.reserve(250);
#ifdef openw
  url.reserve(200);
#endif
#if defined(mqtt3)
  async_str_mqtt.reserve(40); //async mqtt3
#endif


#ifdef AUDIO
  play_song.reserve(90);
  ch_url.reserve(150);
  pl_list_file_name.reserve(13);
  stat_num.reserve(60);
#ifdef pin_sd
  track_name.reserve(15);
#endif
#endif

  //----------------------PIN-OUT-ON-PLAY--------HIGH----------------
#ifdef pin_play_WWW_MP3_out_hight
  pinMode(pin_play_WWW_MP3_out_hight, OUTPUT);
  digitalWrite(pin_play_WWW_MP3_out_hight, LOW);
#endif //
#ifdef pin_play_MP3_out_hight
  pinMode(pin_play_MP3_out_hight, OUTPUT);
  digitalWrite(pin_play_MP3_out_hight, LOW);
#endif //
#ifdef pin_play_WWW_out_hight
  pinMode(pin_play_WWW_out_hight, OUTPUT);
  digitalWrite(pin_play_WWW_out_hight, LOW);
#endif //



#ifdef pin_playFM_out_hight
  pinMode(pin_playFM_out_hight, OUTPUT);
  digitalWrite(pin_playFM_out_hight, LOW);
#endif //
#ifdef pin_playALL_out_hight
  pinMode(pin_playALL_out_hight, OUTPUT);
  digitalWrite(pin_playALL_out_hight, LOW);
#endif //
#ifdef pin_night_out_hight
  pinMode(pin_night_out_hight, OUTPUT);
  digitalWrite(pin_night_out_hight, LOW);
#endif
#ifdef pin_sunset_out_hight
  pinMode(pin_sunset_out_hight, OUTPUT);
  digitalWrite(pin_sunset_out_hight, LOW);
#endif

  //-----------------LOW
#ifdef pin_play_WWW_MP3_out_low
  pinMode(pin_play_WWW_MP3_out_low, OUTPUT);
  digitalWrite(pin_play_WWW_MP3_out_low, HIGH);
#endif //
#ifdef pin_play_WWW_out_low
  pinMode(pin_play_WWW_out_low, OUTPUT);
  digitalWrite(pin_play_WWW_out_low, HIGH);
#endif //
#ifdef pin_play_MP3_out_low
  pinMode(pin_play_MP3_out_low, OUTPUT);
  digitalWrite(pin_play_MP3_out_low, HIGH);
#endif //

#ifdef pin_playFM_out_low
  pinMode(pin_playFM_out_low, OUTPUT);
  digitalWrite(pin_playFM_out_low, HIGH);
#endif //
#ifdef pin_playALL_out_low
  pinMode(pin_playALL_out_low, OUTPUT);
  digitalWrite(pin_playALL_out_low, HIGH);
#endif //
#ifdef pin_night_out_low
  pinMode(pin_night_out_low, OUTPUT);
  digitalWrite(pin_night_out_low, HIGH);
#endif
#ifdef pin_sunset_out_low
  pinMode(pin_sunset_out_low, OUTPUT);
  digitalWrite(pin_sunset_out_low, HIGH);
#endif




  //---------------------------------PIR-----------------------------
#if defined(pin_pir)
  pinMode(pin_pir, INPUT);
  digitalWrite( pin_pir, LOW);
  delay(500);
#endif

#ifdef pin_pir_out_hight
  pinMode(pin_pir_out_hight, OUTPUT);
  digitalWrite(pin_pir_out_hight, LOW);
#endif

#ifdef pin_pir_out_low
  pinMode(pin_pir_out_low, OUTPUT);
  digitalWrite(pin_pir_out_low, HIGH);
#endif
  //  digitalWrite(TFT_CS, HIGH); // TFT screen chip select

#ifdef pin_sd
  digitalWrite(pin_sd, HIGH);
#endif


  //----------------------------------------TFT
#ifdef  two_tft
  tft2.begin();
  tft2.fillScreen(TFT_BLACK );
  tft2.setRotation(rotation2);
#endif

#if defined (tft_400_300)
  //tft.init(115200);
  tft.init(0);
  tft.setRotation(epd_rotation);
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK);
  tft.setFullWindow();
  do_show_define();
  tft.display(false);
#else
  tft.begin();
  tft.fillScreen(TFT_BLACK );
  tft.setRotation(rotation);
  tft.setTextWrap(0);
  //  tft.setCursor(0, 10);
  tft.setFreeFont(v6);
  //  tft.setTextColor(0x07E0);
  do_show_define();
#endif

#if !defined (EN) && !defined (two_tft)&& !defined (epd)
  tft.setAttribute(UTF8_SWITCH , false); //on utf-8
#endif

#if (!defined (EN) && defined (two_tft))|| defined (epd)
  //adafruit
  tft2.cp437(true);
#endif


#ifdef pin_sd
  while (!SD.begin(pin_sd))
  {
    Serial.println(F("SD ERR"));
    print_info("SD_ERR");
    delay(2000);
  }//while
  status_sd = true;
  print_info("SD_OK");
#ifdef debug
  Serial.println(F("SD ok"));
#endif

#if defined(Sf)||defined(tft2_Sf)
#ifdef epd
  max_jpg_num = do_maxnum_file("/foto_bw");
#else
  max_jpg_num = do_maxnum_file("/foto");
#endif

#ifdef debug
  Serial.print(F("max jpeg:")); Serial.println(max_jpg_num);
#endif
#endif //Sf
  //--------------mp3--------------
#ifdef ESP32
  max_num_mp3 = do_maxnum_file("/mp3");
#ifdef debug
  if (max_num_mp3 == 0)
  {
    Serial.println(F("No mp3 file"));
  }
  else
  {
    Serial.print(F("max MP3 num="));
    Serial.println(max_num_mp3);
    Serial.print(F("track_min="));
    Serial.println(min_num_mp3);
  }
#endif //debug
#endif //esp32 mp3
  print_info("SD");
#else //SD
  SPIFFS.begin();
#endif

  if (do_read_config_txt())
    print_info("config OK");
  else
  {
    print_info("config.txt ERR");
    delay(5000);
  }
  //----------------------------------pin_led_out------------------------

#if defined (pin_led)||defined (gpio_out)

#if  defined (ESP32)
#if defined (pin_led)
  pinMode(pin_led, OUTPUT);
  ledcSetup(ledChanne0, freq, resolution);
  ledcAttachPin(pin_led, ledChanne0);
  do_led(led_def);
#endif

#if defined (pin_out1)
  pinMode(pin_out1, OUTPUT);
#ifdef pin_out1_color_leds
  FastLED.addLeds<led_type, pin_out1, COLOR_ORDER>(leds, pin_out1_color_leds).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(0);
  //  currentPalette = RainbowColors_p;
  //  currentBlending = LINEARBLEND;
  //  FastLED.addLeds<LED_TYPE, pin_out1>(leds, pin_out1_color_leds);
#else
  ledcSetup(ledChanne1, freq, resolution);
  ledcAttachPin(pin_out1, ledChanne1);
  gpio_out_set_val(ledChanne1, 0);
#endif
#endif

#if defined (pin_out2)
  pinMode(pin_out2, OUTPUT);
  /*
    #ifdef pin_out2_color_leds
    FastLED.addLeds<NEOPIXEL, pin_out2>(leds, pin_out2_color_leds);
    #else
  */
  ledcSetup(ledChanne2, freq, resolution);
  ledcAttachPin(pin_out2, ledChanne2);
  gpio_out_set_val(ledChanne2, 0);
#endif
  //#endif

#if defined (pin_out3)
  pinMode(pin_out3, OUTPUT);
  ledcSetup(ledChanne3, freq, resolution);
  ledcAttachPin(pin_out3, ledChanne3);
  gpio_out_set_val(ledChanne3, 0);
#endif
#if defined (pin_out4)
  pinMode(pin_out4, OUTPUT);
  ledcSetup(ledChanne4, freq, resolution);
  ledcAttachPin(pin_out4, ledChanne4);
  gpio_out_set_val(ledChanne4, 0);
#endif
#if defined (pin_out5)
  pinMode(pin_out5, OUTPUT);
  ledcSetup(ledChanne5, freq, resolution);
  ledcAttachPin(pin_out5, ledChanne5);
  gpio_out_set_val(ledChanne5, 0);
#endif
#endif //----------------32

#if defined (ESP8266)
  analogWriteRange(1023);
#if defined (pin_led)
  pinMode(pin_led, OUTPUT);
  do_led(led_def);
#endif
#if defined (pin_out1)
  pinMode(pin_out1, OUTPUT);
  digitalWrite(pin_out1, LOW);
  //  gpio_out_set_val(pin_out1, po1_val);
#endif

#if defined (pin_out2)
  pinMode(pin_out2, OUTPUT);
  digitalWrite(pin_out2, LOW);
  //  gpio_out_set_val(pin_out2, po2_val);
#endif
#if defined (pin_out3)
  pinMode(pin_out3, OUTPUT);
  digitalWrite(pin_out3, LOW);
  //  gpio_out_set_val(pin_out3, po3_val);
#endif
#if defined (pin_out4)
  pinMode(pin_out4, OUTPUT);
  digitalWrite(pin_out4, LOW);
  //  gpio_out_set_val(pin_out4, po4_val);
#endif
#if defined (pin_out5)
  pinMode(pin_out5, OUTPUT);
  digitalWrite(pin_out5, LOW);
  //  gpio_out_set_val(pin_out5, po5_val);
#endif
#endif //8266

#endif //led+out


  //----------------------------------pin_analog_key------------------------
#ifdef ESP32
#if defined( pin_analog_key1)

#ifdef key1btn_1
  if (key1_debounce != 0)   key1btn1.setDebounce(key1_debounce);
  if (key1_timeout != 0)   key1btn1.setTimeout(key1_timeout);
  if (key1_clicktimeout != 0)   key1btn1.setClickTimeout(key1_clicktimeout);
  if (key1_steptimeout != 0)   key1btn1.setStepTimeout(key1_steptimeout);

#endif


#ifdef key1btn_2
  if (key1_debounce != 0)   key1btn2.setDebounce(key1_debounce);
  if (key1_timeout != 0)   key1btn2.setTimeout(key1_timeout);
  if (key1_clicktimeout != 0)   key1btn2.setClickTimeout(key1_clicktimeout);
  if (key1_steptimeout != 0)   key1btn2.setStepTimeout(key1_steptimeout);
#endif


#ifdef key1btn_3
  if (key1_debounce != 0)   key1btn3.setDebounce(key1_debounce);
  if (key1_timeout != 0)   key1btn3.setTimeout(key1_timeout);
  if (key1_clicktimeout != 0)   key1btn3.setClickTimeout(key1_clicktimeout);
  if (key1_steptimeout != 0)   key1btn3.setStepTimeout(key1_steptimeout);
#endif


#ifdef key1btn_4
  if (key1_debounce != 0)   key1btn4.setDebounce(key1_debounce);
  if (key1_timeout != 0)   key1btn4.setTimeout(key1_timeout);
  if (key1_clicktimeout != 0)   key1btn4.setClickTimeout(key1_clicktimeout);
  if (key1_steptimeout != 0)   key1btn4.setStepTimeout(key1_steptimeout);
#endif


#ifdef key1btn_5
  if (key1_debounce != 0)   key1btn5.setDebounce(key1_debounce);
  if (key1_timeout != 0)   key1btn5.setTimeout(key1_timeout);
  if (key1_clicktimeout != 0)   key1btn5.setClickTimeout(key1_clicktimeout);
  if (key1_steptimeout != 0)   key1btn5.setStepTimeout(key1_steptimeout);
#endif
#endif

  //----------------------------------KEY2
#if defined( pin_analog_key2)
#ifdef key2btn_1
  if (key2_debounce != 0)   key2btn1.setDebounce(key2_debounce);
  if (key2_timeout != 0)   key2btn1.setTimeout(key2_timeout);
  if (key2_clicktimeout != 0)   key2btn1.setClickTimeout(key2_clicktimeout);
  if (key2_steptimeout != 0)   key2btn1.setStepTimeout(key2_steptimeout);
#endif


#ifdef key2btn_2
  if (key2_debounce != 0)   key2btn2.setDebounce(key2_debounce);
  if (key2_timeout != 0)   key2btn2.setTimeout(key2_timeout);
  if (key2_clicktimeout != 0)   key2btn2.setClickTimeout(key2_clicktimeout);
  if (key2_steptimeout != 0)   key2btn2.setStepTimeout(key2_steptimeout);
#endif


#ifdef key2btn_3
  if (key2_debounce != 0)   key2btn3.setDebounce(key2_debounce);
  if (key2_timeout != 0)   key2btn3.setTimeout(key2_timeout);
  if (key2_clicktimeout != 0)   key2btn3.setClickTimeout(key2_clicktimeout);
  if (key2_steptimeout != 0)   key2btn3.setStepTimeout(key2_steptimeout);
#endif


#ifdef key2btn_4
  if (key2_debounce != 0)   key2btn4.setDebounce(key2_debounce);
  if (key2_timeout != 0)   key2btn4.setTimeout(key2_timeout);
  if (key2_clicktimeout != 0)   key2btn4.setClickTimeout(key2_clicktimeout);
  if (key2_steptimeout != 0)   key2btn4.setStepTimeout(key2_steptimeout);
#endif


#ifdef key2btn_5
  if (key2_debounce != 0)   key2btn5.setDebounce(key2_debounce);
  if (key2_timeout != 0)   key2btn5.setTimeout(key2_timeout);
  if (key2_clicktimeout != 0)   key2btn5.setClickTimeout(key2_clicktimeout);
  if (key2_steptimeout != 0)   key2btn5.setStepTimeout(key2_steptimeout);
#endif
#endif


#endif //32


  //==================================fmSi4703

#ifdef  fmSi4703_lib2
  Wire.begin(pin_fm_sdio, pin_fm_sclk);
  FM.setup(pin_fm_rst, pin_fm_sdio);
  fm_setVolume(0);
  delay(500);
  fm_setFreq(freq_fm);
  FM.setRds(false);
#ifdef debug
  Serial.print(F("Si4703 lib2")); Serial.println(freq_fm);
#endif
#endif  //fmSi4703 lib2

#ifdef  fmSi4703
#ifdef debug
  Serial.print(F("Si4703 fr=")); Serial.println(freq_fm);
#endif
  FM.powerOn();
  fm_setVolume(0);
  fm_setFreq(freq_fm);
#endif  //fmSi4703

#ifdef fmRDA5807
  Wire.begin(pin_fm_sdio, pin_fm_sclk);
  //FM.setup();
  FM.init();
#ifdef debug
  Serial.print(F("fmRDA5 fr=")); Serial.println(freq_fm);
#endif
#ifdef  set_audio_mono_fm
  FM.setMono(true);
#else
  FM.setMono(false);
#endif
  FM.setMute(true);
  delay(500);
#endif  //fmRDA

#if defined (pin_led) //после config.txt
  //  Serial.print(F("do_led"));
  do_led(led_def);

#endif


  //==================================
#ifdef bme280
  //==================================
  Wire.begin();
  for (int i = 0; i <= 2; i++)
  {
    if (!sensor_bme.begin())
    {
      log_str = (F("ERR BME280"));
      delay(100);
    }
    else
    {
      status_bme = true;
      log_str = (F("BME280 OK"));
      break;
    }
  }
  print_info("BME280");
  //==================================
#endif //bme


  //==================================
#if defined (aht10) || defined (aht20)
  //==================================
  Wire.begin();
  if (!sensor_aht.begin())
  {
#ifdef debug
    Serial.println(F("ERR AHTxx"));
#endif
  }
  else
  {
    status_aht = true;
  }
  print_info("AHTxx");
#endif //AHT10

  //==================================
#ifdef si7021
  //==================================
  sensor_si.begin();
#endif //si




  //----------------------------AUDIO----------------PCM
#if defined (ESP32)&&defined (AUDIO) //radio
  // make sure we don't get killed for our long running tasks
#ifdef def_core0
  //  esp_task_wdt_init(10, false);
#endif

#if defined (vs1053)&&!defined(def_core0)
  audio.begin();
  audio.setVolume(0);
#endif
  //--------
#if defined (audio_lib2)&& defined (PCM5102)
#ifndef def_core0
  audio.setPinout(bclkPin, wclkPin, doutPin);
  audio.setVolume(0); // 0...21
#ifdef set_audio_mono
  audio.forceMono(true);
#endif //mono
#endif  //c0
#endif  //lib2

  //-----------------------------audiolib1
  //
#if !defined (audio_lib2)&& !defined (PCM5102)&& !defined (vs1053) //no pcm
  // First, preallocate all the memory needed for the buffering and codecs, never to be freed
  //  Serial.println(F("preallocate the memory"));
  preallocateBuffer = malloc(preallocateBufferSize);
  preallocateCodec = malloc(preallocateCodecSize);
  if (!preallocateBuffer || !preallocateCodec)
  {
    Serial.printf_P(PSTR("ERR: Unable to preallocate %d bytes for app\n"), preallocateBufferSize + preallocateCodecSize);
    while (1) delay(1000); // Infinite halt
  }

#ifdef PCM5102
  out = new AudioOutputI2S(0, 0); //dac
  out->SetPinout(bclkPin, wclkPin, doutPin);
#ifdef debug
  Serial.println(F("ext. DAC"));
#endif

#else
  out = new AudioOutputI2S(0, 1);
#ifdef debug
  Serial.println(F("int. DAC"));
#endif
#endif

  file = NULL;
  buff = NULL;
  mp3 = NULL;
  file_mp3 = NULL;
#endif //lib1
  do_count_pl_list();
  print_info("PL");
#endif //ESP32
  /*
    #ifdef def_core0
    audioInit();
    #endif
  */
  //----------------



  //==================================
#ifdef pin_rele
  //==================================
  pinMode(pin_rele, OUTPUT);
  digitalWrite(pin_rele, rele_on_off);
#endif
  //----------------------------------


  //==================================
#ifdef ds1820
  //==================================
  ds.begin();
  delay(500);
#endif
  //-----------------------------------



  //==================================
#ifdef pin_ir
  //==================================
  irrecv.enableIRIn(); // запускаем прием
  if (!do_read_file_ik())
  {
    tft.setTextColor(TFT_RED);
    tft.println("read ik.txt   ERR ");
    tft.setTextColor(0x07E0);
#ifdef debug
    Serial.println(F("read ik.txt   ERR"));
#endif
    delay(5000);
  }

#endif





  //------------------------------------
  //==================================
#if defined( pin_beep)
  //==================================
  pinMode(pin_beep, OUTPUT); //объявляем пин beep как выход
  digitalWrite(pin_beep, LOW );
#endif
  //----------------------------------





  ///--------------------mqtt--------------------------------------------------
#ifdef mqtt
  //  client_mqtt.setServer(mqtt_server, mqtt_port);
  client_mqtt.setServer(mqtt_server.c_str(), mqtt_port);
  client_mqtt.setCallback(callback_mqtt);
  client_mqtt.setSocketTimeout(mqtt_soc_timeout * 1000L);
#endif //mqtt

#if defined(mqtt2)
  client_mqtt.begin(mqtt_server.c_str(), mqtt_port, wifi_mqtt);
  client_mqtt.onMessage(callback_mqtt);
  client_mqtt.setTimeout(mqtt_soc_timeout * 1000L);
#endif //mqtt



#if defined(mqtt3)
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(t_reconn_mqtt * 1000L), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  client_mqtt.onConnect(onMqttConnect);
  client_mqtt.onDisconnect(onMqttDisconnect);
  //  client_mqtt.onSubscribe(onMqttSubscribe);
  //  client_mqtt.onUnsubscribe(onMqttUnsubscribe);
  client_mqtt.onMessage(onMqttMessage);
  // client_mqtt.onPublish(onMqttPublish);
#endif //mqtt


#ifdef tlg
  bot.setToken(tlg_token);
  bot.setChatID(chat_id);
  bot.setLimit(1);
  bot.attach(newMsg);
#endif


  //WIFI=======WIFI=======WIFI=======WIFI=======WIFI=======WIFI=======WIFI=======
  wifiConnect();
  if (WiFi.getMode() != WIFI_AP) print_info("IP");
  //WIFI=======WIFI=======WIFI=======WIFI=======WIFI=======WIFI=======WIFI=======



#if defined (http_update) && defined(ESP8266)
  httpUpdater.setup(&server, update_path);
#endif

#if defined (http_update) && defined(ESP32)
  server.on(update_path, HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update ok: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
#endif
  server.begin();
  //-------------------------------WIFI_AP
  if (WiFi.getMode() == WIFI_AP)
  {
    goto end_setup;
  }

  //===UDP SEND READ====================================
#if defined (wf_send)||  defined (wf_read)
  broadcastAddress = (uint32_t)WiFi.localIP() | ~((uint32_t)WiFi.subnetMask());
#ifdef debug
  Serial.print(F("Broadcast addr:"));
  Serial.println(broadcastAddress);
#endif

#if defined(ESP32)&&defined(AsyncUDP_lib)
  if (udp_wf.listen(wf_localPort)) udp_wf.onPacket(do_wf_read);
#else
  udp_wf.begin(wf_localPort);
#endif
#endif

#ifdef gpio_out
  do_fread_pin_out(); //pin_out.tmp
#endif
  // openw
#ifdef openw
  do_init_now();
  do_init_ch();
  do_delta_tz();
  do_openw();
#endif  //ow
  //-------------------------------------------------------------------------

  //====FTP================================================
#ifdef ftp
  ftpSrv.begin("esp8266", "esp8266");   //username, password for ftp.
#ifdef debug
  Serial.println(F("ftp esp8266/esp8266"));
#endif
#endif

  //===========IoT==============IoT==================IoT
#if defined (iot_send)||defined (iot_read)
  ThingSpeak.begin(client_iot);  // Initialize ThingSpeak
#endif


#ifdef ota_update
  //=====OTA==========================================================
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
#ifdef debug
    Serial.print(F("Start updating ")); Serial.println(type);
#endif
  });
  ArduinoOTA.onEnd([]() {
#ifdef debug
    Serial.println(F("\nEnd"));
#endif
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
#ifdef debug
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
#endif
  });
  ArduinoOTA.onError([](ota_error_t error) {
#ifdef debug
    Serial.printf("Error[%u]: ", error);
#endif
    if (error == OTA_AUTH_ERROR) {
#ifdef debug
      Serial.println(F("Auth Failed"));
#endif
    } else if (error == OTA_BEGIN_ERROR) {
#ifdef debug
      Serial.println(F("Begin Failed"));
#endif
    } else if (error == OTA_CONNECT_ERROR) {
#ifdef debug
      Serial.println(F("Connect Failed"));
#endif
    } else if (error == OTA_RECEIVE_ERROR) {
#ifdef debug
      Serial.println(F("Receive Failed"));
#endif
    } else if (error == OTA_END_ERROR) {
#ifdef debug
      Serial.println(F("End Failed"));
#endif
    }
  });
  ArduinoOTA.setHostname(ota_name);
  ArduinoOTA.begin();
#endif

  //NTP=======================================
  udp.begin(localPort);
  delay(200);
  //--------------------------------------------


#ifdef only_radio
  fn_cmd = "play";
  do_fn_btn_ir();
#endif


  //==================================
#ifdef pin_dht
  //==================================
  dht.begin();
#endif


#ifdef pin_sd
  randomSeed(millis() );
#endif

#ifdef two_tft
  // tft2.fillScreen(TFT_BLACK);
#endif



#ifdef tft_invert
  tft.invertDisplay( true );
#endif

  tft.setTextSize(1);

  //encoder
#ifdef CLK
  pinMode (CLK, INPUT_PULLUP);
#endif
#ifdef DT
  pinMode (DT, INPUT_PULLUP);
#endif
#ifdef SW
  pinMode (SW, INPUT_PULLUP);
#endif

#if defined (encoder)&& defined (encoder_int)
  attachInterrupt(digitalPinToInterrupt(CLK), isrCLK, CHANGE ); //CHANGE
  attachInterrupt(digitalPinToInterrupt(DT), isrDT, CHANGE ); //RISING
  //  attachInterrupt(digitalPinToInterrupt(SW), enc_int, CHANGE ); //RISING
#endif

#if defined (encoder_lib2) && defined (ESP32)
#ifdef encType_single//single,half,full  
  enc.attachSingleEdge(CLK, DT);
#elif encType_half
  attachHalfQuad(CLK, DT);
#elif encType_full
  attachFullQuad(CLK, DT);
#endif //type
#if defined (puType)
  ESP32Encoder::useInternalWeakPullResistors = puType;
#endif

  attachInterrupt(SW, isr_enc_btn, CHANGE ); //RISING

#if defined (steptimeout)
  enc_btn.setStepTimeout(steptimeout);
#endif
#if defined (setdebounce)
  enc_btn.setDebounce(setdebounce);// настройка антидребезга (по умолчанию 80 мс)
#endif
#if defined (settimeout)
  enc_btn.setTimeout(settimeout);        // настройка таймаута на удержание (по умолчанию 500 мс)
#endif
#if defined (setclicktimeout)
  enc_btn.setClickTimeout(setclicktimeout);   // настройка таймаута между кликами (по умолчанию 300 мс)
#endif
#if defined (pull)
  enc_btn.setType(pull);
#endif
  enc.clearCount();

#endif



  //---------------------------------LCD-----------------------------
#ifdef lcd_display
  lcd.init();
  lcd.setBacklight(1);
  lcd.print("OK");
  delay(1000);
  // lcd.clear();
  lcd.setBacklight(0);
  lcd_on = false;
#if defined (lcd_clock_b)
  load_num_b() ;
#endif
#if defined (lcd_clock_m)
  load_num_m() ;
#endif
#endif //lcd_display


  //------------------------------------------433
#ifdef pin_r433_rx
  if (!r433rx.init())
    log_str = (F("ERR R433rx"));
  else
  {
    log_str = (F("R433rx Ok"));
    status_r433rx = true;
  }
#ifdef debug
  Serial.println(log_str);
#endif
  print_info("R433rx");
#endif
#ifdef pin_r433_tx
  if (!r433tx.init())
    log_str = (F("ERR R433tx"));

  else
  {
    log_str = (F("R433tx Ok"));
    status_r433tx = true;
  }
#ifdef debug
  Serial.println(log_str);
#endif
  print_info("R433tx");
#endif



  //==================================
#if defined(nrf)
  //==================================

#ifdef ESP32
  hspi = new SPIClass(HSPI);
  hspi->begin(nrf_SCLK, nrf_MISO, nrf_MOSI, pin_nrf_CS); //SCLK, MISO, MOSI, SS
  pinMode(pin_nrf_CS, OUTPUT); //VSPI SS
  if (!radio.begin(hspi))
  {
    log_str = (F("ERR NRF24"));
  }
  else
  {
    status_nrf = true;
    log_str = (F("NRF start"));
  }
#ifdef debug
  Serial.println(log_str);
#endif
  print_info("NRF24");
#else //32
  if (!radio.begin())
    log_str = (F("ERR NRF24"));
  else
  {
    status_nrf = true;
    log_str = (F("NRF start"));
  }
#ifdef debug
  Serial.println(log_str);
#endif
  print_info("NRF24");
#endif
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.setChannel(nrf_channel);                                       // Указываем канал приёма данных (от 0 до 127), 5 - значит приём данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
  delay(100);
#if nrf_palevel==1
  radio.setPALevel (RF24_PA_MIN);
#elif nrf_palevel==2
  radio.setPALevel (RF24_PA_LOW);
#elif nrf_palevel==3
  radio.setPALevel (RF24_PA_HIGH);
#elif nrf_palevel==4
  radio.setPALevel (RF24_PA_MAX);
#else
#error "ERR set nrf_palevel "
#endif

  // скорость обмена RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
#if nrf_speed==250
  radio.setDataRate (RF24_250KBPS);
#elif nrf_speed==1000
  radio.setPALevel (RF24_1MBPS);
#elif nrf_speed==2000
  radio.setPALevel (RF24_2MBPS);
#else
#error "ERR set nrf_speed "
#endif
  /*
    radio.openReadingPipe (1, 0xAABBCCDD11LL);
    radio.openReadingPipe (2, 0xAABBCCDD22LL);
    radio.openReadingPipe (3, 0xAABBCCDD33LL);
    radio.openReadingPipe (4, 0xAABBCCDD44LL);
    radio.openReadingPipe (5, 0xAABBCCDD55LL);
  */
  radio.powerUp();
  //  radio.startListening();
  delay(150);
#endif  //nrf


#if defined (def_parse_str)&&defined (pin_vcc_low_alert)
  pinMode(pin_vcc_low_alert, OUTPUT);
  digitalWrite(pin_vcc_low_alert, LOW);
#endif

  scr_pack( );

#ifndef openw
#ifdef tft_320_240
  do_find_scr("S0"); //big clock no openw
#endif

#ifdef tft_480_320
  if (!do_find_scr(scr_name_def))
  {
    String val = "Sn";
    byte len = val.length() + 1;
    char buf[len];
    val.toCharArray(buf, len);
    strcpy(scr_name_def, buf);
    do_find_scr("Sn"); //big clock no openw
  }
#endif

#endif  //openw

#ifdef openw
  if (!do_find_scr(scr_name_def))
    scr_number = 0;
#endif

#ifdef pause_after_boot
  t_timer_scr = millis() + pause_after_boot * 1000L; //использую, чтобы не заводить новую переменную
  while (millis() < t_timer_scr)
  {

    if (!sync_t) do_ntp();
#ifdef openw
    if (!ref_wh) do_openw();
#endif
    delay(100);
  }

  t_timer_scr = 0;
  //delay(pause_after_boot * 1000L);
#endif
#ifdef def_core0
  audioInit();
#endif
  scr_num_show(scr_number);


end_setup:;
  server.on("/",         HomePage);
  server.on("/download",  File_Download);
  server.on("/upload",   File_Upload);
  server.on("/fupload",  HTTP_POST, []()
  {
    server.send(200);
  }, handleFileUpload);
  server.on("/wifi", wifi_ap);
  server.onNotFound(handleNotFound);

  server.on("/delete",   File_Delete);
#ifdef pin_sd
  server.on("/dir",      SD_dir);
#else
  server.on("/dir",      SPIFFS_dir);
#endif

#ifdef debug
  Serial.println(F("--------------END--setup"));
#endif
#if defined(wf_send)
  log_str = "------BOOT";
  do_send_str_wf(log_str);
#endif
}

// LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
//                                lOOP
// LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
void loop() {

  if (WiFi.getMode() == WIFI_AP)
  {
    server.handleClient();

    if (millis() > t_stay_ap + wifi_rep_t)
    {

#ifdef debug
      Serial.println(F("reboot WIFI AP"));
#endif

      ESP.restart();
    }
    //если по какой то причине сразу не приконн.
    //перегр через t_stay_ap.
    goto skip_all;
  }

#if defined(AsyncUDP_lib)
  if (async_str_wf.length() > 0)
  {
#ifdef debug
    Serial.print(F("async_str_wf_loop:"));
    Serial.println(async_str_wf);
#endif
    log_str = async_str_wf;
    async_str_wf = "";
    do_short_cmd(log_str);
  }
#endif//udp
#if defined(mqtt3)
  if (async_str_mqtt != "")
  {
#ifdef debug
    Serial.print(F("-----async_str_mqtt:"));
    Serial.println(async_str_mqtt);
#endif
    log_str = async_str_mqtt;
    async_str_mqtt = "";
    do_short_cmd(log_str);
  }
#endif //mqtt


  //--------------AP------------------------
#ifdef gpio_out
  if (t_timer_pin_out > 0)
    gpio_out_timer_control();//save tmp file
#endif

#ifdef pin_btn1
  do_btn1();
#endif
#ifdef pin_btn2
  do_btn2();
#endif
#ifdef pin_btn3
  do_btn3();
#endif
#ifdef pin_btn4
  do_btn4();
#endif


  //----------------------------serial port
#ifdef debug
  do_serial(); //cmd
#endif  //SERIAL




  if (sync_t)
  {
    if (t_timer_boot > 0 && !scr_timer) //old timer, boot
    {
      fn_cmd = "timer";
      do_fn_btn_ir();
    }

    do_readnow();
#ifdef  time_correct
    now_correct() ;
#endif
    //--------------- раз в минуту   ---------------------------
    do_every_min();  //shed,beep,scr_sav,restart  t=now();
  }



  //ntp not sync
  if (WiFi.status() == WL_CONNECTED)
  {
    //WIFI read data
#ifdef wf_read
#ifndef AsyncUDP_lib
    do_wf_read();
#endif
    if (alert) do_alert();
#endif
    //------tlg mqtt
#if defined(AUDIO)&&( defined(tlg)||defined(mqtt3)||defined(wf_send))
    if (play_bg && new_song)
    {
#if defined(tlg)
      bot.sendMessage(play_song);
#endif
#if defined(mqtt_send)
      do_send_song_mqtt('s');
#endif
#if defined(wf_send)
      do_send_str_wf(play_song);
#endif
      new_song = false;
      play_song = "";
    }
#endif


    if (!sync_t || sync_force == true) do_ntp();
  }
  else
  {
    if (play)
    {
      fn_cmd = "stop";
      do_fn_btn_ir();
      wifiConnect();
    }
  }

#ifdef def_core0
  if (WiFi.status() == WL_CONNECTED)
    //mqtt3
#if defined (mqtt3)&&defined (mqtt_send) //async
    if (client_mqtt.connected())   do_send_mqtt();
#endif //3
#else //!core
  if (WiFi.status() == WL_CONNECTED && !play && !play_mp3)
#endif //core
  {
    //FTP
#ifdef ftp
    ftpSrv.handleFTP();
#endif
    //wwww
    server.handleClient();
    // openw
#ifdef openw
    if (sync_t == true && !play && !play_mp3)   do_openw();
#endif  //ow
  }



#ifdef lcd_display
  draw_lcd();
#endif //lcd_display



  //IR----------------------
#if defined (pin_ir)
  do_read_ir();
#endif


  //Encoder--------------------
#ifdef encoder
  do_encoder();
#endif

#if defined (encoder_lib2) && defined (ESP32)
  do_encoder();
#endif




#if defined( pin_analog_key1)
  do_analog_key1();
#endif

#if defined( pin_analog_key2)
  do_analog_key2();
#endif



  if (new_tick) //2sec //in ntp.ino
  {
    if (t_timer > 0) //приоритет ручного таймера
      control_timer();
  }


  //----------------Local---Sensors------------- --------------

#ifdef  pin_ds1820
  do_read_ds1820();
#endif
#ifdef bme280
  do_read_bme();
#endif
#ifdef aht10
  do_read_aht();
#endif
#ifdef si7021
  do_read_si7021();
#endif

#ifdef pin_dht
  do_read_dht();
#endif


#ifndef epd
  do_color() ;//color frame Tu Tkv sec
#endif //#ifndef epd




  //PIR----------------------
#if defined(pin_pir)
  do_read_pir();
#endif

  //  pin_btn------------------------
#if defined (pin_btn_touch) && defined( ESP32)
  do_touch_btn(1);
#endif


#ifdef only_radio //если  одно радио опрос лок. сенсоров
  read_sensors_radio();
#endif //radio

  //ulica
#if defined(nrf_read)
  if (status_nrf)
  {
    do_read_nrf();
    if (alert) do_alert();
  }
#endif
#if defined(nrf_send)
  if (status_nrf) do_send_data();
#endif


#ifdef pin_r433_rx
  if (status_r433rx)
  {
    do_read_r433();
    if (alert) do_alert();
  }
#endif
#ifdef pin_r433_tx
  if (status_r433tx)  do_send_data();
#endif


  if (WiFi.status() == WL_CONNECTED && !refresh_all) //!refresh_all panic on bg_play

  {
#if defined (read_remote_sensor_onplay)||defined(ESP8266)
    //ioT  ThingSpeak
#if defined (iot_read)
    if (iot_id_read > 0) do_read_iot();
#endif
#endif //read_remote_sensor_onplay
    //mqtt
#if defined (mqtt)||defined (mqtt2)
    if (reconn_mqtt())
    {
      do_send_mqtt(); //not on play_bg
      client_mqtt.loop();
    }
#endif //mqtt

#ifdef ESP8266
    //UDP
#if defined (wf_read)
    do_wf_read();
    if (alert) do_alert();
#endif

#ifdef wf_send
    do_wf_send();
#endif
#endif //8266

#if defined(wf_send)&&defined(AsyncUDP_lib)&&defined(wf_send_on_play)
    do_wf_send();
#endif

  }//                     conn WIFI
  //#endif //read_remote_sensor_onplay


  //------------REF
#ifdef debug
  if (refresh_all) Serial.println(F("REF_ALL"));
  if (ref_kv_th) Serial.println(F("REF_kv_th"));
  if (ref_temp_u) Serial.println(F("REF_temp_u"));
  if (ref_wh) Serial.println(F("REF_wh"));
  if (ref_wh_now) Serial.println(F("REF_wh_now"));

#endif

  //----------------EPD
#ifdef tft_400_300
  if (scr_name[scr_number] == (char *)"S1" ) S1_epd();
  if (scr_name[scr_number] == (char *)"S2" ) S2_epd();
  if (scr_name[scr_number] == (char *)"S3" ) S3_epd();
#ifdef Sfepd
  if (scr_name[scr_number] == (char *)"Sf") Sf_epd();
#endif
#endif


  //------------------------------------------------------------------------------------
  //                                    IF  NOT PLAY!!!!!!!!!!!!!
  //------------------------------------------------------------------------------------
#ifndef only_radio //если  одно радио этот блок искл.

  if ((!play  && !play_fm && !play_mp3 && !scr_timer && !s_info) || play_bg)
  {


    if (WiFi.status() == WL_CONNECTED)
    {
      //ioT  ThingSpeak
#if defined (iot_send)
      if (iot_id > 0 && !play_bg && !refresh_all) do_send_iot();
#endif
#if defined (iot_read)
      if (iot_id_read > 0 && !refresh_all) do_read_iot();
#endif


      //mqtt3
#if defined (mqtt3)&&defined (mqtt_send)&&!defined (def_core0) //only core=1 easync
      if (client_mqtt.connected() && !play_bg && !refresh_all)   do_send_mqtt();
#endif //3

#ifdef wf_send
      if ( !play_bg && !refresh_all) do_wf_send();
#endif
      ///OTA
#ifdef ota_update
      if ( !play_bg)  ArduinoOTA.handle();
#endif
      //ntp
      if ( !play_bg && !refresh_all)   do_ntp();
    }//                     conn WIFI

#ifdef Sn
    //nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn
    if ((scr_name_def != (char*)"Sn" && now_hour >= t_b && now_hour < t_e && t_b < t_e) || (now_hour >= t_b && now_hour > t_e && t_b > t_e) || (now_hour < t_b && now_hour < t_e && t_b > t_e))
    {
      if (nigth == false) //1 raz
      {

        if (scr_name[scr_number] == (char *)"Sn" )
          do_find_scr(scr_name_def);// //если найдет будет по умолч иначе ночной останется
        scr_number_prev = scr_number;

        nigth = true;
        if (do_find_scr( (char *)"Sn"))
          scr_num_show(scr_number);//refresh tft
#if defined(pin_night_out_hight)||defined(pin_night_out_low)
#if defined(pin_night_out_hight)
        digitalWrite(pin_night_out_hight, HIGH);
#endif
#if defined(pin_night_out_low)
        digitalWrite(pin_night_out_low, LOW);
#endif
#endif//pin_night_out

      }

    }
    else   //день

      if (nigth == true)
      {
        //выход с ноч часов
        nigth = false;
        scr_num_show(scr_number_prev);
#if defined(pin_night_out_hight)||defined(pin_night_out_low)
#if defined(pin_night_out_hight)
        digitalWrite(pin_night_out_hight, LOW );
#endif
#if defined(pin_night_out_low)
        digitalWrite(pin_night_out_low, HIGH);
#endif
#endif//pin_night_out
      }
#endif

    //экраны по кругу----RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
#ifndef epd
#ifdef  openw
    if (!nigth && scr_name[scr_number] != (char*)"Sn" && scr_show_count > 1 && scr_round)
      do_scr_round();
#endif
#endif //#ifndef epd

#if !defined (openw) &&   defined (tft_320_240)
    if (!nigth && scr_name[scr_number] != "Sn" && scr_show_count > 1 && scr_round)
      do_scr_round();
#endif

#if !defined (openw) &&   defined (tft_480_320)
    if (!nigth && scr_show_count > 1 && scr_round)
      do_scr_round();
#endif


    //------------------------------3 2 0 x 240 ----------------------------
    //------------------------------3 2 0 x 240 ----------------------------


#if defined (S0) && defined(tft_320_240) && !defined (openw)
    if (scr_name[scr_number] == (char *)"S0") S0_time();
#endif


#if defined (openw) && defined(tft_320_240)

#if defined (S1)
    if (scr_name[scr_number] == (char *)"S1") S1_time();
#endif

    //--------S2-----------------tft_320_240
#if defined (S2)
    if (scr_name[scr_number] == (char *)"S2")
    {
      S2_time();
    }
#endif //S2 

    //--------S3-----------------tft_320_240
#if defined (S3)
#if defined (tft2_S3)
    if (ref_wh || ref_tft2  )
    {
      tft2_act = true;
      S3_show();
      tft2_act = false;
    }
    //#endif
#else
    if (scr_name[scr_number] == (char *)"S3" && (ref_wh == true || refresh_all == true))
    {
      S3_show();
    }
#endif
#endif //S3 tft_320_240



    //-------------S4--------tft_320_240
#if defined (S4)
#if defined (tft2_S4)
    if (ref_wh || ref_tft2)
    {
      tft2_act = true;
      S4_show_ch();
      tft2_act = false;
    }
    //#endif
#else

    if (scr_name[scr_number] == (char *)"S4" && (ref_wh == true || refresh_all == true))
      S4_show_ch();

#endif

#endif //S4


#if defined (S5)
    if (scr_name[scr_number] == (char *)"S5") S5_time();
#endif

    //--------S6-----------------tft_320_240
#if defined (S6)
#if defined (tft2_S6)
    tft2_act = true;
    S6_time();
    tft2_act = false;
#else

    if (scr_name[scr_number] == (char *)"S6")
    {
      S6_time();
    }
#endif
#endif //S6 tft_320_240 

#if defined (tft2_Sf)
    tft2_act = true;
    Sf2_time();
    tft2_act = false;
#endif //Sf2


#endif //-----openw-------------- tft_320_240 


    //------------------------------4 8 0 x 3 2 0----------
    //------------------------------4 8 0 x 3 2 0----------

#if defined(tft_480_320)&& defined (openw)


#if defined (S1)
#if  defined(tft2_S1)
    tft2_act = true;
    S1_time(); //no ref on cmd next or prev
    tft2_act = false;
#else
    if (scr_name[scr_number] == (char *)"S1") S1_time();
#endif
#endif

#if defined (S2)
#if  defined(tft2_S2)
    tft2_act = true;
    S2_time(); //no ref on cmd next or prev    S2_time();
    tft2_act = false;
#else
    if (scr_name[scr_number] == (char *)"S2") S2_time();
#endif
#endif //S2 


#if defined (S3)
#if  defined(tft2_S3)
    tft2_act = true;
    S3_time();
    tft2_act = false;
#else
    if (scr_name[scr_number] == (char *)"S3" )
    {
      S3_time();
    }
#endif
#endif //S3 

#if defined (S4)
#if  defined(tft2_S4)
    tft2_act = true;
    S4_time();
    tft2_act = false;
#else
    if (scr_name[scr_number] == (char *)"S4")
    {
      S4_time();
    }
#endif
#endif //S4


#if defined (S5)
    if (scr_name[scr_number] == (char *)"S5")
    {
      S5_time();
    }
#endif //S5


#if defined (tft2_Sf)
    tft2_act = true;
    Sf2_time();
    tft2_act = false;
#endif //Sf2


#endif //tft_480_320

    //--E--------------------4 8 0 x 3 2 0----------




#if defined (Sn)
    if (scr_name[scr_number] == (char *)"Sn") Sn_time();
#endif

#if defined (pin_sd) && defined (Sf)&& !defined (epd)
    if (scr_name[scr_number] == (char *)"Sf") Sf_time();
#endif

    //-------------------Sg----------------------------------
#if defined (Sg) && defined (openw)

#ifdef tft2_Sg
    if (ref_wh || ref_tft2)
    {
      tft2_act = true;
      Sg_show();
      tft2_act = false;
    }
    //#endif
#else
    if (scr_name[scr_number] == (char *)"Sg" && (ref_wh == true || refresh_all == true))
      Sg_show();
#endif


#endif //Sg



    //-------------------scr_round----------------------------------
    if (scr_show_count > 1 && !scr_round && second(t) % 5 == 0 ) draw_stop_round(); //draw stop around
  }
  //-----------------------------------IF  NOT PLAY
#endif //not radio


  //on play----------------------------------

#if defined (tft2_Sf)&&defined (def_core0)
  if (Sf2_slide_on_play)
  {
    tft2_act = true;
    Sf2_time();
    tft2_act = false;
  }
#endif //Sf2


#if !defined(tft_400_300)&&!defined(no_tft)
  if (scr_timer)
  {
    do_scr_timer();
    if ( timer_alarm)
    {
#if defined ESP32 && defined(AUDIO)
      if (tm_alarm_count > 0)      do_timer_alarm();
#endif //32      
#ifdef pin_beep
      if (tm_alarm_count_beep > 0)  do_timer_alarm_beep();
#endif
    }
  }
#endif //!defined(tft_400_300)


  //===============================================
  //WWW      radio
  //===============================================


#if defined (fmSi4703)|| defined (fmRDA5807) || defined (fmSi4703_lib2)
  if (play_fm)
  {
    do_scr_play_fm();

  }
#endif   //fmSi4703


#if defined ESP32 && defined(AUDIO)

  if (play && !play_bg)
  {
#ifndef no_tft
    do_scr_play();
#endif

#if defined(audio_lib2)||defined(vs1053)
#ifndef def_core0
    audio.loop();
#endif //c0
#else
    do_play();
#endif
  }
#endif  //esp32

#if defined (def_core0) && defined(AUDIO)
  if (play_beep && mp3_eof) //+beep
  {
    mp3_eof = false;
    play_beep = false;
  }
#endif

#if defined (pin_sd)&&defined (ESP32) && defined(AUDIO)
  if (play_mp3) //+beep
  {
#if defined(def_core0)
    if (mp3_eof)
    {
      fn_cmd = "next";
      do_fn_btn_ir();
      draw_num(0, 'C');
      mp3_eof = false;
    }
#endif //c0
#ifndef no_tft
    do_scr_play_mp3();
#endif

#if defined(audio_lib2)||defined(vs1053)
#ifndef def_core0
    audio.loop();
#endif //c0
#else
    do_play_mp3(); //lib1
#endif
  }
#endif   //pin_sd mp3



#ifdef pin_ir
  if (refresh_all)
  {
    irrecv.resume(); // принимаем следующую команду

  }
#endif

#ifdef vcc_read
  if (refresh_all) draw_vcc();
#endif
  //-------------------init
  refresh_all = false;
#ifdef two_tft
  ref_tft2 = false;
#endif
  ref_kv_th = false;
  ref_temp_u = false;
  ref_wh = false;
  ref_wh_now = false;
  new_minute_b = false;
  fn_cmd_tmp = "";
skip_all:;


}

// E---loop---EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE




//------------------------------------------------------------------------------------
//                        do_every_min
//------------------------------------------------------------------------------------

void do_every_min()
{
  byte mm = now_min;
  byte hh = now_hour;

  //mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
  if (last_minute != mm)
  {
    do_read_cron_txt();//test=false
#ifdef vcc_read
    draw_vcc();
#endif

    last_minute = mm;
#ifdef debug
    if (!play) serial_time_start ();
#endif

    //заход
#if defined(pin_sunset_out_hight)||defined(pin_sunset_out_low)
    bool static b_sunset_ = false;
    int t_minute_now = hh * 60 + mm;
    if   (( t_minute_now > t_sun_z_min || t_minute_now < t_sun_v_min) && b_sunset_ == false && t_sun_z_min != 0)
    {

#if defined(pin_sunset_out_hight)
      digitalWrite(pin_sunset_out_hight, HIGH);
#endif
#if defined(pin_sunset_out_low)
      digitalWrite(pin_sunset_out_low, LOW);
#endif

#ifdef debug
      Serial.println(F("pin_sunset_out_ ON"));
#endif
      b_sunset_ = true;
    }
    else if   ( t_minute_now > t_sun_v_min && t_minute_now < t_sun_z_min && t_sun_v_min != 0)
    {

#if defined(pin_sunset_out_hight)
      digitalWrite(pin_sunset_out_hight, LOW);
#endif
#if defined(pin_sunset_out_low)
      digitalWrite(pin_sunset_out_low, HIGH );
#endif



#ifdef debug
      Serial.println(F("pin_sunset_out_ OFF"));
#endif

      b_sunset_ = false;

    }


#endif //pin_sunset


#ifdef pin_led
    bool static b_sunset = false, b_def = false;
    int t_min_now = hh * 60 + mm;
    if (!nigth)
    {

      if   (( t_min_now > t_sun_z_min || t_min_now < t_sun_v_min) && b_sunset == false && t_sun_z_min != 0)
      {
        if (led_on) do_led(led_sunset);
        else
          led_val = led_sunset;


#ifdef debug
        Serial.println(F("SunSet led_sunset"));
        Serial.println(b_sunset);
#endif

        b_sunset = true;
        b_def = false;
      }

      else if   ( t_min_now > t_sun_v_min && t_min_now < t_sun_z_min && b_def == false && t_sun_v_min != 0)
      {

        if (led_on) do_led(led_def);
        else
          led_val = led_def;


#ifdef debug
        Serial.println(F("SunRise led_def"));
        Serial.println(b_def);
#endif


        b_def = true;
        b_sunset = false;

      }

    }
    else  do_led(led_night);


#endif //led               


    //в начале часа принудительно обновляю погоду
#ifdef openw
    if (!play && !play_mp3)
    {
      if (mm == 7) lastConnectionTime_ch = 0;
      if (mm == 6) lastConnectionTime_now = 0;
    }
#endif //openw





  }


  //hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh

  if (last_hour != hh) //раз в час и была синхр-я
  {
    last_hour = hh;

    //beep
#ifdef pin_beep

    if (beep_b < beep_e && mm == 0 && hh >= beep_b && hh <= beep_e && !nigth && scr_name[scr_number] != "Sn" )
      do_beep(fr, dur);
#endif



  }

}


//------------------------------------------------------------------------------------
//                        do_serial
//------------------------------------------------------------------------------------
#ifdef debug
void do_serial()
{
  if (Serial.available()) {
    String r = Serial.readString(); r.trim();
    if (r.length() > 0)
    {
      log_str = r;
      do_short_cmd(log_str);
    }
  }
}
#endif
//------------------------------------------------------------------------------------
//                        do_short_cmd
//------------------------------------------------------------------------------------
void do_short_cmd(String &str)
{

  if (str.length() > 15 && play && str[0] != '=' && str[0] != ':')
  {
#if defined(audio_lib2)|| defined(vs1053)
    ch_fnd = 0;
    ch_url = str;
    str = "";
    play_song = "";
    ch_name = "";
    fn_cmd = "play";
    do_fn_btn_ir();

#endif //lib2
  }
  else if (str.length() > 0)
  {
#ifdef debug
    if (str[0] == ':')
    {
      str[0] = ' ';
      do_parse_cron(str);//test
      return;
    }
    else
#endif
      /*
           if (str == "sm_end")
           {
             Serial.end();
        #ifdef ESP32
             gpio_reset_pin((gpio_num_t)1);
             gpio_reset_pin((gpio_num_t)3);
             gpio_pullup_dis((gpio_num_t)1);
             gpio_pullup_dis((gpio_num_t)3);
        #endif
           }
           else if (str == "sm_start")
           {
             Serial.begin(115200);
             delay(100);
           }
        else
      */
      if (str[0] == '-' || str[0] == '=') //для RX пока оставил =="="
      {
        if (do_parse_str(str))    return;//иначе  дальше
      }

#if defined (wf_send)
      else if (str == "define")
      {
        send_def_wf = true;
        print_info("IP");
        do_show_define();
        print_info("tm_long");
        print_info("tm_ntp");
#if defined (openw)
        print_info("tm_wh_now");
        print_info("tm_wh_ch");
#endif
        print_info("mem");
        send_def_wf = false;
        str = "";
      }

      else if (str == "get_stat")
      {
#if defined (gpio_out)
        do_wf_send_gpio_status();
#endif
#if defined (AUDIO)
        String  vl_ = "vlR:";
        vl_ += String(volume);
        do_send_str_wf(vl_);
#endif
      }
#endif //wf

#if defined (nrf_send)||defined (pin_r433_tx)
      else       if (str[0] == '_') //передаю дальше
      {
#if defined (nrf_send)
        if (status_nrf) do_send_nrf(str);
#endif
#ifdef pin_r433_tx
        if (status_r433tx) do_send_r433(str);
#endif
        return; //-->do_short_cmd();
      }
#endif //nrf r433

      else if (str == "n") fn_cmd = "next";
      else if (str == "p") fn_cmd = "prev";
      else if (str == "m") fn_cmd = "mute";
      else if (str == "f") fn_cmd = "fav";
      else if (str == "s") fn_cmd = "stop";
      else if (str == "-") fn_cmd = "vl-";
      else if (str == "+") fn_cmd = "vl+";
      else if (str == "nl") fn_cmd = "next_pl";
      else if (str == "pl") fn_cmd = "prev_pl";
      else if (str == "i") fn_cmd = "Scr_info";
      else if (str == "fm") fn_cmd = "play_fm";
      else if (str == "mp") fn_cmd = "play_mp3";
    //timer
      else if (str == "t" && !play_fm && !play_mp3 && !play) fn_cmd = "timer";
      else if (str == "play" && scr_timer) fn_cmd = "timer_start";

      else if (str == "rst") fn_cmd = "restart";
    //   else if (str == "ref_kv_th") ref_kv_th = true;
    //    else if (str == "ref_temp_u") ref_temp_u = true;
      else if (str == "ref") refresh_all = true;
#ifdef two_tft
      else if (str == "ref_tft2") ref_tft2 = true;
#endif

#if defined(pin_beep)
      else if (str == "beep") do_beep(fr, dur);
#endif
#if defined(audio_lib2)||defined(vs1053)
      else if (str == "ns") fn_cmd = "next_scr";
      else if (str == "ps") fn_cmd = "prev_scr";
      else if (str == "pbg") fn_cmd = "play_bg";
      else if (str == "vl")
      {
        Serial.print(F("getVol:")); Serial.println(audio.getVolume());
      }

#endif //lib2

      else if (str.toInt() > 0 && str.toInt() < 100 && (play || play_bg))
      {
        ch_fnd = str.toInt();
        fn_cmd = "play";
      }
    //--------------------------------cmd
      else
        fn_cmd = str;

  }
#ifdef debug
  Serial.print(F("short fn_cmd:")); Serial.println(fn_cmd);
#endif

  do_fn_btn_ir();
  str = "";//для енкодера и кнопок опустил ниже, там не log_str а fn_cmd

}





//=======================================================================
//                do_parse_str
//=======================================================================
#if defined (def_parse_str)||defined (debug)//read_meteo_send
bool do_parse_str(String &str)
{
  String par_full = "";
  String par_full_loc = "";
  int par_val, par_val2, par_val3;
  bool ret_cmd = false; //если true выход из short_cmd
  const byte count_loc_max = 4;
  //#ifdef pin_out1_color_leds
  //  static uint8_t bright1 = 0;
  //#endif
  // при '-' команда идёт дальше в short_cmd, при "=" возврат в исход. func
  if (str[0] == '=') ret_cmd = true;


  str[0] = 32; //" "
  str.trim();

  if (str.indexOf("tm:") == 0)
  {
    fn_cmd = str;
    return false;
  }

  GParser data((char*) str.c_str(), ';');
  byte count = data.split();
  byte count_loc = 0;
#ifdef debug
  Serial.print(F("parse_full_count:"));
  Serial.println( count);
#endif
  //1111111111111111111111111
  for (byte i = 0; i < count; i++)
  {
    par_full = data[i];
#ifdef debug
    Serial.print(F("num:")); Serial.print(i);
    Serial.print(F(" {"));
    Serial.print( data[i]);
    Serial.print(F("} "));
#endif
    //--------------------------------
    // par_full_ = par_full;
    GParser data_loc((char*) par_full.c_str(), ':');
    count_loc = data_loc.split();
    if ( count_loc == 0 )
    {
      Serial.println(F("parse_loc ERR not':'"));
      break;
    }
#ifdef debug
    Serial.print(F(">>"));
    Serial.print( count_loc);
#endif
    //   par_name = "";
    par_val = -99;
    par_val2 = -99;
    par_val3 = -99;
#ifdef debug
    Serial.print(F("["));
#endif
    if (count_loc > count_loc_max) count_loc = count_loc_max;
    for (byte ii = 0; ii < count_loc; ii++)
    {
      //      if (ii == 0)    par_name = data_loc[0];
      if (ii == 1)    par_val = data_loc.getInt(1);
      if (ii == 2)    par_val2 = data_loc.getInt(2);
      if (ii == 3)    par_val3 = data_loc.getInt(3);
#ifdef debug
      if (ii > 0) Serial.print(':');
      Serial.print( data_loc[ii]);
#endif
    }
#ifdef debug
    Serial.println(F("]"));
#endif

#if defined(AUDIO)

    //------------------------------------beep file
    if (data_loc.equals(0, "beep") && !play_mp3 && !play)
    {
      //  async_str = data_loc[0];
      String filename_mp3 = data_loc[1];
      if (filename_mp3.length() > 0)
        do_play_beep(filename_mp3);//play_mp3 file
      return true;
    }


    //------------------------------------play
    if (t_timer_scr == 0 && ( data_loc.equals(0, "play") || data_loc.equals(0, "play_bg")))
    {
      bool play_short_cmd = false;
      if (t_timer > 0) timer_stop();
      else
      {
        //запоминаю режим
      }
      // async_str = data_loc[0]; //=play
      fn_cmd = data_loc[0];
      if (par_val > 0 && ch_fnd != par_val)
      {
        ch_fnd = par_val; //ch
        play_short_cmd = true;
      }
      //plist
      if (par_val2 >= 0 && par_val2 <= pl_list_max && pl_list != par_val2)
      {
        pl_list = par_val2;
        play_short_cmd = true;
      }

      if (par_val3 > 0)
      {
        timer_start( par_val3, 0);//timer play

      }
      if (play && play_short_cmd == false && par_val3 > 0)
      {
        ret_cmd = true; //уже играет тот же канал+лист, просто timer restart
        str = "";
      }
    }
    //------------------------------------play_mp3
    if (t_timer_scr == 0 && (data_loc.equals(0, "play_mp3") && !play_mp3))
    {
      if (t_timer > 0) timer_stop();
      else
      {
        //запоминаю режим
      }
      //  async_str = data_loc[0];
      fn_cmd = data_loc[0];
      if (par_val > 0) timer_start( par_val, 2);//timer play_mp3 id=2
      // return;
    }


    //------------------------------------VOL
    if (data_loc.equals(0, "vl") && play)
    {
      if (par_val > 0) volume = par_val;
      set_vol_www();
      vol_rotate = true;

    }
    if (data_loc.equals(0, "vl") && play_mp3)
    {
      if (par_val > 0) volume_mp3 = par_val;
      set_vol_mp3();
      vol_rotate = true;

    }
    /*
        if (data_loc.equals(0, "vl") && play_fm)
        {
          if (par_val > 0) volume = par_val;
          set_vol_fm();

        }
    */
#endif //audio



#if defined(wf_read_Tu)||defined( r433_read_Tu)||defined( nrf_read_Tu)||defined( tlg)||defined( debug)
    if (data_loc.equals(0, "T0"))
    {
      if (par_val + delta_t0 != temp_u)
      {
        ref_temp_u = true;
        temp_u = par_val + delta_t0;
      }
      period_ulica = millis();
    }
#endif //T0

#if defined(wf_read_Tk)||defined( r433_read_Tk)||defined( nrf_read_Tk)||defined( tlg)||defined( debug)
    if (data_loc.equals(0, "T1"))
    {
      if (par_val + delta_t != temp_kv)
      {
        ref_kv_th = true;
        temp_kv = par_val + delta_t;
      }
      t_read_tk = millis();
    }
#endif //T1

#if defined(wf_read_Hh)||defined( r433_read_Hh)||defined( nrf_read_Hh)||defined( tlg)||defined( debug)

    if (data_loc.equals(0, "H"))
    {
      if (par_val != h_kv)
      {
        ref_kv_th = true;
        h_kv = par_val;
      }
      t_read_tk = millis();
    }
    else
#endif
#if defined(wf_read_Pr)||defined( r433_read_Pr)||defined( nrf_read_Pr)||defined( tlg)||defined( debug)
      if (data_loc.equals(0, "P"))
      {
        if (par_val + delta_p != pr_kv)
        {
          pr_kv = par_val + delta_p;
        }
      }

#endif

#ifdef vcc_read
    if (data_loc.equals(0, "VCC"))
    {
      t_next_read_vcc = millis() + t_vcc_ref * 1000L;
      if (par_val != vcc)
      {
        vcc = par_val;
        draw_vcc();
      }

    }
#endif
#ifdef alert_read
    int8_t a_num = -1;
    if (data_loc.equals(0, "A1")) a_num = 1;
    else if (data_loc.equals(0, "A2")) a_num = 2;
    else if (data_loc.equals(0, "A3")) a_num = 3;
    else if (data_loc.equals(0, "A4")) a_num = 4;
    else if (data_loc.equals(0, "A5")) a_num = 5;
    else if (data_loc.equals(0, "A6")) a_num = 6;
    else if (data_loc.equals(0, "A7")) a_num = 7;
    else if (data_loc.equals(0, "A8")) a_num = 8;
    else if (data_loc.equals(0, "A9")) a_num = 9;
#ifdef debug
    Serial.print(F("a_num :"));    Serial.print( a_num);
    Serial.print(F(" par_val: "));    Serial.println( par_val);
#endif
    if (par_val > 0 && a_num >= 0)
    {
      alert_arr_id[a_num] = par_val; //count blink uint8_t
      alert = true; //flag ON
      return true;
    }

#endif
    //------------------------------------LED
#if defined(pin_led)
    if (data_loc.equals(0, "led"))
    {
      do_led(par_val);
      return true;
    }
#endif //led

    //------------------------------------PIN_OUT
#ifdef gpio_out

#if defined(pin_out1)
    if (data_loc.equals(0, "out1"))
    {
#ifdef pin_out1_color_leds
      bool br_changed = false; //для таймера и mqtt send
      bool color_changed = false; //для таймера и mqtt send
      uint8_t bright1 = 0;
      uint8_t color1 = 0;
      if (par_val == -99 || data_loc.equals(1, "") || po1_val == par_val) //нет яркости
      {
        Serial.println(po1_val);
        bright1 = map(po1_val, 0, 100, 0, 255);
        //  FastLED.setBrightness(bright1); //bright
        Serial.print(F("bright1 old:"));
        Serial.println(bright1);
      }
      else
      {
        br_changed = true;

        bright1 = map(par_val, 0, 100, 0, 255);

#ifdef debug
        Serial.print(F("bright1 old/new:"));
        Serial.print(po1_val); Serial.print(F("/"));
        Serial.println(par_val);
#endif
        po1_val = par_val; //для записи 0-100
      }
      //COLOR
      if (par_val2 >= 0 && !data_loc.equals(2, "")  && po1_val2 != par_val2 ) //color
      {
        color_changed = true;
#ifdef debug
        Serial.print(F("color old/new:"));  Serial.print(po1_val2);
        Serial.print(F("/")); Serial.println(par_val2);
#endif
        po1_val2 = par_val2;
        //   Serial.print(F(" br:"));
        //  Serial.println(bright1);
        color1 = map(par_val2, 0, 100, 0, 255);
      }
      else//old
      {
        color1 = map(po1_val2, 0, 100, 0, 255);
        Serial.print(F("color old:"));
        Serial.print(po1_val2);
      }

      if (color_changed || br_changed)
      {
#ifdef debug
        Serial.print(F("color_br_changed:"));  Serial.print(bright1);
        Serial.print(F("/")); Serial.println(color1);
#endif
        FastLED.setBrightness(bright1); //bright
        for (byte l = 0; l < pin_out1_color_leds; l++)
        {

          leds[l] = CHSV( color1, 255, bright1);
        }
        FastLED.show();
        //timer
        t_timer_pin_out = millis() + t_period_save_status * 1000UL; //reset timer
#ifdef mqtt_send
        if (br_changed) do_send_mqtt_pin_out(pin_out1, po1_val); //bright1
        if (color_changed) do_send_mqtt_pin_out_color(pin_out1, po1_val2); //color
#endif
#ifdef wf_send_gpio
        if (br_changed) do_send_wf_pin_out(pin_out1, po1_val); //bright1
        if (color_changed) do_send_wf_pin_out_color(pin_out1, po1_val2); //color
#endif
#ifdef debug
        Serial.println(F("t_timer_pin_out color START"));
#endif
      }
#else//End color
      gpio_out_set_val(pin_out1, par_val);
#endif
    }
#endif //out1

#if defined(pin_out2)
    if (data_loc.equals(0, "out2"))
    {
      gpio_out_set_val(pin_out2, par_val  );
      return true;
    }

#endif

#if defined(pin_out3)
    if (data_loc.equals(0, "out3"))
    {
      gpio_out_set_val(pin_out3, par_val  );
      return true;
    }
#endif
#if defined(pin_out4)
    if (data_loc.equals(0, "out4"))
    {
      gpio_out_set_val(pin_out4, par_val  );
      return true;
    }
#endif
#if defined(pin_out5)
    if (data_loc.equals(0, "out5"))
    {
      gpio_out_set_val(pin_out5, par_val  );
      return true;
    }
#endif

#endif //gpio_out
  } //for count par full;
  if (ret_cmd) str = "";
#ifdef debug
  Serial.print(F("ret_cmd:")); Serial.print(ret_cmd);
  Serial.print("str:"); Serial.print(str);
  Serial.println(F(" parseOk"));
#endif
  return ret_cmd;



}
#endif
