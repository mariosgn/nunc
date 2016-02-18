#include "diary.h"
#include "entry.h"

#include <QFileInfo>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDir>
#include <QDateTime>

#define CONF_NUNC "nunc_conf"
#define CONF_ENCRYPTED "encrypted"
#define CONF_CHECK_ENCRYPTED "checkenc"

Diary::Diary(const QString &path, QObject *parent) :
    QObject(parent),
    ms_DiaryPath(path)
{

}

Diary::~Diary()
{
    mm_Entries.clear();
}


int Diary::entriesSize() const
{
    return mm_Entries.size();
}

Entry *Diary::entryAtIndex(int i )  const
{
    return mm_Entries[ ml_EntriesOrdered.at( i ) ].data();
}

void Diary::updateEntriesIdx()
{
    mm_EntriesAtDate.clear();
    ml_EntriesOrdered = mm_Entries.keys().toVector();
    for (int i = 0; i < ml_EntriesOrdered.size(); ++i)
    {
        QDate d = QDateTime::fromTime_t( ml_EntriesOrdered.at(i) ).date();

        if ( !mm_EntriesAtDate.contains(d) )
        {
            EntriesInfo ei;
            ei.modelIdx = i;
            ei.amount = 0;
            mm_EntriesAtDate[d] = ei;
        }

        mm_EntriesAtDate[d].amount++;
    }
    std::sort (ml_EntriesOrdered.begin(), ml_EntriesOrdered.end());
}



bool Diary::create()
{
    QString diaryPath = ms_DiaryPath;

    QFileInfo p(diaryPath);

    if ( p.isFile() )
    {
        errorMsg( QString( tr("%1 already exists: please insert a not existing path") ).arg(diaryPath) );
        return false;
    }

    if ( !p.isDir() )
    {
        errorMsg( QString( tr( "%1 does not exists: please insert an existing and writable path").arg(diaryPath) ) );
        return false;
    }

    ms_ConfFileName = DEFAULT_CONF_FILE;

    ms_ConfObject = QJsonObject();
    ms_ConfObject.insert( CONF_NUNC, true );

    if ( ! writeConf()  )
        return false;

    createCurrentEntry();
    updateEntriesIdx();

    return true;
}


bool Diary::writeConf()
{
    QString fullPath( ms_DiaryPath + "/" + ms_ConfFileName );
    QFile configuration( fullPath );
    if ( !configuration.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        errorMsg(QString( tr("Cannote create the configuration file [%1]: please check the path or permission").arg( fullPath ) ));
        return false;
    }

    QJsonDocument doc(ms_ConfObject);
    QString confStr = doc.toJson();
    quint64 res = configuration.write( confStr.toUtf8() );
    if ( res != (quint64)confStr.size() )
    {
        errorMsg(QString( tr("Cannote write the configuration file [%1]: please check the path or permission").arg( fullPath ) ));
        return false;
    }

    return true;
}

void Diary::errorMsg(QString err)
{
    ml_Errors.append( err );
    error(err);
}


bool Diary::load( const QByteArray &password )
{
    ms_Password = password;

    scanForEntries(ms_DiaryPath);
    createCurrentEntry();
    updateEntriesIdx();

    emit loaded();
    return true;
}

bool Diary::open()
{
    QString diaryPath = ms_DiaryPath;

    QFileInfo p(diaryPath);
    QFile configuration;

    if ( p.isFile() )
    {
        configuration.setFileName(diaryPath);
    }
    else if ( p.isDir() )
    {
        QFileInfo pc( diaryPath + "/" + DEFAULT_CONF_FILE );
        if ( !pc.isFile() )
        {
            errorMsg(tr("Diary path not valid: configuration file missing"));
            return false;
        }
        configuration.setFileName( pc.absoluteFilePath() );
    }
    else
    {
        errorMsg(tr("Diary path not valid"));
        return false;
    }

    if ( !configuration.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        errorMsg(QString( tr("Diary path not valid: cannot read configuration file [%1]").arg(configuration.fileName() ) ));
        return false;
    }

    QString conf = configuration.readAll();
    configuration.close();

    QJsonParseError e;
    QJsonDocument d = QJsonDocument::fromJson(conf.toUtf8(), &e);
    if (e.error != QJsonParseError::NoError	)
    {
        errorMsg(tr("Diary conf json not valid: ")+ e.errorString() );
        return false;
    }

    ms_ConfObject = d.object();
    bool errorConf = true;
    QString confNotFound;

    do
    {
        if ( !ms_ConfObject.value(QString(CONF_NUNC)).toBool() )
        {
            confNotFound = CONF_NUNC;
            break;
        }

        if ( ms_ConfObject.value( CONF_CHECK_ENCRYPTED ).toString().size()<=0 )
        {
            errorMsg( tr("Missing diary key password") );
            return false;
        }

        errorConf = false;
    } while(0);


    if ( errorConf )
    {
        errorMsg( tr("Diary conf not valid. Value missing or wrong: ")+confNotFound );
        return false;
    }

    QFileInfo cfi( configuration.fileName() );
    ms_DiaryPath = cfi.absoluteDir().absolutePath();
    return true;
}

