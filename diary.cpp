#include "diary.h"
#include "entry.h"

#include <QFileInfo>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDateTime>

#define CONF_NUNC "nunc_conf"
#define CONF_ENCRYPTED "encrypted"

Diary::Diary(QObject *parent) :
    QObject(parent),
    mb_Encripted(false)
{

}

int Diary::entriesSize() const
{
    return mm_Entries.size();
}

Entry *Diary::entriAtIndex(int i )  const
{
    return mm_Entries[ml_EntriesOrdered.at( i )];
}

void Diary::updateEntriesIdx()
{
    ml_EntriesOrdered = mm_Entries.keys().toVector();
    qSort(ml_EntriesOrdered);
}

bool Diary::load(const QString &diaryPath)
{
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
            error(tr("Diary path not valid: configuration file missing"));
            return false;
        }
        configuration.setFileName( pc.absoluteFilePath() );

    }
    else
    {
        error(tr("Diary path not valid"));
        return false;
    }

    if ( !configuration.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        error(QString( tr("Diary path not valid: cannot read configuration file [%1]").arg(configuration.fileName() ) ));
        return false;
    }

    QString conf = configuration.readAll();
    configuration.close();

    QJsonParseError e;
    QJsonDocument d = QJsonDocument::fromJson(conf.toUtf8(), &e);
    if (e.error != QJsonParseError::NoError	)
    {
        error(tr("Diary conf json not valid: ")+ e.errorString() );
        return false;
    }

    QJsonObject settJson = d.object();
    bool errorConf = true;
    QString confNotFound;
    do
    {
        if ( !settJson.value(QString(CONF_NUNC)).toBool() )
        {
            confNotFound = CONF_NUNC;
            break;
        }

        if ( settJson.value(QString(CONF_ENCRYPTED)).toBool() )
        {
            if ( password().size()==0 )
            {
                error( tr("Missing diary password") );
                return false;
            }
            setEncrypted(true);
        }

        errorConf = false;
    } while(0);


    if ( errorConf )
    {
        error( tr("Diary conf not valid. Value missing or wrong: ")+confNotFound );
        return false;
    }

    QFileInfo cfi( configuration.fileName() );
    ms_DiaryPath = cfi.absoluteDir().absolutePath();

    scanForEntries(ms_DiaryPath);
    createEntry();
    log("diary ok "+configuration.fileName() );
    return true;
}

Entry *Diary::createEntry()
{
    Entry *e;
    if ( mm_Entries.size()==0 )
    {
        e = new Entry(this);
    }
    else
    {
        e = new Entry(mm_Entries.last());
    }
    mm_Entries[e->id()] = e;
    updateEntriesIdx();
    return e;
}

void Diary::setPassword(const QByteArray &password)
{
    ms_Password = password;
}

const QByteArray &Diary::password() const
{
    return ms_Password;
}

bool Diary::setCurrentEntryText(const QString &text)
{
    if ( mm_Entries.size()==0 )
    {
        createEntry();
    }

    mm_Entries.last()->setText(text);
}

Entry *Diary::currentEntry()
{
    if ( mm_Entries.size()==0 )
    {
        createEntry();
    }

    return mm_Entries.last();
}

void Diary::scanForEntries(const QString &fullPath)
{
    QDir d(fullPath);
    d.setFilter(QDir::AllDirs|QDir::Readable);

    QFileInfoList list = d.entryInfoList();
    Entry* lastEntry = NULL;

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

            Entry *e;
            if ( lastEntry )
            {
                e = new Entry( lastEntry , yfileInfo.absoluteFilePath());
            }
            else
            {
                e = new Entry( this, yfileInfo.absoluteFilePath());
            }
            lastEntry = e;
            mm_Entries[e->id()] = e;

            log("Found "+ yfileInfo.fileName() );
        }
    }
    updateEntriesIdx();
}

QString Diary::fullPath() const
{
    return ms_DiaryPath;
}

void Diary::setEncrypted( bool v )
{
    mb_Encripted = v;
}

void Diary::encrypt( )
{
    //TODO: changhe the conf file
    if ( ms_Password.size()<=0 )
    {
        log("Empty password" );
        return;
    }

    mb_Encripted = true;

    QMapIterator<quint32, Entry*> i(mm_Entries);
    while (i.hasNext()) {
        i.next();
        i.value()->explicitSave();
    }
}


bool Diary::isEncripted() const
{
    return mb_Encripted;
}



