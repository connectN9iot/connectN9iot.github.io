

#include "Arduino.h"
#include "ArduinoJson.h"
#include <FirebaseESP32.h>
#include <WiFi.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

#include <ShiftRegister74HC595.h>

ShiftRegister74HC595<2> shiftRegister1(/*data || SDI*/ 17, /*clock || sclk*/ 5,
                                       /*latch || LOAD*/ 13);
ShiftRegister74HC595<2> shiftRegister2(27, 14, 21);

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Cong Nghe Hy Lap 2.4G"
#define WIFI_PASSWORD "vanlabay199"

// For the following credentials, see
// examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyAVvEOBMqsd3NGI6LN3FDrVfwF6uxDVGqk"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://iotn9-dengiaothong-default-rtdb.firebaseio.com"
/* 4. Define the user Email and password that alreadey registerd or added in
 * your project */
#define USER_EMAIL "hovanthanh3349@gmail.com"
#define USER_PASSWORD "hovanthanhsc1"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

const int led_red_1_pin = 22;
const int led_yellow_1_pin = 23;
const int led_green_1_pin = 16;

const int led_red_2_pin = 33;
const int led_yellow_2_pin = 25;
const int led_green_2_pin = 32;

int time_for_red = 18;   // thời gian đèn đỏ
int time_for_green = 15; // thời gian đèn xanh
int time_for_yellow = 3; // thời gian đèn vàng

String red_command = "0";
String yellow_command = "1";
String green_command = "2";

bool is_all_yellow = false;

int state_led_1 = 0;
int state_led_2 = 0;

int old_state_led_1 = 0;
int old_state_led_2 = 0;

// bool is

#define turn_on_led_red_1                \
  {                                      \
    digitalWrite(led_red_1_pin, HIGH);   \
    digitalWrite(led_green_1_pin, LOW);  \
    digitalWrite(led_yellow_1_pin, LOW); \
  }

#define turn_on_led_green_1              \
  {                                      \
    digitalWrite(led_red_1_pin, LOW);    \
    digitalWrite(led_green_1_pin, HIGH); \
    digitalWrite(led_yellow_1_pin, LOW); \
  }

#define turn_on_led_yellow_1              \
  {                                       \
    digitalWrite(led_red_1_pin, LOW);     \
    digitalWrite(led_green_1_pin, LOW);   \
    digitalWrite(led_yellow_1_pin, HIGH); \
  }

#define turn_on_led_red_2                \
  {                                      \
    digitalWrite(led_red_2_pin, HIGH);   \
    digitalWrite(led_green_2_pin, LOW);  \
    digitalWrite(led_yellow_2_pin, LOW); \
  }

#define turn_on_led_green_2              \
  {                                      \
    digitalWrite(led_red_2_pin, LOW);    \
    digitalWrite(led_green_2_pin, HIGH); \
    digitalWrite(led_yellow_2_pin, LOW); \
  }

#define turn_on_led_yellow_2              \
  {                                       \
    digitalWrite(led_red_2_pin, LOW);     \
digitalWrite(led_green_2_pin, LOW);   \
    digitalWrite(led_yellow_2_pin, HIGH); \
  }

#define set_all_yellow(isOn)              \
  {                                       \
    digitalWrite(led_red_1_pin, LOW);     \
    digitalWrite(led_green_1_pin, LOW);   \
    digitalWrite(led_yellow_1_pin, isOn); \
    digitalWrite(led_red_2_pin, LOW);     \
    digitalWrite(led_green_2_pin, LOW);   \
    digitalWrite(led_yellow_2_pin, isOn); \
  }

uint8_t segment_number[] = {
    B11000000, // 0 // 0xC0
    B11111001, // 1 // 0xF9
    B10100100, // 2 // 0xA4
    B10110000, // 3 // 0xB0
    B10011001, // 4 // 0x99
    B10010010, // 5 // 0x92
    B10000010, // 6 // 0x82
    B11111000, // 7 // 0xF8
    B10000000, // 8 // 0x80
    B10010000  // 9 // 0x90
};

void setup_network();
void setup_firebase();
void setup_pin();
void set_light_2(int time, bool isRed);
void set_light_1(int time, bool isRed);
void led_handler();
void blink();

void set_led_right(String index);
void set_led_left(String index);

unsigned long sendDataPrevMillis = 0;

void setup()
{

  Serial.begin(115200);
  setup_network();
  setup_firebase();
  setup_pin();
}

