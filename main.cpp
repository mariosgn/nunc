#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtGui>
#include <QtQml>
#include <QCommandLineParser>

#define VERSION "0.1"

#include "diarymodel.h"
#include "diary.h"

#include "entry.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QCoreApplication::setApplicationName("Nunc");
    QCoreApplication::setApplicationVersion(VERSION);

    QCommandLineParser parser;
    parser.setApplicationDescription("Nunc options");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("diary_path", QCoreApplication::translate("main", "Diary directory of configuration file"));


    QCommandLineOption createOption(QStringList() << "c" << "create",
                                   QCoreApplication::translate("main", "Create a diary in the given directory."));
    parser.addOption(createOption);

    parser.process(app);

    bool createDiary = parser.isSet(createOption);
    const QStringList args = parser.positionalArguments();
    if ( args.size() == 0 )
    {
        parser.showHelp(-1);
    }
    const QString& diaryPath = args.at(0);


    Diary d( diaryPath );

    if ( createDiary )
    {
        bool res = d.create();
        if (!res)
        {
            qCritical() << d.getErrors();
            return -1;
        }

        QTextStream s(stdin);
        qInfo() << "Please enter the new password:";
        QString value = s.readLine();
        d.setPassword( value.toLatin1() );
    }

    int res;
    {
        DiaryModel model(&d);

        QQmlApplicationEngine engine;

        QQmlContext *ctxt = engine.rootContext();
        ctxt->setContextProperty("modelData", QVariant::fromValue(&model));
        ctxt->setContextProperty("diary", QVariant::fromValue(&d));

        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

        res = app.exec();
    }
    return res;
}
