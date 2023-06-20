#include "Order.h"

Order::Order(const QString& orderType, int orderDuration, QColor color)
    : orderType(orderType), orderDuration(orderDuration), color(color)
{
}

Order::Order() {}

bool Order::operator!=(const Order& other) const
{
    return (orderType != other.orderType) ||
           (orderDuration != other.orderDuration) ||
           (color != other.color);
}

bool Order::operator==(const Order& other) const
{
    return (orderType == other.orderType) &&
           (orderDuration == other.orderDuration) &&
           (color == other.color);
}

QString Order::getType() const
{
    return orderType;
}

int Order::getDuration() const
{
    return orderDuration;
}

QColor Order::getColor() const
{
    return color;
}
