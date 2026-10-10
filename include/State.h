#ifndef STATE_H
#define STATE_H

#include <Arduino.h>
// ---------------------------------------------------------------------
// Live bay state
// ---------------------------------------------------------------------
extern String bayStatus;
extern float voltage, current, power, energyWh, temperature;

extern unsigned long sessionStartMs;
extern float predictedArrivalProb;
extern int predictedDurationMin;
extern int lastHourOfDay;

extern String loadDecision;
extern int throttleLevel;

extern float predictionThreshold;
extern int peakTariffStartHr;
extern int peakTariffEndHr;
extern bool overloadActive;
extern int overloadCurrentA; // 16A
extern int maxStationLoadW; // 3kW
extern bool manualOverrideActive; // indicates if manual override is active

#endif