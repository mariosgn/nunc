#ifndef ENTRY_H
#define ENTRY_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QImage>

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

    bool hasImage() const;
    const QImage& image() const;
    void setImage( const QImage& image);

signals:
    void error(QString);

private slots:
    bool save();
    void load();
    void errorMsg(const QString& err);

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
    bool mb_HasImage;
    bool mb_HasLoadedImage;
    QImage ms_Image;
};

#endif // ENTRY_H
