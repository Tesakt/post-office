#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "order.h"
#include <QString>

class Customer
{
public:
    Customer(const QString& name, Order order);

    QString getName() const;
    Order getOrder() const;

private:
    QString name;
    Order order;
};

#endif // CUSTOMER_H
