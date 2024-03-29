#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./test_ids.h"

#include <QEvent>
#include <QProcess>

constexpr QEvent::Type USER_EVENT_STAGE_TEST = QEvent::User;
constexpr QEvent::Type USER_EVENT_DO_TEST = QEvent::Type(QEvent::User + 1);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("thread-tester-ui");

    // Test Events
    event_display.set_display(ui->textBrowser_events);

    // Lock Order group
    ui->buttonGroup_lockOrder->setId(ui->radioButton_lockOrderSame, TEST_LOCK_ORDER_SAME);
    ui->buttonGroup_lockOrder->setId(ui->radioButton_lockOrderInverted, TEST_LOCK_ORDER_INVERTED);
    ui->radioButton_lockOrderSame->setChecked(true);
    connect(ui->lockOrder_pushButton, &QPushButton::released, this, &MainWindow::handleButton_testLockOrder);

    // Sync group
    ui->buttonGroup_sync->setId(ui->radioButton_syncLatch, TEST_SYNC_LATCH);
    ui->buttonGroup_sync->setId(ui->radioButton_syncBarrier, TEST_SYNC_BARRIER);
    ui->radioButton_syncLatch->setChecked(true);
    connect(ui->sync_pushButton, &QPushButton::released, this, &MainWindow::handleButton_testSync);

    // Promise group
    ui->buttonGroup_promise->setId(ui->radioButton_matrixCalc, TEST_PROMISE_MATRIX_CALC);
    ui->buttonGroup_promise->setId(ui->radioButton_withException, TEST_PROMISE_WITH_EXCEPTION);
    ui->radioButton_matrixCalc->setChecked(true);
    connect(ui->promise_pushButton, &QPushButton::released, this, &MainWindow::handleButton_testPromise);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Button handlers
void MainWindow::handleButton_testLockOrder()
{
    runTest(ui->lockOrder_groupBox->title(), ui->buttonGroup_lockOrder->checkedId(), ui->buttonGroup_lockOrder->checkedButton()->text());
}

void MainWindow::handleButton_testSync()
{
    runTest(ui->sync_groupBox->title(), ui->buttonGroup_sync->checkedId(), ui->buttonGroup_sync->checkedButton()->text());
}

void MainWindow::handleButton_testPromise()
{
    runTest(ui->promise_groupBox->title(), ui->buttonGroup_promise->checkedId(), ui->buttonGroup_promise->checkedButton()->text());
}

// Tests
void MainWindow::runTest(QString group, int id, QString name)
{
    test_group = group;
    test_id = id;
    test_name = name;
    //
    QString test_event = "START TEST ";
    test_event += test_group;
    QString test_id_str;
    test_id_str.setNum(test_id);
    test_event += " (";
    test_event += test_id_str;
    test_event += ") ";
    test_event += test_name;
    //
    event_display.clear_events();
    event_display.add_event(test_event);
    event_display.display_events();

    // allocate a QEvent on the heap and post it to stage the test
    disableAllButtons();
    QEvent* stage_test = new QEvent(USER_EVENT_STAGE_TEST);
    QCoreApplication::postEvent(this, stage_test);
}

void MainWindow::stageTest()
{
    // allocate a QEvent on the heap and post it to do the test
    QEvent* do_test = new QEvent(USER_EVENT_DO_TEST);
    QCoreApplication::postEvent(this, do_test);
}

void MainWindow::doTest()
{
    // ***** launch external process to do the test

    // Create a QProcess object
    QProcess process;

    // Set the command to execute and arguments
    QString program = "../thread-tester-ui-engines/";
    if (test_id > TEST_PROMISE_MATRIX_CALC)
    {
        program += "thread-tester-engine";
    }
    else
    {
        program += "thread-tester-engine-promises";
    }

    QString test_id_str;
    test_id_str.setNum(test_id);
    QStringList arguments = { test_id_str };

    // Start the process
    process.start(program, arguments);

    // Wait for the process to finish
    if (!process.waitForFinished())
    {
        qDebug() << "Failed to execute the process:" << process.errorString();
    }
    else
    {
        // Read the output of the process
        QByteArray output = process.readAllStandardOutput();
        QString test_events = QString::fromUtf8(output);
        event_display.set_events(test_events);
        event_display.display_events();
    }

    enableAllButtons();
}

void MainWindow::disableAllButtons()
{
    ui->lockOrder_pushButton->setDisabled(true);
}

void MainWindow::enableAllButtons()
{
    ui->lockOrder_pushButton->setDisabled(false);
}

bool MainWindow::event(QEvent* event)
{
    if (event->type() == USER_EVENT_STAGE_TEST)
    {
        // Handle the custom event
        stageTest();
        return true; // Event handled
    }
    else if (event->type() == USER_EVENT_DO_TEST)
    {
        // Handle the custom event
        doTest();
        return true; // Event handled
    }
    return QMainWindow::event(event);
}
