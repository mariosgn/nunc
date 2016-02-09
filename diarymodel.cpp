#include "diarymodel.h"
#include "diary.h"
#include "entry.h"

#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QDebug>

DiaryModel::DiaryModel(Diary *diary) :
    QAbstractListModel(diary),
    mp_Diary(diary)
{
    ml_RoleNames[DateRole] = "date";
    ml_RoleNames[HourRole] = "hour";
    ml_RoleNames[TimeRole] = "time";
    ml_RoleNames[TextRole] = "entryText";
}

int DiaryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mp_Diary->entriesSize();
}

QVariant DiaryModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= mp_Diary->entriesSize()) {
        return QVariant();
    }

    const Entry* it = mp_Diary->entriAtIndex( row );

    switch(role) {
    case DateRole:
        return it->date().date();
    case HourRole:
        return it->date().time();
    case TextRole:
        return it->text();
    case TimeRole:
        return it->date();
    }
    return QVariant();
}

QHash<int, QByteArray> DiaryModel::roleNames() const
{
    return ml_RoleNames;
}
