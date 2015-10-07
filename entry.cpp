#include "entry.h"
#include "diary.h"

#include <QDateTime>
#include <QFile>
#include <QDebug>

#include <openssl/aes.h>
#include <openssl/evp.h>

QByteArray ____password = "ASD";


Entry::Entry(Diary *parent, const QString &filePath) :
    QObject(parent),
    mp_Diary(parent),
    ms_filePath(filePath),
    mb_Modified(true)
{
    if ( ms_filePath.size()==0 )
    {
        ms_filePath = mp_Diary->fullPath();
        ms_filePath += "/";
        ms_filePath += QString::number( QDate::currentDate().year() );
        ms_filePath += "/";
        mi_Id = QDateTime::currentDateTime().toTime_t();
        ms_filePath += QString::number( mi_Id );
    }
    else
    {
        mi_Id = ms_filePath.split("/").last().toInt();

        QFile f(ms_filePath);
        f.open(QIODevice::ReadOnly);
        QByteArray e = f.readAll();
        if ( mp_Diary->isEncripted() )
        {
            e = decript( e, ____password);
        }
        ms_Text = e;
    }
    connect (&ms_Timer, SIGNAL(timeout()), this, SLOT(save()));
}

Entry::~Entry()
{
    if (mb_Modified)
    {
        save();
    }
}

const QString &Entry::text() const
{
    return ms_Text;
}


void Entry::setText(const QString &value)
{
    ms_Text = value;
    mb_Modified = true;
    ms_Timer.start(2000);
}

bool Entry::save()
{
    if ( !mb_Modified || ms_Text.size() == 0)
        return true;

    QFile f(ms_filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    if ( f.write( ms_Text.toUtf8() ) < 0 )
        return false;

    f.close();
    return true;
}

QByteArray Entry::encript(const QByteArray &data, const QByteArray &key)
{
    //    QCryptographicHash _hashKey(QCryptographicHash::Sha256);
    //    _hashKey.addData(key);
    //    QByteArray hashKey = _hashKey.result();

    QByteArray saltHeader = "Salted__";

    qsrand((uint)QDateTime::currentDateTime().toTime_t());

    QByteArray res;

    EVP_CIPHER_CTX en;
    unsigned char salt[8];
    unsigned char *key_data = (unsigned char *)key.constData();
    int key_data_len = key.size();

    //buid a salt
    for (int i = 0; i < 8; ++i) {
        salt[i] = qrand()%255;
    }


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

QByteArray Entry::decript(const QByteArray &data, const QByteArray &key)
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

quint32 Entry::id() const
{
    return mi_Id;
}

