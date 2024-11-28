/* 
  Hogwarts sorting hat
  
  Version software 1.03 by Alexandr Kuznetsov
  28.11.2024 

  RP204 Core Library here: https://github.com/earlephilhower/arduino-pico

  th0 хмм... трудно... очень трудно... куда же определить вас?
  th1 а... ну все ясно... конечно же... решено!
  th2 куда мне вас определить?... посмотрим... знаю!
  th3 ах.. разумееться!... я знаю что с вами делать!
*/
//#include "WiFiNINA.h"

#include "h_music.h"
#include "gryffindor_1.h"
#include "hufflepuff_2.h"
#include "ravenclaw_3.h"
#include "slytherin_4.h"

#include "think_0.h"
#include "think_1.h"
#include "think_2.h"
#include "think_3.h"

#include <stdio.h>
#include <PWMAudio.h>

#define LED_PIN 25
#define BUTTON_PIN 6
#define AUDIO_PIN 0
#define RANDOM_PIN 28

PWMAudio pwm(AUDIO_PIN);
volatile int16_t *__ptr_sound;
volatile float __f_duration;
volatile uint32_t __time = 0;
volatile uint32_t t_led = 0;
volatile uint8_t rr = 0;

bool is_playing = false;

void __audio_callback() {
  while (pwm.availableForWrite()) {
    pwm.write(*__ptr_sound++);
  }
}

void play_sound(const uint8_t s[], uint32_t sz) {
  __ptr_sound = (int16_t *)s;
  __f_duration = sz * 0.5f / 22050 * 1000;

  pwm.begin(22050);
  pwm.onTransmit(__audio_callback);
  delay(__f_duration);
  pwm.end();
}

void setup() {
  randomSeed(analogRead(RANDOM_PIN));

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Hogwarts sorting hat");

  pwm.begin(22050);
  play_sound(h_music, sizeof(h_music));
}

void loop() {
  __time = millis();

  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(100);
    while(digitalRead(BUTTON_PIN) == LOW);
    delay(100);

    rr = random(0, 255) % 4;
    switch (rr) {
      case 0: 
        Serial.println("think 0");
        play_sound(think_0, sizeof(think_0));
        break;
      case 1: 
        Serial.println("think 1");
        play_sound(think_1, sizeof(think_1));
        break;
      case 2: 
        Serial.println("think 3");
        play_sound(think_2, sizeof(think_2));
        break;
      case 3: 
        Serial.println("think 4");
        play_sound(think_3, sizeof(think_3));
        break;
    }

    delay(1000);

    rr = random(0, 255) % 4;
    switch (rr) {
      case 0: 
        Serial.println("gryffindor!");
        play_sound(gryffindor_1, sizeof(gryffindor_1));
        break;
      case 1: 
        Serial.println("hufflepuff!");
        play_sound(hufflepuff_2, sizeof(hufflepuff_2));
        break;
      case 2: 
        Serial.println("ravenclaw!");
        play_sound(ravenclaw_3, sizeof(ravenclaw_3));
        break;
      case 3: 
        Serial.println("slytherin!");
        play_sound(slytherin_4, sizeof(slytherin_4));
        break;
    }
  }

  if (__time - t_led > 400) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    t_led = __time;
  }
}
