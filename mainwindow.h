#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "simulation.h"
#include <QMainWindow>
#include <QLabel>
#include <QGraphicsColorizeEffect>
#include <QColor>
#include <QRandomGenerator>
#include <QInputDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void del_customer();

private slots:
    void on_start_stop_clicked(bool checked);

    void new_window();

    void new_order();

    void new_customer();

    void handleCustomerVisibleChanged(bool visible, QLabel* label);


signals:
    void labelClicked(QLabel* label);

private:
    bool eventFilter(QObject* obj, QEvent* event) override;
    QColor generateColor(const int& s) const;

    QMutex* mutex;
    Ui::MainWindow *ui;
    QList<QLabel*> queue_labels;
    Simulation simulation;
};
#endif // MAINWINDOW_H
