#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "eventdisplay.h"

class QEvent;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Button handlers
    void handleButton_testLockOrder();
    void handleButton_testSync();

    // Tests
    void runTest(QString group, int id, QString name);

protected:
    bool event(QEvent* event) override;

private:
    void stageTest();
    void doTest();

    void disableAllButtons();
    void enableAllButtons();

private:
    Ui::MainWindow *ui;
    QString test_group;
    int test_id;
    QString test_name;

    // Test Events
    EventDisplay event_display;
};
#endif // MAINWINDOW_H
