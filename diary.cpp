#include "diary.h"
#include "entry.h"

#include <QFileInfo>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDateTime>

#define CONF_NUNC "nunc"
#define CONF_ENCRYPTED "encrypted"

Diary::Diary(QObject *parent) :
    QObject(parent),
    mb_Encripted(false)
{

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
            setEncripted(true);
            break;
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
    Entry *e = new Entry(this);
    mm_Entries[e->id()] = e;
    return e;
}

const QString &Diary::password() const
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

void Diary::scanForEntries(const QString &fullPath)
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

            Entry *e = new Entry(this, yfileInfo.absoluteFilePath());
            mm_Entries[e->id()] = e;

            log("Found "+ yfileInfo.fileName() );
        }
    }

}

QString Diary::fullPath() const
{
    return ms_FullPath;
}

void Diary::setFullPath(const QString &value)
{
    ms_FullPath = value;
}

bool Diary::isEncripted() const
{
    return mb_Encripted;
}

void Diary::setEncripted(bool value)
{
    mb_Encripted = value;
}

void Diary::setPassword(const QString &value)
{
    ms_Password = value;
}

