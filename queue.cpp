#include "queue.h"

Queue::Queue(QMutex* mutex)
    : mutex(mutex)
{
}

Queue::Queue()
{
}

Queue& Queue::operator=(const Queue& other)
{
    if (this != &other)
    {
        QMutexLocker locker(mutex);
        customers = other.customers;
    }
    return *this;
}

void Queue::addCustomer(Customer customer)
{
    QMutexLocker locker(mutex);
    customers.push_back(customer);
}

bool Queue::getCustomer(Order order)
{
    QMutexLocker locker(mutex);

    if (customers.empty()) return false;

    if (customers.front().getOrder() != order) return false;

    customers.pop_front();
    emit customerRemoved();
    return true;
}

bool Queue::isEmpty() const
{
    QMutexLocker locker(mutex);
    return customers.empty();
}
