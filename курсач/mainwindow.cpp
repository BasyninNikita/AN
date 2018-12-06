#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include"header.h"
#include<QtCore>
#include <QMessageBox>
#include <QDebug>
#include<qstring.h>
#include <QString>
#include<QScrollBar>

QFileInfo fileinfor;
antivirus_scaner scaner;
another_thread another;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model=new QFileSystemModel(this);
    model->setFilter(QDir::QDir::AllEntries);
    model->setRootPath("");
    ui->listView->setModel(model);
    scaner.set_logger(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{

    QListView * listview = (QListView *) sender();
    QFileInfo fileinfo = model->fileInfo(index);
    if(fileinfo.fileName()=="..")
    {
        QDir dir = fileinfo.dir();
        dir.cdUp();
        listview->setRootIndex(model->index(dir.absolutePath()));
    }
    else if(fileinfo.fileName()==".")
    {
        listview->setRootIndex(model->index(""));
    }
    else if(fileinfo.isDir())
    {
        listview->setRootIndex(index);
    }
}
void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    QModelIndexList lst=ui->listView->selectionModel()->selectedIndexes();
    fileinfor = model->filePath(lst.at(0));
    QString str=fileinfor.dir().absolutePath() + '/' + fileinfor.fileName();
    qDebug() << str;
}

void another_thread::run()
{
    QString str=fileinfor.dir().absolutePath() + '/' + fileinfor.fileName();
    //scaner.scan_filesystem(str.toLocal8Bit().constData());
    //if(scaner.counter_()==0) QMessageBox::information(this,"увдеомление","Ну с компом все нормально,но вы на глисты проверьтесь");
    //else QMessageBox::information(this,"увдеомление","Весь комп засрал,опять убирать за тобой");
    //std::cout<<scaner.counter_()<<std::endl;
    //QString string = QString::number(scaner.counter_());
    //QMessageBox::information(this,"увдеомление","Moved " + string + " files");
    scaner.new_count();
}


void MainWindow::on_pushButton_2_clicked()
{
    another.start();
    //while(!another.isFinished());
}

void MainWindow::on_pushButton_3_clicked()
{
    scaner.registry();
    QMessageBox::information(this,"уведомление","Реестр тоже в порядке,а вот проктолог ждет");
}

void MainWindow::checkReport() {
    answer = QString::fromUtf8(reply->readAll());
    if(!answer.contains("Scan finished, information embedded")) {
        scaner.logger->log("Error with report. Maybe you haven`t chosen file for scanning, have chosen not .exe file or report hasn`t made yet\n");
        return;
    }
    QStringList list1 = answer.split("\"positives\": ");
    QStringList list2 = list1[1].split(",");
    QString checking_file = path.dir().absolutePath() + '/' + name.fileName();

    if(list2[0] == '0') {
        QString str = "Don`t worry, " + checking_file + " checked file is not dangerous";
        scaner.logger->log(str.toUtf8().constData());
    }
    else {
        QString str = "File " + checking_file + " is dangerous, so it has been replaced in carantin";
        scaner.move_file(path.dir().absolutePath().toUtf8().constData(), name.fileName().toUtf8().constData());
        scaner.logger->log(str.toUtf8().constData());
    }
}

