#include "../main/utils/utils.h"

double toDeg(double angle){
  return ((angle) *(180/PI));
}

double toRad(double angle){
  return ((angle) * (PI/180));
}

int getTickTime(FlightPhase phase){//map flight phases to tick times
  (void)phase;
  return 10;
}
