#ifndef ORDER_H
#define ORDER_H

#include <QString>
#include <QColor>

class Order
{
public:
    Order(const QString& orderType, int orderDuration, QColor color);
    Order();

    bool operator!=(const Order& other) const;
    bool operator==(const Order& other) const;

    QString getType() const;
    int getDuration() const;
    QColor getColor() const;

private:
    QString orderType;
    int orderDuration;
    QColor color;
};

#endif // ORDER_H
