#ifndef DIARY_H
#define DIARY_H

#include <QObject>
#include <QVariant>
#include <QVector>

#define DEFAULT_CONF_FILE "nunc.conf"

class Entry;

class Diary : public QObject
{
    Q_OBJECT
public:
    explicit Diary(const QString& path, QObject *parent = 0);

    int entriesSize() const ;
    Entry* entriAtIndex(int i) const;

    Q_INVOKABLE void setPassword(const QByteArray &password );
    Q_INVOKABLE bool load();

signals:
    void loaded();


private:
    void updateEntriesIdx();
    //ODLS-------

public:

    Entry* createEntry();


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
    QVector<quint32> ml_EntriesOrdered;
    QString ms_InitialPath;
    QString ms_DiaryPath;
    QByteArray ms_Password;
    bool mb_Encripted;

signals:
    void error(QString err);
    void log(QString log);

public slots:
};

#endif // DIARY_H
