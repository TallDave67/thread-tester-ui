#include "eventlogger.h"

#include <QString>
#include "eventdisplay.h"

EventLogger::EventLogger()
    : m_pDisplay(nullptr)
{
}

void EventLogger::set_display(EventDisplay *pDisplay)
{
    m_pDisplay = pDisplay;
}

void EventLogger::send_event(std::string & event)
{
    if(m_pDisplay)
    {
        QString q_event = event.c_str();
        m_pDisplay->add_event(q_event);
        m_pDisplay->display_events();
    }
}
