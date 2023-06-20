#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "neworderdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->ui->start_stop->setCheckable(true);

    this->connect(ui->start_stop, SIGNAL (clicked(bool)), this, SLOT (on_start_stop_clicked(bool)));
    this->connect(ui->window_btt, SIGNAL (clicked()), this, SLOT (new_window()));
    this->connect(ui->order_btt, SIGNAL (clicked()), this, SLOT (new_order()));
    this->connect(ui->customer_btt, SIGNAL (clicked()), this, SLOT (new_customer()));
    this->connect(simulation.getQueue(), &Queue::customerRemoved, this, &MainWindow::del_customer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_start_stop_clicked(bool checked)
{
    if (checked) {
        ui->start_stop->setText("Stop");
        this->simulation.startWindows();
    } else {
        ui->start_stop->setText("Start");
        this->simulation.stopWindows();
    }
}

void MainWindow::new_window()
{
    auto orders = simulation.getOrders();
    if (orders.size() == 0) {
        QMessageBox::information(nullptr, "Error", "Add new order first!");
        return;
    }

    QStringList items;
    for (const auto& order : orders)
    {
        QString orderType = order.getType();
        int orderDuration = order.getDuration();
        QString item = orderType;
        items << item + " (" + QString::number(orderDuration) + "s)";
    }

    // Wyświetlanie okna dialogowego do wprowadzania danych
    bool ok;
    QString selected = QInputDialog::getItem(this, "Create new post office window", "Choose type of order:", items, 0, false, &ok);
    if (!ok) return;

    Order selectedOrder;
    QColor color;
    for (auto& order : orders)
    {
        if (order.getType() + " (" + QString::number(order.getDuration()) + "s)" == selected)
        {
            selectedOrder = order;
            break;
        }
    }

    color = selectedOrder.getColor();

    QPixmap icon(":/icons/post_office.png");
    QLabel* label = new QLabel;
    label->setPixmap(icon);

    QGraphicsColorizeEffect* colorizeEffect = new QGraphicsColorizeEffect;
    colorizeEffect->setColor(color);
    label->setGraphicsEffect(colorizeEffect);
    label->setFixedSize(50, 50); // Ustaw rozmiar etykiety

    // Utwórz kontener dla etykiety i obsługiwanej kolejki klientów
    QWidget* container = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout(container);

    QPixmap customerIcon(":/icons/person.png");
    // Utwórz etykietę z ikoną klienta
    QLabel* customerLabel = new QLabel;
    customerLabel->setPixmap(customerIcon);

    QGraphicsColorizeEffect* customerColorizeEffect = new QGraphicsColorizeEffect;
    customerColorizeEffect->setColor(color);
    customerLabel->setGraphicsEffect(customerColorizeEffect);
    customerLabel->setFixedSize(50, 50); // Ustaw rozmiar etykiety
    customerLabel->setVisible(false); // Ustaw domyślnie jako niewidoczne

    layout->addWidget(customerLabel);
    layout->addWidget(label);

    container->setLayout(layout);
    ui->windows_layout->addWidget(container);

    simulation.addWindow(selectedOrder, customerLabel, label);

    QSharedPointer<Window> window = simulation.getWindow(simulation.size() - 1);
    if (window != nullptr) {
        connect(window.data(), &Window::customerVisibleChanged, this, &MainWindow::handleCustomerVisibleChanged);
    }

    label->installEventFilter(this);
}


void MainWindow::new_order()
{
    NewOrderDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) return;
    QString orderName = dialog.getOrderName();
    int orderDuration = dialog.getOrderDuration();
    QColor color = generateColor(simulation.getOrders().size());
    simulation.addOrder(Order(orderName, orderDuration, color));
}

void MainWindow::new_customer()
{
    QMutexLocker locker(mutex);
    QPixmap icon(":/icons/person.png");
    QLabel* label = new QLabel;
    label->setPixmap(icon);

    auto orders = simulation.getOrders();
    if (orders.empty()) {
        QMessageBox::information(nullptr, "Error", "Add new order first!");
        return;
    }

    // Wybierz losowy Order na podstawie wylosowanego indeksu
    Order randomOrder = simulation.addRandomCustomer();

    QGraphicsColorizeEffect* colorizeEffect = new QGraphicsColorizeEffect;
    colorizeEffect->setColor(randomOrder.getColor());
    label->setGraphicsEffect(colorizeEffect);
    label->setFixedSize(50, 50); // Ustaw rozmiar etykiety
    label->setAlignment(Qt::AlignRight);

    // Dodaj etykietę do układu
    queue_labels.prepend(label);
    ui->queue->insertWidget(0, label);
}

void MainWindow::handleCustomerVisibleChanged(bool visible, QLabel* customerLabel) {
    QMutexLocker locker(mutex);
    if (customerLabel != nullptr && customerLabel->isWidgetType()) {
        customerLabel->setVisible(visible);
    }
}

void MainWindow::del_customer()
{
    QMutexLocker locker(mutex);
    if (!queue_labels.isEmpty()) {
        QLabel* label = queue_labels.takeAt(queue_labels.count()-1); // Pobierz pierwszą ikonę z listy
        ui->queue->removeWidget(label); // Usuń etykietę z układu
        delete label; // Zwolnij pamięć
    }
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QLabel* label = qobject_cast<QLabel*>(obj);
        if (label) {
            // Usuń layout i wywołaj metodę remove() dla okienka
            QWidget* container = label->parentWidget();
            QLayout* layout = container->layout();
            ui->windows_layout->removeItem(layout);
            Window* window = simulation.getWindowByCustomerLabel(label).data();
            if (window) {
                window->disable();
            }
            container->deleteLater();
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

QColor MainWindow::generateColor(const int& s) const
{
    // Obliczanie wartości RGB na podstawie tekstu wejściowego
    uint32_t h = qHash(QString::number(s));
    uint8_t r, g, b;

    do {
        r = (h & 0xFF0000) >> 16;
        g = (h & 0x00FF00) >> 8;
        b = h & 0x0000FF;

        // Sprawdzanie, czy kolor jest jasny
        int brightness = (r + g + b) / 3;
        if (brightness > 128) {
            // Generowanie nowego hasha, jeśli kolor jest jasny
            h = qHash(QString::number(h));
        }
    } while ((r + g + b) / 3 > 192); // Powtarzaj, jeśli kolor jest jasny

    // Tworzenie obiektu QColor na podstawie wartości RGB
    QColor color(r, g, b);

    return color;
}
