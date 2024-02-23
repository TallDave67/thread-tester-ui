#ifndef EVENTDISPLAY_H
#define EVENTDISPLAY_H

#include <QString>

class QTextBrowser;

class EventDisplay
{
public:
    EventDisplay();

    void set_display(QTextBrowser *pDisplay);
    void clear_events();
    void add_event(QString & event);
    void set_events(QString & events);
    void display_events();

private:
    QTextBrowser *m_pDisplay;
    QString m_events;
};

#endif // EVENTDISPLAY_H
