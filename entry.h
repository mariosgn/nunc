#ifndef ENTRY_H
#define ENTRY_H

#include <QObject>
#include <QTimer>
#include <QDateTime>

class Diary;
class Entry;

class Entry : public QObject
{
    Q_OBJECT
public:
    explicit Entry(Diary *parent, const QString& filePath = QString() );
    ~Entry();

    const QString& text() const;
    void setText(const QString &value);

    QDateTime date() const;
    quint32 id() const;

    bool explicitSave();
    bool decoded();

    static bool verifyEncoding(const QByteArray& data, const QByteArray &key );
    static QByteArray generateEncoding(const QByteArray& data, const QByteArray &key );

private slots:
    bool save();
    void load();

    static QByteArray encript( const QByteArray& data, const QByteArray &key );
    static QByteArray decript( const QByteArray& data, const QByteArray &key );

private:
    void init();

private:
    Diary* mp_Diary;
    QString ms_filePath;
    QString ms_Text;
    bool mb_Modified;
    QTimer ms_Timer;
    quint32 mi_Id;
    bool mb_SuccessDecode;
};

#endif // ENTRY_H
