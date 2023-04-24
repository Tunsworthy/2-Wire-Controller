#include "effects.h"
#include "hardware.h"

extern int currentEffect;
extern int lastEffect; 

void combination(PubSubClient &client)
{
    for (int i = 0; i < 3; i++) {
        //show colour 1
        digitalWrite(INPUT_1, HIGH);
        analogWrite(ENABLE_A, 255);
        digitalWrite(INPUT_2, LOW);
        delay(200);

        //show colour 2
        digitalWrite(INPUT_2, HIGH);
        analogWrite(ENABLE_A, 100);
        digitalWrite(INPUT_1, LOW);
        delay(200);
    }
}

void inWaves(PubSubClient &client)
{
    //show colour 1
    digitalWrite(INPUT_1, HIGH);
    analogWrite(ENABLE_A, 255);
    digitalWrite(INPUT_2, LOW);
    delay(200);

    //show colour 2
    digitalWrite(INPUT_2, HIGH);
    analogWrite(ENABLE_A, 100);
    digitalWrite(INPUT_1, LOW);
    delay(200);

    //show colour 1
    digitalWrite(INPUT_1, HIGH);
    analogWrite(ENABLE_A, 255);
    digitalWrite(INPUT_2, LOW);
    delay(200);

    //show colour 2
    digitalWrite(INPUT_2, HIGH);
    analogWrite(ENABLE_A, 100);
    digitalWrite(INPUT_1, LOW);
    delay(200);

    //show colour 1
    digitalWrite(INPUT_1, HIGH);
    analogWrite(ENABLE_A, 255);
    digitalWrite(INPUT_2, LOW);
    delay(200);
}

void sequential(PubSubClient &client)
{
    //show colour 1
    digitalWrite(INPUT_1, HIGH);
    analogWrite(ENABLE_A, 255);
    digitalWrite(INPUT_2, LOW);
    delay(500);

    //show colour 2
    digitalWrite(INPUT_1, LOW);
    digitalWrite(INPUT_2, HIGH);
    analogWrite(ENABLE_A, 255);
    delay(500);
}

void slowGlo(PubSubClient &client)
{
  digitalWrite(INPUT_1, LOW);
  digitalWrite(INPUT_2, LOW);
  analogWrite(ENABLE_A, 0);
  digitalWrite(INPUT_1, HIGH);

  while (true) {
    for (int i = 0; i < 255; i++) {
      analogWrite(ENABLE_A, i);
      delay(20);
      client.loop(); // check for incoming MQTT messages
      if(currentEffect != 3){
        return;
      }
    }

    for (int i = 255; i > 0; i--) {
      analogWrite(ENABLE_A, i);
      delay(20);
      client.loop(); // check for incoming MQTT messages
      if(currentEffect != 3){
        return;
      }
    }

    digitalWrite(INPUT_1, LOW);
    digitalWrite(INPUT_2, HIGH);

    for (int i = 0; i < 255; i++) {
      analogWrite(ENABLE_A, i);
      delay(20);
      client.loop(); // check for incoming MQTT messages
      if(currentEffect != 3){
        return;
      }
    }

    for (int i = 255; i > 0; i--) {
      analogWrite(ENABLE_A, i);
      delay(20);
      client.loop(); // check for incoming MQTT messages
      if(currentEffect != 3){
        return;
      }
    }

    digitalWrite(INPUT_2, LOW);
    digitalWrite(INPUT_1, HIGH);
  }

    client.loop(); // check for incoming MQTT messages
    if(currentEffect != 3){
      return;
    }
}

void chasingFlash(PubSubClient &client)
{
    digitalWrite(INPUT_1, LOW);
    digitalWrite(INPUT_2, LOW);
    //show colour 1
    digitalWrite(INPUT_1, HIGH);
    analogWrite(ENABLE_A, 255);
    digitalWrite(INPUT_2, LOW);
    delay(200);

    //show colour 2
    digitalWrite(INPUT_1, LOW);
    digitalWrite(INPUT_2, HIGH);
    analogWrite(ENABLE_A, 255);
    delay(200);

    //turn off both colours
    digitalWrite(INPUT_1, LOW);
    digitalWrite(INPUT_2, LOW);
    delay(200);

    //show colour 1
    digitalWrite(INPUT_1, HIGH);
    analogWrite(ENABLE_A, 255);
    digitalWrite(INPUT_2, LOW);
    delay(200);

    //show colour 2
    digitalWrite(INPUT_1, LOW);
    digitalWrite(INPUT_2, HIGH);
    analogWrite(ENABLE_A, 255);
    delay(200);

    //turn off both colours
    digitalWrite(INPUT_1, LOW);
    digitalWrite(INPUT_2, LOW);
    delay(200);
}

void slowFadeAlternate(PubSubClient &client) {
  // Set initial values
  int brightness = 0;
  int fadeAmount = 5;
  int led = 0;

  // Loop until both LEDs are fully lit
  while (brightness <= 255) {
    // Set the LED pin based on the current LED
    if (led == 0) {
      digitalWrite(INPUT_1, HIGH);
      digitalWrite(INPUT_2, LOW);
    } else {
      digitalWrite(INPUT_1, LOW);
      digitalWrite(INPUT_2, HIGH);
    }
      client.loop(); // check for incoming MQTT messages
    if(currentEffect != 7){
      return;
    }
    // Set the brightness of the current LED
    analogWrite(ENABLE_A, brightness);
    
    // Increment or decrement the brightness based on the fade amount
    brightness += fadeAmount;
    
    // If the brightness has reached the maximum or minimum, switch to the other LED
    if (brightness >= 255 || brightness <= 0) {
      led = (led + 1) % 2;
      fadeAmount *= -1;
    }
    
    delay(20);
  }
  client.loop(); // check for incoming MQTT messages
    if(currentEffect != 7){
      return;
  }
}


  void twinkleFlash(PubSubClient &client) {
  for (int i = 0; i < 20; i++) {
    digitalWrite(INPUT_1, HIGH);
    digitalWrite(INPUT_2, LOW);
    delay(50);
    digitalWrite(INPUT_1, LOW);
    digitalWrite(INPUT_2, HIGH);
    delay(50);
  }
}

void steadyOn(PubSubClient &client) {
  digitalWrite(INPUT_2, LOW);
  digitalWrite(INPUT_1, HIGH);
  analogWrite(ENABLE_A, 255);
}

void powerOff(PubSubClient &client){
  digitalWrite(INPUT_1, LOW);
  digitalWrite(INPUT_2, LOW);

}