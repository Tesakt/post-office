#ifndef QUEUE_H
#define QUEUE_H

#include "customer.h"
#include "order.h"
#include "qobject.h"
#include <QList>
#include <QMutex>
#include <QMutexLocker>
#include <condition_variable>
#include <algorithm>

class Queue : public QObject
{
    Q_OBJECT

public:
    Queue(QMutex* mutex);
    Queue();

    // opy assignment operator
    Queue& operator=(const Queue& other);

    void addCustomer(Customer customer);
    bool getCustomer(Order order);
    bool isEmpty() const;
signals:
    void customerRemoved();

private:
    QList<Customer> customers;
    QMutex* mutex;
};

#endif // QUEUE_H