void loop()
{
  if (Firebase.ready() &&
      (millis() - sendDataPrevMillis > 3000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    // Firebase.getString(fbdo, F("/test/string"));
    if (Firebase.getString(fbdo, "/traffic"))
    {

      String data = fbdo.to<const char *>();
      StaticJsonDocument<200> doc;
      deserializeJson(doc, data);

      is_all_yellow = doc["light_yellow"].as<int>();
      time_for_red = doc["time_red"].as<int>();
      time_for_green = doc["time_green"].as<int>();
      time_for_yellow = time_for_red - time_for_green;
      Serial.println("data: " + data);

      Serial.print("is_all_yellow: " + String(is_all_yellow));
      Serial.print(", time_for_red: " + String(time_for_red));
      Serial.println("time_for_green: " + String(time_for_green));
    }
    else
    {
      Serial.println(fbdo.errorReason());
    }
  }

  if (is_all_yellow)
  {
    blink();
  }
  else
  {
    led_handler();
  }
}

void set_led_right(String index)
{
  if (state_led_1 != old_state_led_1)
  {
    old_state_led_1 = state_led_1;
    Firebase.setInt(fbdo, F("/traffic/light_right"), index.toInt());
  }
}

void set_led_left(String index)
{
  if (state_led_2 != old_state_led_2)
  {
    old_state_led_2 = state_led_2;
    Firebase.setInt(fbdo, F("/traffic/light_left"), index.toInt());
  }
}

unsigned long time_last_blink = 0;
unsigned long time_blink_interval = 1000;
bool isShow = false;
void blink()
{
  if (millis() - time_last_blink > time_blink_interval)
  {
    time_last_blink = millis();
    set_all_yellow(isShow);
    isShow = !isShow;
    uint8_t numberToPrint[] = {segment_number[8], segment_number[8]};
    shiftRegister1.setAll(numberToPrint);
    shiftRegister2.setAll(numberToPrint);
    set_led_right(yellow_command);
    // turn_on_led_yellow_1;
    set_led_left(yellow_command);
state_led_1 = 0;
    state_led_2 = 0;
  }
}

void set_light_1(int time, bool isRed)
{
  int firstDigit = time / 10;
  int secondDigit = time % 10;
  if (!isRed)
  {
    int _time = time - time_for_yellow;
    if (_time < 0)
    {
      _time = time;
      set_led_right(yellow_command);
      turn_on_led_yellow_1;
      state_led_1 = 0;
    }
    else
    {
      set_led_right(green_command);
      turn_on_led_green_1;
      state_led_1 = 1;
    }
    firstDigit = _time / 10;
    secondDigit = _time % 10;
  }
  else
  {
    set_led_right(red_command);
    turn_on_led_red_1;
    state_led_1 = 2;
  }
  uint8_t numberToPrint[] = {segment_number[firstDigit],
                             segment_number[secondDigit]};
  shiftRegister2.setAll(numberToPrint);
}

void set_light_2(int time, bool isRed)
{
  int firstDigit = time / 10;
  int secondDigit = time % 10;
  if (!isRed)
  {
    int _time = time - time_for_yellow;
    if (_time < 0)
    {
      _time = time;
      turn_on_led_yellow_2;
      set_led_left(yellow_command);
      state_led_2 = 0;
    }
    else
    {
      turn_on_led_green_2;
      set_led_left(green_command);
      state_led_2 = 1;
    }
    firstDigit = _time / 10;
    secondDigit = _time % 10;
  }
  else
  {
    set_led_left(red_command);
    turn_on_led_red_2;
    state_led_2 = 2;
  }
  uint8_t numberToPrint[] = {segment_number[firstDigit],
                             segment_number[secondDigit]};
  shiftRegister1.setAll(numberToPrint);
}

unsigned long old_time_update_led = 0;
unsigned long time_interval_update_led = 1000;
int count_update_led = time_for_red;
bool is_light_1_red = true;

void led_handler()
{

  if (millis() - old_time_update_led > time_interval_update_led)
  {

    set_light_1(count_update_led, is_light_1_red);
    set_light_2(count_update_led, !is_light_1_red);

    count_update_led = count_update_led - 1;
    if (count_update_led < 0)
    {
      count_update_led = time_for_red;
      is_light_1_red = !is_light_1_red;
    }
    old_time_update_led = millis();
  }
}

void setup_network()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void setup_firebase()
{
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void setup_pin()
{

  pinMode(led_red_1_pin, OUTPUT); // cau hinh led don
  pinMode(led_yellow_1_pin, OUTPUT);
  pinMode(led_green_1_pin, OUTPUT);
  pinMode(led_red_2_pin, OUTPUT); // cau hinh led don
  pinMode(led_yellow_2_pin, OUTPUT);
  pinMode(led_green_2_pin, OUTPUT);
}
