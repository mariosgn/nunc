#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class Diary;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QByteArray encript( const QByteArray& data, const QByteArray &key );
    QByteArray decript( const QByteArray& data, const QByteArray &key );

public slots:
    void log(QString s);
    void err(QString s);
private slots:
    void on_plainTextEdit_textChanged();

private:
    Ui::MainWindow *ui;
    Diary* mp_Diary;
};

#endif // MAINWINDOW_H
