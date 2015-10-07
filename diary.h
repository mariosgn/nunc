#ifndef DIARY_H
#define DIARY_H

#include <QObject>
#include <QVariant>

#define DEFAULT_CONF_FILE "nunc.conf"

class Entry;

class Diary : public QObject
{
    Q_OBJECT
public:

    enum DiaryProperty
    {
      FullPath
    } ;

    explicit Diary(QObject *parent = 0);

    bool load(const QString& diaryPath);
    Entry* createEntry();

    QVariant property( DiaryProperty p ) const;

public slots:
    bool setCurrentEntryText(const QString& text);

private:
    void scanForEntries( const QString& fullPath );

private:
    QMap<quint32, Entry*> mm_Entries;
    QString ms_DiaryPath;

signals:
    void error(QString err);
    void log(QString log);

public slots:
};

#endif // DIARY_H
