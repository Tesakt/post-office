#ifndef SIMULATION_H
#define SIMULATION_H

#include <QSharedPointer>
#include <QVector>
#include <QColor>
#include <QMutex>
#include <QString>
#include "order.h"
#include "queue.h"
#include "window.h"

class Simulation
{
public:
    Simulation();

    Order addRandomCustomer();
    void addWindow(Order toAdd, QLabel* customerLabel, QLabel* windowLabel);
    void addOrder(Order toAdd);

    int size() const;

    QVector<Order> getOrders();
    Queue* getQueue();
    QSharedPointer<Window> getWindow(int toFind);
    QSharedPointer<Window> getWindowByCustomerLabel(QLabel* label);

    void startWindows(); // Uruchomienie wątków windows
    void stopWindows(); // Zatrzymanie wątków windows

private:
    bool running;
    Queue queue;
    QVector<Order> orders;
    QVector<QSharedPointer<Window>> windows;
    QMutex* mutex;
    int customer_count;
};

#endif // SIMULATION_H
