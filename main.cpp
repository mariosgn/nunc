#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "diarymodel.h"
#include "diary.h"

#include <QtGui>
#include <QtQml>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Diary* d = new Diary;
    d->setPassword("ASD");
    d->load("/home/mario/Dropbox/Dev/Nunc/njr/nunc.conf");

    DiaryModel* model = new DiaryModel(d);

    QQmlApplicationEngine engine;

    QQmlContext *ctxt = engine.rootContext();
    ctxt->setContextProperty("modelData", QVariant::fromValue(model));

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));



    return app.exec();
}
