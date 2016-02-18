#ifndef DIARY_H
#define DIARY_H

#include <QJsonObject>
#include <QObject>
#include <QDate>
#include <QVariant>
#include <QVector>
#include <QSharedPointer>
#include <QThread>

#define DEFAULT_CONF_FILE "nunc.conf"

class Entry;

class Diary : public QThread
{
    Q_OBJECT
public:
    explicit Diary(const QString& path, QObject *parent = 0);
    ~Diary();

    bool create();


    int entriesSize() const ;
    Entry* entryAtIndex(int i) const;
    Entry* entryAtDate(quint32 d) const;

    Q_INVOKABLE bool setPassword( const QByteArray &password );
    Q_INVOKABLE bool checkPassword( const QByteArray &password );
    Q_INVOKABLE bool load( const QByteArray &password );
    Q_INVOKABLE bool open();
    Q_INVOKABLE QStringList getErrors();
    Q_INVOKABLE void setCurrentText(const QString& text);
    Q_INVOKABLE void setCurrentImage(const QString& path);

    Q_INVOKABLE int entriesAtDate(const QDate& date) const;
    Q_INVOKABLE QDate getLastDate() const ;
    Q_INVOKABLE int getIndexForDate( const QDate& date) const ;

signals:
    void error(QString err);
    void loaded();

private:
    void updateEntriesIdx();
    bool writeConf();
    void errorMsg(QString err);
    void run() Q_DECL_OVERRIDE;

private slots:
    void fixThreadParent();

private:
    struct EntriesInfo
    {
        int amount;
        int modelIdx;
    };

    QJsonObject ms_ConfObject;
    QString ms_DiaryPath;
    QString ms_ConfFileName;
    QStringList ml_Errors;
    QMap<QDate, EntriesInfo> mm_EntriesAtDate;

    //ODLS-------




public:

    Entry* createCurrentEntry();

    const QByteArray &password() const;
    QString fullPath() const;



private:
    bool scanForEntries( const QString& fullPath );

private:
    QMap<quint32, QSharedPointer<Entry> > mm_Entries ;
    QVector<quint32> ml_EntriesOrdered;
    QByteArray ms_Password;


    void log(QString log);

public slots:
};

#endif // DIARY_H
