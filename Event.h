#ifndef EVENT_H
#define EVENT_H

#include "Process.h"
class Event
{
    public:
        int type;
        float time;
        Process p;
        Event(int, float);

        int getType();
        float getTime();
        void setProcess(Process);
        Process getProcess();

    protected:

    private:
};

#endif // EVENT_H
