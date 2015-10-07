#include "diary.h"
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
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog	);
    mp_Diary = new Diary(this);

    connect(mp_Diary, SIGNAL(error(QString)), this, SLOT(err(QString)));
    connect(mp_Diary, SIGNAL(log(QString)), this, SLOT(log(QString)));

    mp_Diary->load("/home/mario/Dropbox/Dev/Nunc/njr");



    QFile f("/tmp/cscs");
    f.open(QIODevice::ReadOnly);
    QByteArray res = f.readAll();
    qDebug() << decript(res, "MYPASSWORD");


    {
    QByteArray resen = encript("tuamadrezozzzola", "MYPASSWORD");
    QFile fe("/tmp/ccc");
    fe.open(QIODevice::WriteOnly|QIODevice::Truncate);
    fe.write(resen);
    fe.close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

QByteArray MainWindow::encript(const QByteArray &data, const QByteArray &key)
{
//    QCryptographicHash hashKey(QCryptographicHash::Sha256);
//    hashKey.addData(key);
    QByteArray hashKey = key;

    QByteArray saltHeader = "Salted__";

    qsrand((uint)QDateTime::currentDateTime().toTime_t());

    QByteArray res;

    EVP_CIPHER_CTX en;
    unsigned char salt[8];
    unsigned char *key_data = (unsigned char *)hashKey.constData();
    int key_data_len = hashKey.size();

    //buid a salt
    for (int i = 0; i < 8; ++i) {
        salt[i] = qrand()%255;
    }

    int ii = 0;
    salt[ii++] = 0xc7;
    salt[ii++] = 0x1a;
    salt[ii++] = 0x3c;
    salt[ii++] = 0xe5;
    salt[ii++] = 0x5b;
    salt[ii++] = 0x91;
    salt[ii++] = 0x01;
    salt[ii++] = 0x55;
    QByteArray stllt((const char*)&salt, 8);
    qDebug( )<< "stllt" << stllt.toHex();

    unsigned char _key[32], iv[32];
    memset(&iv, '\0', 32);
    int i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_md5(),
                           (unsigned char*)&salt,
                           key_data, key_data_len, 1,
                           _key, iv);
    if (i != 32) {
      qDebug() << "Key size is  bits - should be 256 bits\n" <<  i;
      return res;
    }

    QByteArray ivBa((const char*)iv, 16);
    qDebug() << "iv" << ivBa.toHex();

    EVP_CIPHER_CTX_init(&en);
    EVP_EncryptInit_ex(&en, EVP_aes_256_cbc(), NULL, _key, iv);

    int c_len = data.size() + AES_BLOCK_SIZE;
    int f_len = 0;
    unsigned char *ciphertext = (unsigned char *)malloc(c_len);

    EVP_EncryptUpdate(&en, ciphertext, &c_len, (unsigned char *)data.constData(), data.size()+1);
    EVP_EncryptFinal_ex(&en, ciphertext+c_len, &f_len);
    res = QByteArray((const char*)ciphertext, c_len + f_len);
    free(ciphertext);
    EVP_CIPHER_CTX_cleanup(&en);
    return saltHeader+QByteArray((const char*)&salt, 8)+res;
}

QByteArray MainWindow::decript(const QByteArray &data, const QByteArray &key)
{
    QByteArray res;

    QByteArray saltHeader = "Salted__";
    int headerSize = 16;
    QByteArray header = data.mid(saltHeader.size(), headerSize-saltHeader.size());

    qDebug() << "salt" << header.toHex();

    EVP_CIPHER_CTX de;
    unsigned char *salt = (unsigned char *)header.constData();
    unsigned char *key_data = (unsigned char *)key.constData();
    int key_data_len = key.size();


    unsigned char _key[32], iv[32];
    int i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_md5(),
                          salt,
                           key_data, key_data_len, 1,  //TODO: what is this magic number
                           _key, iv);
    if (i != 32) {
      qDebug() << "Key size is  bits - should be 256 bits\n" <<  i;
      return res;
    }

    QByteArray ivBa((const char*)iv, 16);
    qDebug() << "iv" << ivBa.toHex();

    EVP_CIPHER_CTX_init(&de);
    EVP_DecryptInit_ex(&de, EVP_aes_256_cbc(), NULL, _key, iv);

    QByteArray dataDecr = data.mid(headerSize);

    int p_len = dataDecr.size();
    int f_len = 0;
    unsigned char *plaintext = (unsigned char *)malloc(p_len);

    EVP_DecryptUpdate(&de, plaintext, &p_len, (unsigned char*)dataDecr.constData(), dataDecr.size());
    EVP_DecryptFinal_ex(&de, plaintext+p_len, &f_len);
    res = QByteArray((const char*)plaintext, p_len + f_len);
    free(plaintext);
    EVP_CIPHER_CTX_cleanup(&de);
    return res;
}

void MainWindow::log(QString s)
{
    qDebug() << "LOG"<<s;
}

void MainWindow::err(QString s)
{
    qDebug() << "ERR"<<s;
}

void MainWindow::on_plainTextEdit_textChanged()
{
    mp_Diary->setCurrentEntryText( ui->plainTextEdit->toPlainText() );
}
