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
    explicit Diary(QObject *parent = 0);

    bool load(const QString& diaryPath);
    Entry* createEntry();

    const QString& password() const;
    void setPassword(const QString &value);

    bool isEncripted() const;
    void setEncripted(bool value);

    QString fullPath() const;
    void setFullPath(const QString &value);

public slots:
    bool setCurrentEntryText(const QString& text);

private:
    void scanForEntries( const QString& fullPath );

private:
    QMap<quint32, Entry*> mm_Entries;
    QString ms_DiaryPath;
    QString ms_Password;
    QString ms_FullPath;
    bool mb_Encripted;

signals:
    void error(QString err);
    void log(QString log);

public slots:
};

#endif // DIARY_H
