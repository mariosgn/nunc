#ifndef DIARY_H
#define DIARY_H

#include <QJsonObject>
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

    bool create();


    int entriesSize() const ;
    Entry* entriAtIndex(int i) const;

    Q_INVOKABLE bool setPassword( const QByteArray &password );
    Q_INVOKABLE bool checkPassword( const QByteArray &password );
    Q_INVOKABLE bool load( const QByteArray &password );
    Q_INVOKABLE bool open();
    Q_INVOKABLE QStringList getErrors();

signals:
    void error(QString err);

signals:
    void loaded();

private:
    void updateEntriesIdx();
    bool writeConf();
    void errorMsg(QString err);

private:
    QJsonObject ms_ConfObject;
    QString ms_DiaryPath;
    QString ms_ConfFileName;
    QStringList ml_Errors;


    //ODLS-------




public:

    Entry* createCurrentEntry();

    const QByteArray &password() const;
    QString fullPath() const;

public slots:
    void setCurrentEntryText(const QString& text);


private:
    bool scanForEntries( const QString& fullPath );

private:
    QMap<quint32, Entry*> mm_Entries;
    QVector<quint32> ml_EntriesOrdered;


    QByteArray ms_Password;


    void log(QString log);

public slots:
};

#endif // DIARY_H
