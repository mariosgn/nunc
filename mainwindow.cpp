#include "diary.h"
#include "entry.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDateTime>
#include <QCryptographicHash>

#include <openssl/aes.h>
#include <openssl/evp.h>

//http://security.stackexchange.com/questions/29106/openssl-recover-key-and-iv-by-passphrase


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mb_DropEvents(true)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog	);


    mp_Diary = new Diary(this);
    connect(mp_Diary, SIGNAL(error(QString)), this, SLOT(err(QString)));
    connect(mp_Diary, SIGNAL(log(QString)), this, SLOT(log(QString)));

    mp_Diary->setPassword("ASD");

    mp_Diary->load("/home/mario/Dropbox/Dev/Nunc/njr/nunc.conf");


//    mp_Diary->encrypt();

    mp_CurrEntry = mp_Diary->currentEntry();
    updateWin();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::log(QString s)
{
    qDebug() << "LOG"<<s;
}

void MainWindow::err(QString s)
{
    qDebug() << "ERR"<<s;
}

void MainWindow::on_pushButton_clicked()
{
    if ( mp_CurrEntry->prev() )
    {
        mp_CurrEntry = mp_CurrEntry->prev();
        updateWin();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if ( mp_CurrEntry->next() )
    {
        mp_CurrEntry = mp_CurrEntry->next();
        updateWin();
    }
}

void MainWindow::updateWin()
{
    mb_DropEvents = true;
    ui->pushButton->setEnabled( mp_CurrEntry->prev()!=NULL );
    ui->pushButton_2->setEnabled( mp_CurrEntry->next()!=NULL );
    ui->plainTextEdit->setPlainText( mp_CurrEntry->text() );
    ui->label->setText( QDateTime::fromTime_t( mp_CurrEntry->id() ).toString() );
    mb_DropEvents = false;
}



void MainWindow::on_plainTextEdit_textChanged()
{
    if (mb_DropEvents)
        return;

    mp_Diary->setCurrentEntryText( ui->plainTextEdit->toPlainText() );
}
