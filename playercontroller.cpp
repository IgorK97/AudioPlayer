#include "PlayerController.h"
#include "AudioInfo.h"

#include <QMediaDevices>
#include <QAudioDevice>
#include <QAudioOutput>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QEventLoop>
#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QVariantMap>
#include <QFileInfo>
#include <QtGlobal>
#include <QRandomGenerator>
#include <QStringList>
#include "imagehandler.h"
#include "myaudioplayer.h"

PlayerController::PlayerController(const QString &newConnectionName, MyAudioPlayer* map, QObject *parent)
    : QAbstractListModel{parent}
{


    connectionName = newConnectionName;

    db = QSqlDatabase::database(connectionName);
    m_myAudioPlayer=map;

    if(!db.open())
    {
        qDebug()<<"Database connection failed: "<<db.lastError().text();
        // return;
    }

    // QSqlQuery query("DELETE FROM tracks where id=5", db);

    // if(!query.exec())
    //     qDebug()<<query.lastError().text();


    db.close();

    imageHandler=  new ImageHandler();


    connect(m_myAudioPlayer, &MyAudioPlayer::itsTimeToSwitch, this, &PlayerController::switchToNextTrack);
    loadTracks();

}




QString PlayerController::coverImage()const{
    return m_coverImage;
}

void PlayerController::setCoverImage(QString newCoverImage){
    if(m_coverImage==newCoverImage)
        return;
    m_coverImage=newCoverImage;
    emit coverImageChanged();
}

QString PlayerController::currentPlaylist()const{
    return m_currentPlaylist;
}

void PlayerController::setCurrentPlaylist(QString newPlaylist){
    qDebug()<<"Switch to "<<newPlaylist;
    if(m_currentPlaylist==newPlaylist)
        return;
    m_currentPlaylist=newPlaylist;
    loadPlaylist(newPlaylist);
    emit currentPlaylistChanged();
}

void PlayerController::loadPlaylist(const QString &name){
    m_playHistory.clear();
    db = QSqlDatabase::database(connectionName);
    if(!db.open()){
        qDebug()<<"Ошибка при подключении к БД";
        return;
    }
    QSqlQuery query(db);
    query.prepare("select id from playlists where name=:name");
    query.bindValue(":name", name);
    query.exec();
    query.next();
    int playlistId = query.value("id").toInt();

    if(m_audioList.count()>0){
        beginRemoveRows(QModelIndex(), 0, m_audioList.count()-1);
        m_audioList.clear();
        endRemoveRows();
    }
    qDebug()<<"I am here, it is Okey~ PlaylistId: "<<playlistId;
    QSqlQuery queryTracks(db);
    queryTracks.prepare("SELECT t.id, t.title, t.file_path, "
                        "art.name, alb.name, g.genre, "
                        "t.duration, t.songText "
                        "from tracks as t "
                        "JOIN artists as art ON t.artistId=art.Id "
                        "JOIN albums as alb ON t.albumId=alb.Id "
                        "JOIN genres as g ON t.genreId=g.id "
                        "JOIN playlists_tracks as pt ON t.id=pt.trackId "
                        "WHERE pt.playlistId=:playlistId "
                        "ORDER BY t.title");
    queryTracks.bindValue(":playlistId", playlistId);
    if(!queryTracks.exec()){
        qDebug()<<"Ошибка загрузки плейлиста "<<query.lastError().text();
    }
    int count=0;
    while(queryTracks.next()){
        int id = queryTracks.value(0).toInt();
        QString title = queryTracks.value(1).toString();
        QUrl filePath = queryTracks.value(2).toUrl();
        QString artist = queryTracks.value(3).toString();
        QString album = queryTracks.value(4).toString();
        QString genre = queryTracks.value(5).toString();
        QString duration = queryTracks.value(6).toString();

        // QString formattedDuration = formatDuration(duration);

        QString songText = queryTracks.value(7).toString();
        bool isBeloved = true;
        qDebug()<<title;
        addToModel(id, title, artist, album, genre, songText, duration, filePath, isBeloved, count);
        count++;
    }
    qDebug()<<count;
    db.close();
}

