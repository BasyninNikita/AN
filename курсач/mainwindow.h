#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QtCore>
#include<QtGui>
#include <QMainWindow>
#include <QDir>
#include <QFileSystemModel>
#include <QtNetwork/QNetworkReply>
#include "header.h"
#include<QThread>

namespace Ui {
class MainWindow;
}

class another_thread : public QThread {
    Q_OBJECT
protected:
    void run();
};

class MainWindow : public QMainWindow, public ilogger
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void test();

private:
    Ui::MainWindow *ui;
    QFileSystemModel * model;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QString answer;
    QFileInfo path, name;

public slots:
    void getReplyFinished();
    void readyReadReply();
    void checkReport();
    void TakeQoute();

private slots:
    void on_listView_doubleClicked(const QModelIndex &index);

    void on_pushButton_2_clicked();//const QModelIndex &index);

    void on_pushButton_3_clicked();

    void on_listView_clicked(const QModelIndex &index);

    void on_action_4_triggered();

    virtual void log(const std::string& message);

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
};

#endif // MAINWINDOW_H
