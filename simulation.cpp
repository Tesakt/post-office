#include "simulation.h"

Simulation::Simulation()
{
    mutex = new QMutex;
    queue = Queue(mutex);
    customer_count = 0;
}

Order Simulation::addRandomCustomer()
{
    QMutexLocker locker(mutex);

    if (orders.size() == 0) addOrder(Order("Letter", 4, QColor(255, 0, 0)));

    // Wygenerowanie losowego indeksu od 0 do rozmiaru orders
    int randomIndex = rand() % orders.size();

    // Pobranie losowego zamówienia z wektora orders
    Order randomOrder = orders[randomIndex];
    QString customer_name = QString::number(customer_count++);

    queue.addCustomer(Customer(customer_name, randomOrder));

    return randomOrder;

}

void Simulation::addOrder(Order toAdd)
{
    orders.append(toAdd);
}

void Simulation::addWindow(Order toAdd, QLabel* customerLabel, QLabel* windowLabel)
{
    windows.append(QSharedPointer<Window>(new Window(toAdd, queue, mutex, customerLabel, windowLabel)));
}

QVector<Order> Simulation::getOrders()
{
    return orders;
}

Queue* Simulation::getQueue()
{
    return &queue;
}

QSharedPointer<Window> Simulation::getWindow(int toFind)
{
    return windows[toFind];
}

QSharedPointer<Window> Simulation::getWindowByCustomerLabel(QLabel* label)
{
    for (const auto& window : windows)
    {
        if (window->getWindowLabel() == label)
        {
            return window;
        }
    }
    return nullptr;
}

void Simulation::startWindows()
{
    for (const auto& window : windows)
    {
        window->start();   // Uruchom wątek
    }
}

void Simulation::stopWindows()
{
    for (const auto& window : windows)
    {
        window->stop_serving();   // Zatrzymaj wątek
    }
}

int Simulation::size() const
{
    return windows.size();
}
