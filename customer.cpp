#include "customer.h"

Customer::Customer(const QString& name, Order order)
    : name(name), order(order)
{
}

QString Customer::getName() const
{
    return name;
}

Order Customer::getOrder() const
{
    return order;
}
