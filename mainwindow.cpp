#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <QString>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QVariant>
#include  <qtextstream.h>
#include <string.h>


#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "드라이버" << QSqlDatabase::drivers();
    qDebug() << QCoreApplication::libraryPaths();
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");      // IP 또는 DNS Host name
    db.setPort(3306);
    db.setDatabaseName("testdb"); // DB명
    db.setUserName("root");     // 계정 명
    db.setPassword("iot123");     // 계정 Password

    db.open();

    model = new QSqlTableModel(this);
    model->setTable("출입기록");    // 테이블명
    ui->tableView->setModel(model);


    //https://1d1cblog.tistory.com/122 이거참고




//    QSqlQuery query( "select count(*) from 출입기록 where id=1");//테이블이름을 지정

//    if ( !query.isActive() )
//        cout << "Error" << endl;
//    else while (query.next())
//    {
//        int val = query.value(0).toInt();
//        //QString word = query.value(1).toString();
//        //cout << QString( "%1\n".arg(val));
//        cout << val << endl;
//    }

    QString aa = "aaa";
    int val2 = countVisit(aa);
    cout << " 출입횟수 :  "<< val2 << endl;
    //ui->label->setText(val2);
    ui->lcdNumber->display(val2);

    int val3 = calcPrice(aa);
    cout << " 요금(사용자) :  "<< val3 << endl;
    ui->lcdNumber_2->display(val3);

    int val4 = sumPrice();
    cout << " 하루수익합계 :  "<< val4 << endl;
    ui->lcdNumber_3->display(val4);

    int val5 = emptySpot();
    cout << " 여석 :  "<< val5 << endl;
    //ui->lcdNumber_4->display(val5);

    //QString carNum1 = "bbb";
    QString carNum2 = "ccc";
//    QString val6 = checkIn(carNum2);
//    cout << "차량등록 " << val6.toStdString() <<endl;

    int val7 = emptySpot();
    cout << " 여석 :  "<< val7 << endl;
    ui->lcdNumber_4->display(val7);

    QString val8 = checkOut(carNum2);
    cout << "나간차: " << val8.toStdString() << endl;




}



MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_clicked()
{
    model->select();
}

int MainWindow::countVisit(QString carNum)
{
    int val;

    QSqlQuery query("select count(*) from 출입기록 where 등록차량번호='" + carNum + "'");

    if ( !query.isActive() )
      cout << "Query Error" << endl;
    else while (query.next())
    {
        val = query.value(0).toInt();
    }

   return val;
}

int MainWindow::calcPrice(QString carNum)
{
    int val;

    QSqlQuery query("select 요금 from 출입기록 where 등록차량번호='" + carNum + "' and id in(select max(id) from 출입기록 where 등록차량번호='" + carNum + "')");

    if ( !query.isActive() )
      cout << "Query Error" << endl;
    else while (query.next())
    {
        val = query.value(0).toInt();
    }

   return val;
}

int MainWindow::sumPrice(void)
{
    int val;

    QSqlQuery query("select SUM(요금) from 출입기록");

    if ( !query.isActive() )
      cout << "Query Error" << endl;
    else while (query.next())
    {
        val = query.value(0).toInt();
    }

   return val;
}

int MainWindow::emptySpot(void)
{
    int val;

    QSqlQuery query("select COUNT(*) from 출입기록 where 나간시간 IS NULL");

    if ( !query.isActive() )
      cout << "Query Error" << endl;
    else while (query.next())
    {
        val = query.value(0).toInt();
    }

    val = 50 - val;

   return val;
}

QString MainWindow::checkIn(QString carNum)
{
    //등록여부확인
    QString checkRegist;
    int val;
    QSqlQuery query("select COUNT(*) from 등록차량 where 등록차량번호='" + carNum + "'");

    if ( !query.isActive() )
      cout << "Query Error" << endl;
    else while (query.next())
    {
        val = query.value(0).toInt();
    }

    if (val > 0)
    {
        checkRegist = "등록";
    }
    else checkRegist= "미등록";


    //QSqlQuery
    QSqlQuery query1("insert into 출입기록(등록차량번호,등록여부,들어온시간) values('" +carNum + "', '" + checkRegist + "', now())" );

    return carNum;

}

QString MainWindow::checkOut(QString carNum)
{
    int time;

    //id 저장
    int idnum;
    QSqlQuery query11("select max(id) from 출입기록 where 등록차량번호='" +carNum + "'");
    if ( !query11.isActive() )
      cout << "Query Error1" << endl;
    else while (query11.next())
    {
        idnum = query11.value(0).toInt();
        cout<< "id: " << idnum<< endl;
    }

    // 나간시간 저장
    QSqlQuery query10("update 출입기록 set 나간시간=now() where id = " + QString::number(idnum) );
    if ( !query10.isActive() )
      cout << "Query Error0" << endl;
    else while (query10.next())
    {
        cout<< "나간시간" << endl;
    }

    //등록여부확인
    int val=0;
    QSqlQuery query12("select count(*) from 출입기록 where 등록차량번호='" + carNum + "' and 등록여부='미등록'");

    if ( !query12.isActive() )
      cout << "Query Error2" << endl;
    else while (query12.next())
    {
        val = query12.value(0).toInt();
        cout << "regist val : " << val  <<endl;
    }

    if (val > 0)
    {

        // 요금계산

        QString time2;
        QSqlQuery query13("select (나간시간-들어온시간)/1 from 출입기록 where id = " + QString::number(idnum));

        if ( !query13.isActive() )
          cout << "Query Error3" << endl;
        else while (query13.next())
        {
            time2 = query13.value(0).toString();
            cout <<"time: " << time2.toStdString() << endl;
        }
        time = time2.toInt();
        int price = 1000 * (time/10000);

        QSqlQuery query14("update 출입기록 set 요금 = " + QString::number(price) + " where id = " + QString::number(idnum));
        if ( !query14.isActive() )
          cout << "Query Error 4" << endl;
        else while (query14.next())
        {
            cout << "update" << endl;
        }

    }

    return carNum;

}
