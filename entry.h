#ifndef ENTRY_H
#define ENTRY_H

#include <QObject>
#include <QTimer>

class Diary;

class Entry : public QObject
{
    Q_OBJECT
public:
    explicit Entry(Diary *parent, const QString& filePath = QString() );
    ~Entry();

    const QString& content() const;
    void setText(const QString &value);

    quint32 id() const;

private slots:
    bool save();

private:
    Diary* mp_Diary;
    QString ms_filePath;
    QString ms_Text;
    bool mb_Modified;
    QTimer ms_Timer;
    quint32 mi_Id;
};

#endif // ENTRY_H