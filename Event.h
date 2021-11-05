#ifndef EVENT_H
#define EVENT_H

class Event{
  private:
    float time;
    bool eType;
  public:
    Event(float, bool);
    bool getType();
    float getTime();
};

#endif
