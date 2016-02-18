#include "entry.h"
#include "diary.h"

#include <QDateTime>
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QBuffer>

#include <openssl/aes.h>
#include <openssl/evp.h>

#define IMAGE_HEADER "\n______NUNC_IMAGE_HEADER______\n"

Entry::Entry(Diary *parent, const QString &filePath) :
    mp_Diary(parent),
    ms_filePath(filePath),
    mb_Modified(true),
    mb_HasImage(false),
    mb_HasLoadedImage(false)
{
    load();
    ms_Timer.setSingleShot(true);
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
    if ( ms_Text.simplified() == value.simplified() )
    {
        return;
    }
    ms_Text = value;
    mb_Modified = true;
    ms_Timer.start(2000);
}

QDateTime Entry::date() const
{
    return QDateTime::fromTime_t( mi_Id );
}

bool Entry::explicitSave()
{
    mb_Modified = true;
    return save();
}

bool Entry::decoded()
{
    return mb_SuccessDecode;
}

bool Entry::verifyEncoding(const QByteArray &data, const QByteArray &key)
{
    return decript( data, key).size() > 0;
}

QByteArray Entry::generateEncoding(const QByteArray &data, const QByteArray &key)
{
    return encript( data, key );
}

bool Entry::hasImage() const
{
    return mb_HasImage;
}

const QImage &Entry::image()
{
    if ( !mb_HasLoadedImage )
    {
        QBuffer buffer(&ms_ImageBuffLoaded);
        buffer.open(QIODevice::ReadOnly);
        ms_Image.load(&buffer, "JPG");
    }
    return ms_Image;
}

void Entry::setImage(const QImage &image)
{
    mb_HasImage = !image.isNull();
    mb_HasLoadedImage = true;
    ms_Image = image;
    mb_Modified = true;
    ms_Timer.start(2000);
}

bool Entry::save()
{
    ms_Timer.stop();

    if ( !mb_Modified )
        return true;

    if ( !mb_HasImage && ms_Text.size() == 0 )
        return true;

    QFileInfo fi(ms_filePath);
    QString fp = fi.absoluteDir().absolutePath();
    QDir fiDir = fi.absoluteDir();
    if ( !fiDir.exists() )
    {
        if ( !fiDir.mkpath( fp ) )
        {
            errorMsg( QString( tr("Cannot create diary page dir [%1], please check permissions ") ).arg( fp ) );
            return false;
        }
    }


    QFile f(ms_filePath);
    if (!f.open(QIODevice::WriteOnly))
    {
        errorMsg( QString( tr("Cannot open diary page [%1], please check permissions ") ).arg( ms_filePath ) );
        return false;
    }

    QByteArray t = ms_Text.toUtf8();

    if ( mb_HasImage )
    {
        t.append( IMAGE_HEADER );
        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        ms_Image.save(&buffer, "JPG");
        t.append( ba.toBase64() );
    }

    qDebug() << t;

    t = encript( t, mp_Diary->password());



    if ( f.write( t ) < 0 )
    {
        errorMsg( QString( tr("Cannot write diary page [%1], please check permissions ") ).arg( ms_filePath ) );
        return false;
    }

    f.close();
    mb_Modified = false;
    return true;
}

void Entry::load()
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

        e = decript( e, mp_Diary->password());

        mb_SuccessDecode = e.size()>0 ;

        int imageIdx = e.indexOf(IMAGE_HEADER);
        if ( imageIdx!=-1 )
        {
            QString header(IMAGE_HEADER);
            ms_ImageBuffLoaded =  QByteArray::fromBase64( e.mid( imageIdx + header.size() ) );
            e = e.mid(0, imageIdx);
            mb_HasImage = true;
            mb_HasLoadedImage = false;
        }

        ms_Text = e;
        mb_Modified = false;
    }
}

void Entry::errorMsg(const QString &err)
{
    qCritical() << err;
    emit error(err);
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
//    qDebug( )<< "stllt" << stllt.toHex();

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
//    qDebug() << "iv" << ivBa.toHex();

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

//    qDebug() << "salt" << header.toHex();

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
//    qDebug() << "iv" << ivBa.toHex();

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

