#ifndef ENTRY_H
#define ENTRY_H

#include <QObject>
#include <QTimer>

class Diary;
class Entry;

class Entry : public QObject
{
    Q_OBJECT
public:
    explicit Entry(Diary *parent, const QString& filePath = QString() );
    explicit Entry(Entry *parent, const QString& filePath = QString() );
    ~Entry();

    const QString& text() const;
    void setText(const QString &value);

    quint32 id() const;

    Entry *next() const;
    Entry *prev() const;

    bool explicitSave();

private slots:
    bool save();
    bool load();

    QByteArray encript( const QByteArray& data, const QByteArray &key );
    QByteArray decript( const QByteArray& data, const QByteArray &key );

private:
    void init();

private:
    Diary* mp_Diary;
    Entry* mp_Next;
    Entry* mp_Prev;
    QString ms_filePath;
    QString ms_Text;
    bool mb_Modified;
    QTimer ms_Timer;
    quint32 mi_Id;
};

#endif // ENTRY_H
