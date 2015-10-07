#include "entry.h"
#include "diary.h"

#include <QDateTime>
#include <QFile>

Entry::Entry(Diary *parent, const QString &filePath) :
    QObject(parent),
    mp_Diary(parent),
    ms_filePath(filePath),
    mb_Modified(true)
{
    if ( ms_filePath.size()==0 )
    {
        ms_filePath = mp_Diary->property( Diary::FullPath ).toString();
        ms_filePath += "/";
        ms_filePath += QString::number( QDate::currentDate().year() );
        ms_filePath += "/";
        mi_Id = QDateTime::currentDateTime().toTime_t();
        ms_filePath += QString::number( mi_Id );
    }
    else
    {
        mi_Id = ms_filePath.split("/").last().toInt();
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

const QString &Entry::content() const
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

quint32 Entry::id() const
{
    return mi_Id;
}

