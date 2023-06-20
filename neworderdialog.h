#ifndef NEWORDERDIALOG_H
#define NEWORDERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>

class NewOrderDialog : public QDialog
{
    Q_OBJECT

public:
    NewOrderDialog(QWidget *parent = nullptr);
    QString getOrderName() const;
    int getOrderDuration() const;

private:
    QLineEdit *nameLineEdit;
    QSpinBox *durationSpinBox;
};

#endif // NEWORDERDIALOG_H
