#ifndef EVENTLOGGER_H
#define EVENTLOGGER_H

#include "thread_system/eventloggerinterface.h"

class EventDisplay;

class EventLogger : public EventLoggerInterface
{
public:
    EventLogger();

    void set_display(EventDisplay *pDisplay);
    void send_event(std::string & event);

private:
    EventDisplay *m_pDisplay;
};

#endif // EVENTLOGGER_H
