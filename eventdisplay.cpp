#include "eventdisplay.h"
#include <QTextBrowser>

EventDisplay::EventDisplay()
    : m_pDisplay(nullptr)
{
}

void EventDisplay::set_display(QTextBrowser *pDisplay)
{
    m_pDisplay = pDisplay;
}

void EventDisplay::clear_events()
{
    m_events.clear();
}

void EventDisplay::add_event(QString & event)
{
    m_events += event;
    m_events += "\n";
}

void EventDisplay::set_events(QString & events)
{
    m_events = events;
}

void EventDisplay::display_events()
{
    if(m_pDisplay)
    {
        m_pDisplay->setPlainText(m_events);
    }
}
