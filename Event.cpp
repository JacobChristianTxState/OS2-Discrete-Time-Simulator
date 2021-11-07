#include "Event.h"

Event::Event(float t, int ty){
  time = t;
  eType = ty;
}

int Event::getType(){
  return eType;
}

float Event::getTime(){
  return time;
}



