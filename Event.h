#ifndef EVENT_H
#define EVENT_H

class Event{
  private:
    float time;
    int eType;
  public:
    Event(float, int);
    int getType();
    float getTime();
};

#endif