Entry *Diary::createCurrentEntry()
{
    QSharedPointer<Entry> e =  QSharedPointer<Entry>(new Entry(this));
    mm_Entries[e->id()] = e;
    return e.data();
}


QStringList Diary::getErrors()
{
    QStringList res = ml_Errors;
    ml_Errors.clear();
    return res;
}

bool Diary::setPassword(const QByteArray &password)
{
    if ( password.size()<=0 )
    {
        errorMsg( "Empty password not permitted" );
        return false;
    }

    ms_Password = password;

    QByteArray data = QString::number( QDateTime::currentDateTime().currentMSecsSinceEpoch() ).toLatin1();

    QByteArray enc = Entry::generateEncoding( data, ms_Password );

    ms_ConfObject.insert( CONF_CHECK_ENCRYPTED, enc.toHex().data() );

    if ( ! writeConf()  )
        return false;

    QMapIterator<quint32, QSharedPointer<Entry> > i(mm_Entries);
    while (i.hasNext()) {
        i.next();
        i.value().data()->explicitSave();
    }

    return true;
}

bool Diary::checkPassword(const QByteArray &password)
{
    QByteArray data = ms_ConfObject.value( CONF_CHECK_ENCRYPTED).toString().toLatin1();
    data = QByteArray::fromHex( data );
    bool res = Entry::verifyEncoding( data, password );
    if ( !res )
    {
        errorMsg( "Wrong password" );
        return false;
    }
    return res;
}

void Diary::setCurrentText(const QString &text)
{
    mm_Entries.last()->setText(text);
}

QDate Diary::getLastDate() const
{
    return mm_EntriesAtDate.keys().last();
}

int Diary::getIndexForDate(const QDate &date) const
{
    if ( mm_EntriesAtDate.contains(date) )
        return mm_EntriesAtDate[date].modelIdx;
    return -1;
}

int Diary::entriesAtDate(const QDate &date) const
{
//    if ( mm_EntriesAtDate.contains(date) )
//        return mm_EntriesAtDate[date].amount;
//    return 0;
    int res = 0;
    if ( mm_EntriesAtDate.contains(date) )
        res = mm_EntriesAtDate[date].amount;

//    qDebug() << date << res;

    return res;
}















const QByteArray &Diary::password() const
{
    return ms_Password;
}





bool Diary::scanForEntries(const QString &fullPath)
{
    QDir d(fullPath);
    d.setFilter(QDir::AllDirs|QDir::Readable);

    QFileInfoList list = d.entryInfoList();

    bool ok;
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);

        QString yearStr = fileInfo.baseName();
        yearStr.toInt(&ok);
        if (!ok)
            continue;

        QDir dY(fileInfo.absoluteFilePath());
        dY.setFilter(QDir::Files|QDir::Readable);
        QFileInfoList ylist = dY.entryInfoList();
        for (int y = 0; y < ylist.size(); ++y)
        {
            QFileInfo yfileInfo = ylist.at(y);

            QString fileEntryStr = yfileInfo.baseName();
            ulong fileEntry = fileEntryStr.toLong(&ok);
            if (!ok)
                continue;

            QDateTime dt = QDateTime::fromTime_t(fileEntry);
            if (!dt.isValid())
                continue;

            QSharedPointer<Entry> e =  QSharedPointer<Entry>(new Entry(this, yfileInfo.absoluteFilePath()));
            Entry *en = e.data();

            if ( !en->decoded() )
            {
                errorMsg( QString( tr("Failed to decode page: [%1], please check the password")).arg(fileEntryStr));
                return false;
            }
            mm_Entries[ en->id() ] = e;

//            log("Found "+ yfileInfo.fileName() );
        }
    }
    updateEntriesIdx();
    return true;
}

void Diary::log(QString log)
{
    qDebug() << log;
}

QString Diary::fullPath() const
{
    return ms_DiaryPath;
}


