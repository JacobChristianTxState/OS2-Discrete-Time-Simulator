#ifndef EVENT_H
#define EVENT_H

class Event{
  private:
    unsigned long time;
    int eType;
  public:
    Event(unsigned long, int);
    int getType();
    unsigned long getTime();
};

#endif
