#include "neworderdialog.h"

NewOrderDialog::NewOrderDialog(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Pole do wprowadzania nazwy zamówienia
    QLabel *nameLabel = new QLabel("Nazwa zamówienia:");
        nameLineEdit = new QLineEdit;

    // Pole do wprowadzania czasu trwania zamówienia
    QLabel *durationLabel = new QLabel("Czas trwania zamówienia:");
        durationSpinBox = new QSpinBox;
    durationSpinBox->setMinimum(1);
    durationSpinBox->setMaximum(999);

    // Przyciski OK i Anuluj
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    layout->addWidget(nameLabel);
    layout->addWidget(nameLineEdit);
    layout->addWidget(durationLabel);
    layout->addWidget(durationSpinBox);
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

QString NewOrderDialog::getOrderName() const
{
    return nameLineEdit->text();
}

int NewOrderDialog::getOrderDuration() const
{
    return durationSpinBox->value();
}
