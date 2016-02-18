#ifndef DIARYMODEL_H
#define DIARYMODEL_H

#include <QObject>
#include <QDateTime>
#include <QtGui>

class Diary;

class DiaryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DiaryModel( Diary *diary );

    enum RoleNames {
            DateRole = Qt::UserRole,
            HourRole = Qt::UserRole+2,
            TimeRole = Qt::UserRole+3,
            TextRole = Qt::UserRole+4
        };

private slots:
    void loaded();

public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE int entriesAtDate(const QDate& date);


private:
    Diary* mp_Diary;
    QHash<int, QByteArray> ml_RoleNames;

};

#endif // DIARYMODEL_H