void PlayerController::addToPlaylist(int trackId, QString playlistName){
    db = QSqlDatabase::database(connectionName);
    if(!db.open()){
        qDebug()<<"Ошибка при подключении к бд";
        return;
    }

    QSqlQuery query(db);
    query.prepare("select id from playlists where name=:name");
    query.bindValue(":name", playlistName);
    query.exec();
    query.next();
    int playlistId = query.value("id").toInt();

    QSqlQuery insertQuery(db);
    insertQuery.prepare("insert into playlists_tracks (playlistId, trackId) values(:playlistId, :trackId)");
    insertQuery.bindValue(":playlistId", playlistId);
    insertQuery.bindValue(":trackId", trackId);
    insertQuery.exec();
    if(m_currentPlaylist==playlistName)
        loadPlaylist(playlistName);
    db.close();

}

void PlayerController::removeFromPlaylist(int trackId){
    db = QSqlDatabase::database(connectionName);
    if(!db.open()){
        qDebug()<<"Ошибка при подключении к бд";
        return;
    }

    QSqlQuery query(db);
    query.prepare("select id from playlists where name=:name");
    query.bindValue(":name", m_currentPlaylist);
    query.exec();
    query.next();
    int playlistId = query.value("id").toInt();

    QSqlQuery deleteQuery(db);
    deleteQuery.prepare("delete from playlists_tracks where trackId=:trackId AND playlistId=:playlistId");
    deleteQuery.bindValue(":playlistId", playlistId);
    deleteQuery.bindValue(":trackId", trackId);
    deleteQuery.exec();
    loadPlaylist(m_currentPlaylist);
    db.close();
}



QString PlayerController::title()const{
    return m_title;
}

void PlayerController::setTitle(QString newTitle){
    if(m_title==newTitle)
        return;
    m_title=newTitle;
    emit titleChanged();
}

QString PlayerController::artist()const{
    return m_artist;
}

void PlayerController::setArtist(QString newArtist){
    if(m_artist==newArtist)
        return;
    m_artist=newArtist;
    emit artistChanged();
}

bool PlayerController::shuffle()const{
    return m_shuffle;
}

void PlayerController::setShuffle(bool newShuffle){
    if(m_shuffle==newShuffle)
        return;
    m_shuffle=newShuffle;
    emit shuffleChanged();
}

QString PlayerController::album()const{
    return m_album;
}

void PlayerController::setAlbum(QString newAlbum){
    if(m_album==newAlbum)
        return;
    m_album=newAlbum;
    emit albumChanged();
}





int PlayerController::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_audioList.length();
}

QVariant PlayerController::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && index.row() >= 0 && index.row() < m_audioList.length()) {
        AudioInfo * audioInfo = m_audioList[index.row()];

        switch((Role) role) {
        case AudioTitleRole:
            return audioInfo->title();
        case AudioFilePathRole:
            return audioInfo->filePath();
        case AudioArtistRole:
            return audioInfo->artist();
        case AudioAlbumRole:
            return audioInfo->album();
        case AudioGenreRole:
            return audioInfo->genre();
        case AudioDurationRole:
            return audioInfo->duration();
        case AudioSongTextRole:
            return audioInfo->songText();
        // case AudioImageBase64Role:
        //     return audioInfo->imageBase64();
        case AudioIndexRole:
            return audioInfo->index();
        case AudioInFavoriteRole:
            return audioInfo->inFavorite();
        case AudioModelIndexRole:
            return audioInfo->modelIndex();
        }
    }

    return {};
}

QHash<int, QByteArray> PlayerController::roleNames() const
{
    QHash<int, QByteArray> result;

    result[AudioTitleRole] = "audioTitle";
    result[AudioFilePathRole] = "audioFilePath";
    result[AudioArtistRole] = "audioArtist";
    result[AudioAlbumRole] = "audioAlbum";
    result[AudioGenreRole] = "audioGenre";
    result[AudioDurationRole]="audioDuration";
    result[AudioSongTextRole]="audioSongText";
    // result[AudioImageBase64Role]="audioImage";
    result[AudioIndexRole]="audioIndex";
    result[AudioInFavoriteRole]="audioInFavorite";
    result[AudioModelIndexRole] = "audioModelIndex";
    return result;
}

