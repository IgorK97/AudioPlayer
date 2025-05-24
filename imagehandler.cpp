#include "imagehandler.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <QDebug>
#include <QString>

using namespace std;

ImageHandler::ImageHandler(QObject *parent) : QObject(parent){}

uint32_t ImageHandler::syncSafeToInt(const char bytes[4]){
    return ((bytes[0] & 0x7F) << 21) |
           ((bytes[1] & 0x7F) << 14) |
           ((bytes[2] & 0x7F) << 7) |
           (bytes[3] & 0x7F);
}


QString ImageHandler::extractCoverFromMp3(const QUrl& mp3FilePath){
    ifstream mp3(mp3FilePath.toString().toStdString(), ios::binary);
    if(!mp3){
        qDebug()<<"Error: Не получилось открыть файл."<< mp3FilePath.toString();
        return "file:///C:/Users/IgorA/source/repos/MyProjects/QtProjects/MyAudioPlayer2/build/Desktop_Qt_6_10_0_MSVC2022_64bit-Debug/MyAudioPlayer2/assets/images/defaultImage.png";
    }

    char header[10];
    mp3.read(header, 10);

    if(string(header, 3)!="ID3"){
        qDebug()<<"Error: Тег ID3v2 не найден.";
        return "file:///C:/Users/IgorA/source/repos/MyProjects/QtProjects/MyAudioPlayer2/build/Desktop_Qt_6_10_0_MSVC2022_64bit-Debug/MyAudioPlayer2/assets/images/defaultImage.png";
    }

    uint32_t tagSize = syncSafeToInt(&header[6]);
    vector<char> tagData(tagSize);

    mp3.read(tagData.data(), tagSize);

    size_t pos = 0;
    while(pos+10<tagData.size()){
        string frameId(&tagData[pos], 4);
        uint32_t frameSize = (unsigned char) tagData[pos+4]<<24 |
                             (unsigned char)tagData[pos+5]<<16|
                             (unsigned char)tagData[pos+6]<<8|
                             (unsigned char)tagData[pos+7];
        if(frameSize==0||pos+10+frameSize>tagData.size())
            break;

        if(frameId == "APIC"){
            size_t apicPos = pos + 10;
            char encoding = tagData[apicPos];
            size_t mimeEnd = tagData.data() + tagSize - tagData.data() - apicPos > 0
                                 ? apicPos + 1
                                 : 0;
            while (tagData[mimeEnd] != 0) ++mimeEnd;
            std::string mime(&tagData[apicPos + 1], mimeEnd - apicPos - 1);

            size_t picTypePos = mimeEnd + 1;
            size_t descEnd = picTypePos;
            while (tagData[descEnd] != 0) ++descEnd;

            size_t imageDataPos = descEnd + 1;
            size_t imageDataSize = frameSize - (imageDataPos - pos - 10);

            std::string ext = (mime.find("png") != std::string::npos) ? "png" : "jpg";

            QByteArray imageData(&tagData[imageDataPos],static_cast<int>(imageDataSize));
            QString str = "data:image/" + QString::fromStdString(ext)+";base64, "+imageData.toBase64();
            return str;

        }

        pos+=10+frameSize;
    }
    qDebug()<<"Error: Нет APIC фрейма.";//я сейчас работаю только с mp3
    return "file:///C:/Users/IgorA/source/repos/MyProjects/QtProjects/MyAudioPlayer2/build/Desktop_Qt_6_10_0_MSVC2022_64bit-Debug/MyAudioPlayer2/assets/images/defaultImage.png";
}
