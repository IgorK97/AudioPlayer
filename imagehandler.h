#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <QObject>
#include <QUrl>
using namespace std;

class ImageHandler :public QObject
{
    Q_OBJECT
public:
    ImageHandler(QObject *parent = nullptr);
    Q_INVOKABLE QString extractCoverFromMp3(const QUrl& mp3FilePath);
private:
    uint32_t syncSafeToInt(const char bytes[4]);
};

#endif // IMAGEHANDLER_H
