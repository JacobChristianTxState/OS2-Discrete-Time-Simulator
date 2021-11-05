#include "Event.h"

Event::Event(float t, bool ty){
  time = t;
  eType = ty;
}

bool Event::getType(){
  return eType;
}

float Event::getTime(){
  return time;
}

