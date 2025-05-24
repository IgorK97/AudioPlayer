
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QImage>
#include <QFileInfo>
#include <QEventLoop>
#include <QByteArray>
#include <QIcon>
#include "PlayerController.h"
#include "myaudioplayer.h"
#include <databasehandler.h>
#include <QQmlContext>
#include "imagehandler.h"
#include <QDebug>


int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "my_database");
    db.setDatabaseName("database/my_audio_tracks_db.db");

    if(!db.open()){
        qCritical()<<"Ошибка подключения к DB: "<<db.lastError().text();
        return -1;
    }




    QStringList tables = db.tables();

    qDebug() << "Table Lists:";

    foreach (const QString &table, tables) {

        qDebug() << table;

    }

    db.close();
    DatabaseHandler* dbHandler = new DatabaseHandler("my_database");
    ImageHandler* imgMp3 = new ImageHandler();

    engine.rootContext()->setContextProperty("dbHandler", dbHandler);
    engine.rootContext()->setContextProperty("imgMp3", imgMp3);
    MyAudioPlayer* myAudioPlayer = new MyAudioPlayer();
    PlayerController *playerController = new PlayerController("my_database", myAudioPlayer, &app);

    qmlRegisterSingletonInstance("com.company.PlayerController", 1, 0, "PlayerController", playerController);
    qmlRegisterSingletonInstance("com.company.MyAudioPlayer", 1, 0,"MyAudioPlayer", myAudioPlayer);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("MyAudioPlayer2", "Main");

    return app.exec();
}



