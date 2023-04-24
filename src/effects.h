#ifndef EFFECTS_H
#define EFFECTS_H

#include <Arduino.h>
#include <PubSubClient.h>

void combination(PubSubClient &client);
void inWaves(PubSubClient &client);
void sequential(PubSubClient &client);
void slowGlo(PubSubClient &client);
void chasingFlash(PubSubClient &client);
void twinkleFlash(PubSubClient &client);
void steadyOn(PubSubClient &client);
void slowFadeAlternate(PubSubClient &client);
void powerOff(PubSubClient &client);

#endif