void PlayerController::switchToNextTrack()
{
    if(m_audioList.isEmpty())
        return;

    int newIndex = 0;
    int currentIndex = m_audioList.indexOf(m_currentTrack);
    if(m_shuffle){
        if(currentIndex !=-1)
            m_playHistory.push(currentIndex);
        if(!m_playFuture.isEmpty()){
            int futureIndex = m_playFuture.pop();
            setCurrentTrack(futureIndex);
        }
        else{
            do{
                newIndex = QRandomGenerator::global()->bounded(m_audioList.size());
            } while(currentIndex == newIndex && m_audioList.size()>1);
        }
    }
    else{
        // int currentIndex = m_audioList.indexOf(m_currentTrack);
        newIndex = (currentIndex+1)%m_audioList.size();
    }
    setCurrentTrack(newIndex);



}

void PlayerController::switchToPreviousTrack()
{

    if(m_audioList.isEmpty())
        return;
    int newIndex = 0;
    int currentIndex = m_audioList.indexOf(m_currentTrack);
    if(m_shuffle){
        if(currentIndex!=-1)
            m_playFuture.push(currentIndex);
        if(!m_playHistory.isEmpty()){
            int previousIndex = m_playHistory.pop();
            setCurrentTrack(previousIndex);
        }
    }
    else{
        int currentIndex = m_audioList.indexOf(m_currentTrack);
        int previousIndex = (currentIndex-1+m_audioList.size())%m_audioList.size();
        setCurrentTrack(previousIndex);
    }


}


