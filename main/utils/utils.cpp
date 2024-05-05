#include "../config/config.h"
#include "utils.h"

double toDeg(double angle){
  return ((angle) *(180/PI));
}

double toRad(double angle){
  return ((angle) * (PI/180));
}

int getTickTime(FlightPhase phase){//map flight phases to tick times
  switch(phase){
  case ONPAD:
    return TICK_TIME_ONPAD;
  case ASCENDING:
    return TICK_TIME_ASCENDING;
  case DESCENDING:
    return TICK_TIME_DESCENDING;
  case POST_FLIGHT:
    return TICK_TIME_POST_FLIGHT;
  }
  return TICK_TIME_POST_FLIGHT;
}
