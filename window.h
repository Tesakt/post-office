#ifndef WINDOW_H
#define WINDOW_H


#include "Queue.h"
#include <QMutex>
#include <QWaitCondition>
#include <QThread>
#include <chrono>
#include <QLabel>

class Window : public QThread
{
    Q_OBJECT

public:
    Window(Order orderType, Queue& queue, QMutex* mutex, QLabel* customerLabel, QLabel* windowLabel);

    void run() override;
    void stop_serving();
    Order get_order() const;
    int getID() const;
    QLabel* getWindowLabel() const;
    void disable();

signals:
    void customerVisibleChanged(bool visible, QLabel* customerLabel);

private:
    static int nextID;
    int ID;
    Order orderType;
    Queue& queue;
    bool running;
    bool disabled;
    QMutex* mutex;
    QWaitCondition conditionVariable;
    QLabel* customerLabel;
    QLabel* windowLabel;
};

#endif // WINDOW_H