QString PlayerController::formatDuration(const QString &durationStr){
    bool ok = false;
    qint64 ms = durationStr.toLongLong(&ok);
    if(!ok||ms<=0) return "00:00";

    int totalSeconds = ms/1000;
    int minutes = totalSeconds/60;
    int seconds = totalSeconds%60;
    return QString("%1:%2")
        .arg(minutes,2,10,QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
}

bool PlayerController::isInDatabase(const QString& filePath, int& id){
    QSqlQuery checkQuery(db);
    checkQuery.prepare(R"(
SELECT id FROM tracks where file_path=:filePath
)");

    checkQuery.bindValue(":filePath", filePath);
    if(!checkQuery.exec())return false;
    if(checkQuery.next()){
        // bool result = checkQuery.value(0).toInt()>0;
        id = checkQuery.value("id").toInt();
        return true;
    }
    return false;
}

int PlayerController::addAudioToDb(const QString &title,
                                    const QString &filePath,
                                    const int& artistId,
                                    const int &albumId,
                                    const int &genreId,
                                    const QString& duration,
                                    const QString& songText)
{

    QSqlQuery query(db);
    query.prepare(R"(
insert into tracks(title, file_path, artistId, albumId, genreId, duration, songText)
values (:title, :file_path, :artistId, :albumId, :genreId, :duration, :songText)
)");
    qDebug()<<"TITLE: "<<title;
    query.bindValue(":title", title);
    query.bindValue(":file_path", filePath);
    query.bindValue(":artistId", artistId);
    query.bindValue(":albumId", albumId);
    query.bindValue(":genreId", genreId);
    query.bindValue(":duration", duration);
    query.bindValue(":songText", songText);

    if(!query.exec()){
        qWarning()<<"Ошибка добавления"<<query.lastError().text();
        return -1;
    }

    return query.lastInsertId().toInt();

}

int PlayerController::getIdFromDb(const QString &table,
                                  const QString &column,
                                  const QString &value){
    if(value.trimmed().isEmpty())
        return 1;
    QSqlQuery query(db);
    query.prepare(QString("SELECT id FROM %1 WHERE %2 = :value").arg(table, column));
    query.bindValue(":value", value);
    if(query.exec()&& query.next()){
        return query.value(0).toInt();
    }
    return 0;
}

int PlayerController::insertValueToDb(const QString& table,
                                      const QString &column,
                                      const QString &value){
    QSqlQuery query(db);
    query.prepare(QString("INSERT INTO %1 (%2) VALUES (:value)").arg(table, column));
    query.bindValue(":value", value);
    if(!query.exec())
        return 1;

    return query.lastInsertId().toInt(0);
}

void PlayerController::openAudio(const QUrl& newFilePath){

    qDebug()<<"Adding new track to DB (Добавление нового трека в бд)";
    db = QSqlDatabase::database(connectionName);
    if(!db.open())
    {
        qDebug()<<"Database connection failed: "<<db.lastError().text();
        return;
    }


    int oldId=-1;
    qDebug()<<newFilePath.toLocalFile();
    if(isInDatabase(newFilePath.toLocalFile(), oldId)){
        qDebug()<<"Трек уже в бд: "<<newFilePath.toLocalFile();
        qDebug()<<oldId;
        int count = 0;
        while(count<m_audioList.count()){
            if(m_audioList[count]->index()==oldId){
                setCurrentTrack(m_audioList[count]);
            }
            count++;
        }
        // changeAudioSource(newFilePath);
        db.close();
        return;
    }
    // changeAudioSource(newFilePath);

    // QEventLoop loop;
    // QObject::connect(&m_mediaPlayer, &QMediaPlayer::metaDataChanged, &loop, &QEventLoop::quit);
    // loop.exec();
    qDebug()<<"Loop is over (Proverka)";
    QMediaPlayer secmp;
    secmp.setSource(newFilePath);

    auto meta = secmp.metaData();
    QString localFile_path = newFilePath.toLocalFile();
    QString localTitle = meta.stringValue(QMediaMetaData::Title);

    if(localTitle.isEmpty()){
        QFileInfo fileInfo(localFile_path);
        // title = fileInfo.fileName();
        localTitle=fileInfo.baseName();
    }

    QString artist_composer = meta.stringValue(QMediaMetaData::ContributingArtist);

    int artistId = getIdFromDb("artists", "name", artist_composer);
    if(artistId<1){
        artistId = insertValueToDb("artists", "name", artist_composer);
    }

    QString album = meta.stringValue(QMediaMetaData::AlbumTitle);

    int albumId = getIdFromDb("albums", "name", album);
    if(albumId<1){
        albumId = insertValueToDb("albums", "name", album);
    }

    QString duration = meta.stringValue(QMediaMetaData::Duration);
    // qDebug()<<"DURATION: "<<secmp.duration();
    // QString duration="";
    // qint64 durMs = secmp.duration();
    // qDebug()<<"DURATION: "<<durMs;
    // QVariant durationVariant = meta.value(QMediaMetaData::Duration);
    // if(durationVariant.isValid()){
    //     qint64 durationMs = durationVariant.toLongLong();
    //     int durationSeconds = durationMs/1000;
    //     qDebug()<<"DURATION: "<<durationSeconds;
    // }
    QString lyrics = "";
    QString genre = meta.stringValue(QMediaMetaData::Genre);

    int genreId = getIdFromDb("genres", "genre", genre);
    if(genreId<1){
        genreId = insertValueToDb("genres", "genre", genre);
    }
    qDebug()<<localTitle<<" "<<localFile_path<<" "<<artistId <<" "<<albumId<<" "<<genreId<< " "<<duration;
    int newTrackId = addAudioToDb(localTitle, localFile_path, artistId, albumId, genreId,
                 duration, lyrics);

    db.close();
    loadTracks();
    // playPause();

    for(auto* track : as_const(m_audioList)){
        if(track->index()==newTrackId){
            setCurrentTrack(track);
            qDebug()<<"Трек найден!";
            break;
        }
    }

}


void PlayerController::addToModel(const int &index,
                                  const QString& title,
                                  const QString& artist,
                                  const QString& album,
                                  const QString& genre,
                                  const QString &songtext,
                                  const QString &duration,
                                  const QUrl& filePath,
                                  const bool& isBeloved,
                                  const int& modelIndex){
    beginInsertRows(QModelIndex(), m_audioList.length(), m_audioList.length());

    AudioInfo *audioInfo = new AudioInfo(this);
    audioInfo->setIndex(index);
    audioInfo->setTitle(title);
    audioInfo->setAlbum(album);
    audioInfo->setArtist(artist);
    audioInfo->setGenre(genre);
    audioInfo->setSongText(songtext);
    audioInfo->setFilePath(filePath);
    // qDebug()<<"FILEPATH: "<<filePath;
    audioInfo->setDuration(duration);
    audioInfo->setInFavorite(isBeloved);
    audioInfo->setModelIndex(modelIndex);
    m_audioList<<audioInfo;

    endInsertRows();
}

void PlayerController::loadFavorites(){
    setCurrentPlaylist("");
    m_playHistory.clear();
    db =QSqlDatabase::database(connectionName);
    if(!db.open()){
        qDebug()<<"Ошибка подключения: "<<db.lastError().text();
        return;
    }
    // int lastIndex = 0 > (m_audioList.count()-1) ? 0 : m_audioList.count()-1;
    if(m_audioList.count()>0){
        beginRemoveRows(QModelIndex(), 0, m_audioList.count()-1);
        m_audioList.clear();
        endRemoveRows();
    }
    qDebug()<<"I am here, it is Okey~";
    QSqlQuery query("SELECT t.id, t.title, t.file_path, "
                    "art.name, alb.name, g.genre, "
                    "t.duration, t.songText "
                    "from tracks as t "
                    "JOIN artists as art ON t.artistId=art.Id "
                    "JOIN albums as alb ON t.albumId=alb.Id "
                    "JOIN genres as g ON t.genreId=g.id "
                    "JOIN favorites as f ON t.id=f.trackId "
                    "ORDER BY t.title", db);
    if(!query.exec()){
        qDebug()<<"Ошибка загрузки избранного"<<query.lastError().text();
    }
    int count=0;
    while(query.next()){
        int id = query.value(0).toInt();
        QString title = query.value(1).toString();
        QUrl filePath = query.value(2).toUrl();
        QString artist = query.value(3).toString();
        QString album = query.value(4).toString();
        QString genre = query.value(5).toString();
        QString duration = query.value(6).toString();

        // QString formattedDuration = formatDuration(duration);

        QString songText = query.value(7).toString();
        bool isBeloved = true;
        qDebug()<<title;
        addToModel(id, title, artist, album, genre, songText, duration, filePath, isBeloved, count);

        count++;
    }
    qDebug()<<count;
    db.close();

}


void PlayerController::loadTracks(){
    setCurrentPlaylist("");
    m_playHistory.clear();
    db =QSqlDatabase::database(connectionName);
    if(!db.open()){
        qDebug()<<"Ошибка подключения: "<<db.lastError().text();
        return;
    }
    // int lastIndex = 0 > (m_audioList.count()-1) ? 0 : m_audioList.count()-1;
    if(m_audioList.count()>0){
beginRemoveRows(QModelIndex(), 0, m_audioList.count()-1);
    m_audioList.clear();
    endRemoveRows();
    }
    qDebug()<<"I am here, it is Okey~";
    QSqlQuery query("SELECT t.id, t.title, t.file_path, "
                    "art.name, alb.name, g.genre, "
                    "t.duration, t.songText "
                    "from tracks as t "
                    "JOIN artists as art ON t.artistId=art.Id "
                    "JOIN albums as alb ON t.albumId=alb.Id "
                    "JOIN genres as g ON t.genreId=g.id "
                    "ORDER BY t.title", db);
    if(!query.exec()){
        qDebug()<<"Error fetching data: "<<query.lastError().text();
    }
    int count=0;
    while(query.next()){
        int id = query.value(0).toInt();
        QString title = query.value(1).toString();
        QUrl filePath = query.value(2).toUrl();
        QString artist = query.value(3).toString();
        QString album = query.value(4).toString();
        QString genre = query.value(5).toString();
        QString duration = query.value(6).toString();

        // QString formattedDuration = formatDuration(duration);

        QString songText = query.value(7).toString();
        bool isBeloved = isFavorite(id);
        qDebug()<<"QWERTY " << title;
        qDebug()<<title<<"---"<<filePath;
        addToModel(id, title, artist, album, genre, songText, duration, filePath, isBeloved, count);
        count++;
    }
    db.close();

}

bool PlayerController::isFavorite(const int& id){
    QSqlQuery checkQuery(db);
    checkQuery.prepare(R"(
SELECT COUNT(*) FROM favorites where trackId=:trackId
)");

    checkQuery.bindValue(":trackId", id);
    if(!checkQuery.exec()){
        qDebug()<<"Ошибка проверки"<<checkQuery.lastError().text();
        return false;
    }
    if(checkQuery.next()){

        return checkQuery.value(0).toInt()>0;
    }
    return false;
}

QVariantMap PlayerController::getTrack(int index){
    QVariantMap result;

    if(index-1>=0){
        int i=0;
        while(i<m_audioList.count()){
            if(m_audioList[i]->index()==index)
                break;
            i++;
        }
        auto audioTrack = m_audioList[i];
        result["title"]=audioTrack->title();
        result["artist"]=audioTrack->artist();
        result["album"]=audioTrack->album();
        result["genre"]=audioTrack->genre();
        result["songText"]=audioTrack->songText();
    }
    return result;
}

// bool PlayerController::checkAudio(const QUrl& filePath){
//     return true;
// }

void PlayerController::addToFavorites(int index){
    db = QSqlDatabase::database(connectionName);
    QSqlQuery query(db);
    query.prepare("INSERT into favorites(trackId, userId) VALUES(:trackId, :userId)");
    int userId=1;
    query.bindValue(":trackId", index);
    query.bindValue(":userId", userId);
    if(!query.exec()){
        qDebug()<<"Ошибка при добавлении в избранное"<<query.lastError().text();
    }
    db.close();
    loadTracks();
}


void PlayerController::removeFromFavorites(int index){
    db = QSqlDatabase::database(connectionName);
    QSqlQuery query(db);
    query.prepare("DELETE from favorites where trackId=:trackId");
    query.bindValue(":trackId", index);
    if(!query.exec()){
        qDebug()<<"Ошибка при удалении из избранного"<<query.lastError().text();
    }
    db.close();
    loadTracks();
}


void PlayerController::filterTracks(const QString& artist,
                                    const QString& album,
                                    const QString& genre){
    db =QSqlDatabase::database(connectionName);
    qDebug()<<artist<<" "<<album <<" "<<genre;
    // QSqlQuery query(db);
    m_playHistory.clear();

    QString baseQuery="SELECT t.id, t.title, t.file_path, "
                        "art.name, alb.name, g.genre, "
                        "t.duration, t.songText "
                        "from tracks as t "
                        "JOIN artists as art ON t.artistId=art.Id "
                        "JOIN albums as alb ON t.albumId=alb.Id "
                        "JOIN genres as g ON t.genreId=g.id ";
    // QStringList conditions;
    int c=0;
    if(!artist.isEmpty()&&artist!="Все исполнители")
    {
        baseQuery+=" WHERE art.name = :artist";
        c=1;
    }
    if(!album.isEmpty()&&album!="Все альбомы")
    {
        if(c)
            baseQuery+=" AND ";
        else
            baseQuery+=" WHERE ";
        baseQuery+=" alb.name = :album";
        c=1;
    }
    if(!genre.isEmpty()&&genre!="Все жанры")
    {
        if(c)
            baseQuery+=" AND ";
        else
            baseQuery+=" WHERE ";
        baseQuery+=" g.genre = :genre";
        // c=1;
    }
    qDebug()<<baseQuery;
    QSqlQuery query(db);
    query.prepare(baseQuery);
    query.bindValue(":artist", artist);
    query.bindValue(":genre", genre);
    query.bindValue(":album", album);
    if(!query.exec()){
        qDebug()<<"Error fetching data: "<<query.lastError().text();
    }
    if(m_audioList.count()>0){
        beginRemoveRows(QModelIndex(), 0, m_audioList.count()-1);
        m_audioList.clear();
        endRemoveRows();
    }
    int count=0;
    while(query.next()){
        int id = query.value(0).toInt();
        QString title = query.value(1).toString();
        QUrl filePath = query.value(2).toUrl();
        QString artist = query.value(3).toString();
        QString album = query.value(4).toString();
        QString genre = query.value(5).toString();
        QString duration = query.value(6).toString();

        // QString formattedDuration = formatDuration(duration);

        QString songText = query.value(7).toString();
        bool isBeloved = isFavorite(id);
        qDebug()<<"QWERTY " << title;
        addToModel(id, title, artist, album, genre, songText, duration, filePath, isBeloved, count);
        count++;
    }
    db.close();
}

void PlayerController::updateAudio(const int& index,
                                   const QString& title,
                                   const QString& artist,
                                   const QString& album,
                                   const QString& genre){

    db = QSqlDatabase::database(connectionName);
    if(!db.open()){
        qDebug()<<"Ошибка подключения: "<<db.lastError().text();
        return;
    }
    QSqlQuery artistQuery(db);
    artistQuery.prepare("SELECT id from artists where name = :artist");
    artistQuery.bindValue(":artist", artist);
    if(!artistQuery.exec()){
        qDebug()<<"Error fetching artist name"<<artistQuery.lastError().text();
        return;
    }
    artistQuery.next();
    int artistId = artistQuery.value(0).toInt();

    QSqlQuery albumQuery(db);
    albumQuery.prepare("SELECT id from albums where name = :album");
    albumQuery.bindValue(":album", album);
    if(!albumQuery.exec()){
        qDebug()<<"Ошибка загрузки данных"<<albumQuery.lastError().text();
        return;
    }
    albumQuery.next();
    int albumId = albumQuery.value(0).toInt();

    QSqlQuery genreQuery(db);
    genreQuery.prepare("SELECT id from genres where genre = :genre");
    genreQuery.bindValue(":genre", genre);
    if(!genreQuery.exec()){
        qDebug()<<"Ошибка загрузки данных"<<genreQuery.lastError().text();
        return;
    }
    genreQuery.next();
    int genreId = genreQuery.value(0).toInt();

    QSqlQuery query(db);
    query.prepare(R"(
UPDATE tracks SET title=:title, artistId=:artistId, albumId=:albumId, genreId=:genreId where id=:index
)");
    query.bindValue(":title", title);
    query.bindValue(":artistId", artistId);
    query.bindValue(":albumId", albumId);
    query.bindValue(":genreId", genreId);
    query.bindValue(":index", index);
    if(!query.exec()){
        qDebug()<<"Ошибка загрузки данных"<<query.lastError().text();
        return;
    }
    db.close();
    loadTracks();
}

void PlayerController::removeAudio(int index)
{
    db = QSqlDatabase::database(connectionName);
    if(!db.open()){
        qDebug()<<"Ошибка загрузки данных: "<<db.lastError().text();
        return;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM tracks where id=:trackId");
    query.bindValue(":trackId", index);//index - идентификатор должен быть!!! Проверить потом!
    if(!query.exec()){
        qDebug()<<"Ошибка удаления трека: "<<query.lastError().text();
    }
    if(m_audioList.count()>0){
        beginRemoveRows(QModelIndex(), 0, m_audioList.count()-1);
        m_audioList.clear();
        endRemoveRows();
    }
    db.close();
    loadTracks();

}



void PlayerController::setCurrentTrack(int index)
{
    qDebug()<<index <<"<-----";
    if (index >= 0 && index < m_audioList.length()) {
        setCurrentTrack(m_audioList[index]);
    }
    // playPause();

}

AudioInfo *PlayerController::currentTrack() const
{
    return m_currentTrack;
}

void PlayerController::setCurrentTrack(AudioInfo *newCurrentTrack)
{
    // if (m_currentTrack == newCurrentTrack)
    //     return;
    qDebug()<<newCurrentTrack->filePath()<<"---"<<newCurrentTrack->title();
    m_currentTrack = newCurrentTrack;
    emit currentTrackChanged();
    QString imageBase64 = imageHandler->extractCoverFromMp3(newCurrentTrack->filePath());
    setCoverImage(imageBase64);
    setTitle(newCurrentTrack->title());
    setArtist(newCurrentTrack->artist());
    setAlbum(newCurrentTrack->album());
    if (m_currentTrack) {
        m_myAudioPlayer->changeAudioSource(m_currentTrack->filePath());
    } else {
        // m_myAudioPlayer->changeAudioSource(QUrl());
        m_myAudioPlayer->setPlaying(false);
        // m_mediaPlayer.stop();
        // m_mediaPlayer.setSource(QUrl());
        // m_playing = false;
        // emit playingChanged();
    }
}
