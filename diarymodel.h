#ifndef DIARYMODEL_H
#define DIARYMODEL_H

#include <QObject>
#include <QDateTime>
#include <QtGui>
#include <QQuickImageProvider>

class Diary;


class DiaryImageProvider : public QQuickImageProvider
{
public:
    DiaryImageProvider(Diary* d) :
        QQuickImageProvider(QQuickImageProvider::Image),
        mp_Diary(d)
    {

    }
private:
    Diary* mp_Diary;


public:
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
};



class DiaryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DiaryModel( Diary *diary );

    enum RoleNames {
            DateRole = Qt::UserRole,
            HourRole = Qt::UserRole+2,
            TimeRole = Qt::UserRole+3,
            TextRole = Qt::UserRole+4,
            ImageRole = Qt::UserRole+5,
            HasImageRole = Qt::UserRole+6
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
