#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlTableModel>

#include <QString>

using namespace std;



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_pushButton_clicked();
    int countVisit(QString carNum);
    int calcPrice(QString carNum);
    int sumPrice(void);
    int emptySpot(void);
    QString checkIn(QString carNum);
    QString checkOut(QString carNum);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlTableModel  *model;
};

#endif // MAINWINDOW_H
