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

    void setPassword(const QByteArray &password );
    const QByteArray &password() const;
    bool isEncripted() const;
    QString fullPath() const;

    void setEncrypted(bool v);
    void encrypt();

public slots:
    bool setCurrentEntryText(const QString& text);
    Entry* currentEntry();

private:
    void scanForEntries( const QString& fullPath );

private:
    QMap<quint32, Entry*> mm_Entries;
    QString ms_DiaryPath;
    QByteArray ms_Password;
    bool mb_Encripted;

signals:
    void error(QString err);
    void log(QString log);

public slots:
};

#endif // DIARY_H