void MainWindow::on_action_4_triggered()
{
    QUrl apiUrl("https://www.virustotal.com/vtapi/v2/file/report");
    QByteArray str = "apikey=d11f274bf1bfd35a3ebf539e328e5cf6b8c67f964036f5cac7100473da64eb69&resource=";
    str += answer;
    QByteArray requestString(str);
    QNetworkRequest request(apiUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    reply = manager.post(request, requestString);
    connect(reply, SIGNAL(finished()),this, SLOT(getReplyFinished()));
    connect(reply, SIGNAL(readChannelFinished()), this, SLOT(checkReport()));
}


void MainWindow::log(const std::string& message)
{
    QString str = QString::fromStdString(message);
    ui->textEdit->insertPlainText(str);
    QScrollBar* sb = ui->textEdit->verticalScrollBar();
    sb->setValue(sb->maximum());
    repaint();
}

void MainWindow::on_pushButton_4_clicked()
{
    qsrand(QDateTime::currentMSecsSinceEpoch()); // Установка базового числа для отсчёта рандома в qrand
    int gus = (qrand() % ((6 + 1) - 1) + 1);
    if(gus==1) scaner.logger->log("ЗАПУСКАЕМ \n░ГУСЯ░▄▀▀▀▄░РАБОТЯГИ░░\n▄███▀░◐░░░▌░░░░░░░\n░░░░▌░░░░░▐░░░░░░░\n░░░░▐░░░░░▐░░░░░░░\n░░░░▌░░░░░▐▄▄░░░░░\n░░░░▌░░░░▄▀▒▒▀▀▀▀▄\n░░░▐░░░░▐▒▒▒▒▒▒▒▒▀▀▄\n░░░▐░░░░▐▄▒▒▒▒▒▒▒▒▒▒▀▄\n░░░░▀▄░░░░▀▄▒▒▒▒▒▒▒▒▒▒▀▄\n░░░░░░▀▄▄▄▄▄█▄▄▄▄▄▄▄▄▄▄▄▀▄\n░░░░░░░░░░░▌▌░▌▌░░░░░\n░░░░░░░░░░░▌▌░▌▌░░░░░\n░░░░░░░░░▄▄▌▌▄▌▌░░░░░ \n");
    else if(gus==2) scaner.logger->log("ЗАПУСКАЕМ░░\n░ГУСЯ░▄▀▀▀▄░ГИДРУ░░\n▄███▀░◐░▄▀▀▀▄░░░░░░\n░░▄███▀░◐░░░░▌░░░\n░░░▐░▄▀▀▀▄░░░▌░░░░\n▄███▀░◐░░░▌░░▌░░░░\n░░░░▌░░░░░▐▄▄▌░░░░░\n░░░░▌░░░░▄▀▒▒▀▀▀▀▄\n░░░▐░░░░▐▒▒▒▒▒▒▒▒▀▀▄\n░░░▐░░░░▐▄▒▒▒▒▒▒▒▒▒▒▀▄\n░░░░▀▄░░░░▀▄▒▒▒▒▒▒▒▒▒▒▀▄\n░░░░░░▀▄▄▄▄▄█▄▄▄▄▄▄▄▄▄▄▄▀▄\n░░░░░░░░░░░▌▌░▌▌░░░░░\n░░░░░░░░░░░▌▌░▌▌░░░░░\n░░░░░░░░░▄▄▌▌▄▌▌░░░░░\n");
    else if(gus==3) scaner.logger->log("ЗАПУСКАЕМ \n░ШУРИКЕН░▄▀▀▀▄░РАБОТЯГИ░░\n▓▓▓▓▓▓▀▀░░░░░░▀▀▓▓▓▓▓▓\n▓▓▓▀░░░░░▄██▄░░░░░▀▓▓▓\n ▓▓░░░░░▄▄██▀░░░░░░░░▓▓\n ▓░░░░░▄██▀░░░▄█▄░░░░░▓\n▌░░░░░▀██▄▄▄█████▄░░░▐\n░░▄▄▄░░░▀████▀░▀▀██▄░░\n░░▀██▄░▄▄████▄░░░▀▀▀░░\n▌░░░▀█████▀▀▀██▄░░░░░▐\n▓░░░░░▀█▀░░░▄██▀░░░░░▓\n▓▓░░░░░░░░▄██▀░░░░░░▓▓\n▓▓▓▄░░░░░▀█▀▀░░░░░▄▓▓▓\n");
    else if(gus==4) scaner.logger->log("────────▄──────────────▄\n────────▌▒█───────────▄▀▒▌\n────────▌▒▒▀▄───────▄▀▒▒▒▐\n───────▐▄▀▒▒▀▀▀▀▄▄▄▀▒▒▒▒▒▐\n─────▄▄▀▒▒▒▒▒▒▒▒▒▒▒█▒▒▄█▒▐\n───▄▀▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▀██▀▒▌\n──▐▒▒▒▄▄▄▒▒▒▒▒▒▒▒▒▒▒▒▒▀▄▒▒▌\n──▌▒▒▐▄█▀▒▒▒▒▄▀█▄▒▒▒▒▒▒▒█▒▐\n─▐▒▒▒▒▒▒▒▒▒▒▒▌██▀▒▒▒▒▒▒▒▒▀▄▌\n─▌▒▀▄██▄▒▒▒▒▒▒▒▒▒▒▒░░░░▒▒▒▒▌\n▌▀▐▄█▄█▌▄▒▀▒▒▒▒▒▒░░░░░░▒▒▒▐\n▐▒▀▐▀▐▀▒▒▄▄▒▄▒▒▒▒▒░░░░░░▒▒▒▒▌\n▐▒▒▒▀▀▄▄▒▒▒▄▒▒▒▒▒▒░░░░░░▒▒▒▐\n─▌▒▒▒▒▒▒▀▀▀▒▒▒▒▒▒▒▒░░░░▒▒▒▒▌\n─▐▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▐\n──▀▄▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▄▒▒▒▒▌\n────▀▄▒▒▒▒▒▒▒▒▒▒▄▄▄▀▒▒▒▒▄▀\n───▐▀▒▀▄▄▄▄▄▄▀▀▀▒▒▒▒▒▄▄▀\n──▐▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▀▀\n");
    else if(gus==5) scaner.logger->log("ВЗГЛЯНИ ░ВОКРУГ, \n░ОГЛЯНИСЬ░НАЗАД! \n░ГУСИ░▄▀▀▀▄░С░ТОБОЮ░░░ \n░░░░▀░░░◐░▀███▄░░░░ \n░░░░▌░░░░░▐░░░░░░░ \n░░░░▐░░░░░▐░░░░░░░ \n░░░░▌░░░░░▐▄▄░░░░░\n░░░░▌░░░░▄▀▒▒▀▀▀▀▄ \n░░░▐░░░░▐▒▒▒▒▒▒▒▒▀▀▄ \n░░░▐░░░░▐▄▒▒▒▒▒▒▒▒▒▒▀▄ \n░░░░▀▄░░░░▀▄▒▒▒▒▒▒▒▒▒▒▀▄ \n░░░░░░▀▄▄▄▄▄█▄▄▄▄▄▄▄▄▄▄▄▀▄ \n░СВЯЗАТЬСЯ░░▌▌░▌▌░░░░░ \n░░░ХОТЯТ░░░░▌▌░▌▌░░░░░ \n░░░░░░░░░░░▄▄▌▌▄▌▌░░\n");
    else if(gus==6) scaner.logger->log(" ──────▄▌▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌\n ───▄▄██▌█░ВЕЗЁМ▄▀▀▀▄░ГУСЕЙ░░░░░░░\n ───████▌█▄███▀░◐░▄▀▀▀▄░░РАБОТЯГИ░\n ──██░░█▌█░░▄███▀░◐░░▄▀▀▀▄░░░░░░░\n ─██░░░█▌█░░░░▐░▄▀▀▀▌░░░░◐░▀███▄░\n ▄██████▌█▄███▀░◐░░░▌░░░░░▐░░░░░░\n ███████▌█░░░░▌░░░░░▌░░░░░▐░░░░░░\n ███████▌█▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌\n ▀(@)▀▀▀▀▀▀▀(@)(@)▀▀▀▀▀▀▀▀▀▀▀▀▀(@)▀(@)\n");


}

void MainWindow::on_pushButton_5_clicked()
{
    ui->textEdit->clear();
}

void MainWindow::on_pushButton_6_clicked()
{

}

void MainWindow::getReplyFinished() {
    reply->deleteLater();
}

void MainWindow::readyReadReply() {
    answer = QString::fromUtf8(reply->readAll());
    scaner.logger->log(answer.toUtf8().constData());
    //QStringList list1 = answer.split("\"resource\": \"");
    //QStringList list2 = list1[1].split("\"");
    //answer = list2[0];
}

void MainWindow::test() {
    QUrl apiUrl("https://www.virustotal.com/vtapi/v2/file/scan");
    QByteArray str = "apikey=d11f274bf1bfd35a3ebf539e328e5cf6b8c67f964036f5cac7100473da64eb69&file=";
    str += fileinfor.dir().absolutePath() + '/' + fileinfor.fileName();

    path = fileinfor.dir().absolutePath();
    name = fileinfor.fileName();
    QString inf = fileinfor.dir().absolutePath() + '/' + fileinfor.fileName();
    QFileInfo finf=inf;
    if(!(finf.isFile())){ scaner.logger->log("File isn't chosen \n"); return;}
    else
    {
        QByteArray requestString(str);
        QNetworkRequest request(apiUrl);
        request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        reply = manager.post(request, requestString);
        connect(reply, SIGNAL(finished()),this, SLOT(getReplyFinished()));
        connect(reply, SIGNAL(readChannelFinished()), this, SLOT(readyReadReply()));
        scaner.logger->log("File is scanning by VirusTotal...\nIf you want to know result of scanning, push \"Take Report\"\n");

    }
}

void MainWindow::on_pushButton_7_clicked()
{
    test();
}

void MainWindow::TakeQoute() {
    QString quote = QString::fromUtf8(reply->readAll());
    QStringList list1 = quote.split("quoteText>");
    QStringList list2 = list1[1].split('<');
    quote = list2[0];
    list1 = list1[2].split("Author>");
    list2 = list1[1].split('<');
    quote += ", " + list2[0] + '\n';
    scaner.logger->log(quote.toUtf8().constData());
}

void MainWindow::on_pushButton_8_clicked()
{
    QUrl apiUrl("http://www.forismatic.com/api/1.0/");
    QByteArray requestString("method=getQuote&format=xml");
    QNetworkRequest request(apiUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    reply = manager.post(request, requestString);
    connect(reply, SIGNAL(finished()),this, SLOT(getReplyFinished()));
    connect(reply, SIGNAL(readChannelFinished()), this, SLOT(TakeQoute()));
}
