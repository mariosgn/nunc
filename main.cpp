#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtGui>
#include <QtQml>
#include <QCommandLineParser>
#include <QSettings>

#define VERSION "0.1"

#define SETT_NUNC_ORG "Nunc"
#define SETT_NUNC_APP "Nunc"
#define SETT_DEFAULT_PATH "default"

#include "diarymodel.h"
#include "diary.h"

#include "entry.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QSettings sett(SETT_NUNC_ORG, SETT_NUNC_APP);

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

    QCommandLineOption setAsDefaultOption(QStringList() << "d" << "default",
                                   QCoreApplication::translate("main", "Set the given path as the default one."));
    parser.addOption(setAsDefaultOption);

    parser.process(app);

    bool createDiary = parser.isSet(createOption);
    bool setDefault = parser.isSet(setAsDefaultOption);
    const QStringList args = parser.positionalArguments();

    int errorno = 0;
    QString diaryPath;
    do
    {
        if ( args.size() == 0 && createDiary )
        {
            //cannot create a diary without a path
            errorno = -1;
            break;
        }

        QString defaultDiary = sett.value(SETT_DEFAULT_PATH).toString();
        if ( args.size() == 0 && defaultDiary.size()==0 )
        {
            //no default and no path set
            errorno = -2;
            break;
        }

        if ( args.size() == 0 && setDefault )
        {
            //setting an empty default
            errorno = -3;
            break;
        }

        if ( args.size() > 1 )
        {
            //too many params
            errorno = -4;
            break;
        }


        //ok: everything seems to be fine
        if ( args.size() == 1 )
        {
            diaryPath = args.at(0);
            break;
        }
        else
        {
            diaryPath = defaultDiary;
        }

    } while(0);

    if ( errorno != 0 )
    {
        parser.showHelp(-1);
    }

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


    //TODO: preliminary check before setting the default
    if ( setDefault )
    {
        QFileInfo fi(diaryPath);
        if ( !fi.exists() )
        {
            qCritical() << "Probems: in setting the default path. It does not exists.";
            return -5;
        }
        qDebug() << fi.absolutePath();
        sett.setValue( SETT_DEFAULT_PATH, fi.absolutePath() );
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
