#include "window.h"

Window::Window(Order orderType, Queue& queue, QMutex* mutex, QLabel* customerLabel, QLabel* windowLabel)
    : orderType(orderType), queue(queue), mutex(mutex), customerLabel(customerLabel), windowLabel(windowLabel), disabled(false)
{
    ID = nextID++;
}
void start_serving();
void stop_serving();

void Window::run()
{
    running = true;
    while (running && !disabled)
    {
        if (!queue.getCustomer(orderType)) continue;
        emit this->customerVisibleChanged(true, customerLabel);
        QThread::msleep(orderType.getDuration() * 1000); // Konwersja z sekund na milisekundy
        emit this->customerVisibleChanged(false, customerLabel);
    }
}

void Window::disable()
{
    disabled = true;
}

void Window::stop_serving()
{
    running = false;
}


Order Window::get_order() const
{
    return orderType;
}

int Window::getID() const {
    return ID;
}

QLabel* Window::getWindowLabel() const {
    return windowLabel;
}

int Window::nextID = 1;
