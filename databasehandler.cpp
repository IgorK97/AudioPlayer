#include "databasehandler.h"
#include <QSqlError>
#include <QDebug>

DatabaseHandler::DatabaseHandler(QString newConnectionName, QObject *parent) : QObject(parent) {

    artistsModel = new QStandardItemModel(this);
    albumsModel=new QStandardItemModel(this);
    genresModel=new QStandardItemModel(this);
    playlistsModel=new QStandardItemModel(this);
    // db = newDb;
    connectionName = newConnectionName;
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    // db = QSqlDatabase::addDatabase("QODBC");
    // db.setDatabaseName("DRIVER={SQL Server};SERVER=localhost\\SQLEXPRESS;DATABASE=ApartmentDB;Trusted_Connection=yes;");
    if (!db.open()) {
        qDebug() << "Database connection failed:" << db.lastError().text();
        return;
    }
    db.close();
    loadArtists();

    loadAlbums();

    loadGenres();

    loadPlaylists();
    qDebug()<<"TUTA";
}

DatabaseHandler::~DatabaseHandler() {
    // db.close();
}

void DatabaseHandler::renamePlaylist(QString oldName, QString newName){
    db = QSqlDatabase::database(connectionName);
    if(!db.open()){
        qDebug()<<"Не удалось подключиться к бд!";
        return;
    }

    QSqlQuery query(db);
    query.prepare("update playlists set name=:newName where name=:oldName");
    query.bindValue(":oldName", oldName);
    query.bindValue(":newName", newName);
    if(!query.exec())
        qDebug()<<"Ошибка при изменении наименования плейлиста";
    loadPlaylists();
}

void DatabaseHandler::deleteAlbum(int albumIdx){
    if(albumIdx<0||albumIdx>=albumsModel->rowCount()){
        qWarning()<<"Индекс альбома указан неверно!";
        return;
    }
    db=QSqlDatabase::database(connectionName);
    if (!db.open()) {
        qDebug() << "Ошибка подключения:" << db.lastError().text();
        return;
    }

    QStandardItem *item = albumsModel->item(albumIdx);
    int albumId=item->data(Qt::UserRole+1).toInt();
    // qDebug()<<districtId;
    QSqlQuery query;
    query.prepare("DELETE FROM albums WHERE id=:id");
    query.bindValue(":id",albumId);
    if(!query.exec()){
        qDebug()<<"Не удалось удалить альбом: " << query.lastError().text();
        return;
    }
    db.close();
    loadAlbums();
}

void DatabaseHandler::deleteArtist(int artistIdx){
    if(artistIdx<0||artistIdx>=artistsModel->rowCount()){
        qWarning()<<"Индекс исполнителя указан неверно!";
        return;
    }
    db=QSqlDatabase::database(connectionName);
    if (!db.open()) {
        qDebug() << "Ошибка подключения:" << db.lastError().text();
        return;
    }

    QStandardItem *item = artistsModel->item(artistIdx);
    int artistId=item->data(Qt::UserRole+1).toInt();
    // qDebug()<<districtId;
    QSqlQuery query;
    query.prepare("DELETE FROM artists WHERE id=:id");
    query.bindValue(":id",artistId);
    if(!query.exec()){
        qDebug()<<"Не удалось удалить исполнителя: " << query.lastError().text();
        return;
    }
    db.close();
    loadArtists();
}

void DatabaseHandler::deleteGenre(int genreIdx){
    if(genreIdx<0||genreIdx>=genresModel->rowCount()){
        qWarning()<<"Индекс жанра указан неврено, проверьте еще раз!!";
        return;
    }
    db=QSqlDatabase::database(connectionName);
    if (!db.open()) {
        qDebug() << "Ошибка подключения:" << db.lastError().text();
        return;
    }

    QStandardItem *item = genresModel->item(genreIdx);
    int genreId=item->data(Qt::UserRole+1).toInt();
    // qDebug()<<districtId;
    QSqlQuery query;
    query.prepare("DELETE FROM albums WHERE id=:id");
    query.bindValue(":id",genreId);
    if(!query.exec()){
        qDebug()<<"Не удалось удалить альбом: " << query.lastError().text();
        return;
    }
    db.close();
    loadGenres();
}

void DatabaseHandler::deletePlaylist(const QString &name){
    db=QSqlDatabase::database(connectionName);
    if (!db.open()) {
        qDebug() << "Ошибка подключения:" << db.lastError().text();
        return;
    }
    int playlistId=-1;
    for(int i=0;i<playlistsModel->rowCount();i++){
        QStandardItem *item = playlistsModel->item(i);
        if(item && item->text()==name){
            playlistId = item->data(Qt::UserRole+1).toInt();
            break;
        }
    }
    if(playlistId==-1){
        qDebug()<<"Плейлист не найден";
        db.close();
        return;
    }

    // qDebug()<<districtId;
    QSqlQuery query(db);
    query.prepare("DELETE FROM playlists WHERE id=:id");
    query.bindValue(":id",playlistId);
    if(!query.exec()){
        qDebug()<<"Не удалось удалить плейлист: " << query.lastError().text();
        return;
    }
    db.close();
    loadPlaylists();
}

void DatabaseHandler::loadPlaylists(){
        db = QSqlDatabase::database(connectionName);
        if (!db.open()) {
            qDebug() << "Ошибка подключения:" << db.lastError().text();
            return;
        }
        playlistsModel->clear();
        qDebug()<<"I am here";
        // QStringList districts;
        QSqlQuery query("SELECT id, name FROM playlists ORDER BY name", db);

        while (query.next()) {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            QStandardItem *item = new QStandardItem(name);
            item->setData(id,Qt::UserRole+1);
            playlistsModel->appendRow(item);
            // districts.append(query.value(0).toString());
        }
        db.close();
        // districtsModel.setStringList(districts);
        emit playlistsChanged();
}

void DatabaseHandler::loadArtists() {
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    if (!db.open()) {
        qDebug() << "Ошибка подключения:" << db.lastError().text();
        return;
    }
    artistsModel->clear();
    QStandardItem *firstItem = new QStandardItem("Все исполнители");
    firstItem->setData(0,Qt::UserRole+1);
    artistsModel->appendRow(firstItem);
    // QStringList districts;
    QSqlQuery query("SELECT id, name FROM artists ORDER BY name", db);

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QStandardItem *item = new QStandardItem(name);
        item->setData(id,Qt::UserRole+1);
        artistsModel->appendRow(item);
        // districts.append(query.value(0).toString());
    }
    db.close();
    // districtsModel.setStringList(districts);
    emit artistsChanged();
}
void DatabaseHandler::loadAlbums() {
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    if (!db.open()) {
        qDebug() << "Ошибка подключения:" << db.lastError().text();
        return;
    }

    albumsModel->clear();
    QStandardItem *firstItem = new QStandardItem("Все альбомы");
    firstItem->setData(0,Qt::UserRole+1);
    albumsModel->appendRow(firstItem);
    // QStringList streets;
    QSqlQuery query("SELECT id, name FROM albums ", db);
    // QSqlQuery query("SELECT name FROM streets");
    // query.prepare("SELECT id, name FROM albums ");
    // if(!query.exec()){
    //     qDebug()<<"Ошибка при получении улиц: " << query.lastError().text();
    //     return;
    // }
    while (query.next()) {
        int id=query.value(0).toInt();
        QString name=query.value(1).toString();
        QStandardItem *item=new QStandardItem(name);
        item->setData(id,Qt::UserRole+1);
        albumsModel->appendRow(item);
        // streets.append(query.value(0).toString());
    }

    // streetsModel.setStringList(streets);
    db.close();
    emit albumsChanged();
}
void DatabaseHandler::loadGenres(){
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    if (!db.open()) {
        qDebug() << "Ошибка подключения:" << db.lastError().text();
        return;
    }


    genresModel->clear();
    QStandardItem *firstItem = new QStandardItem("Все жанры");
    firstItem->setData(0,Qt::UserRole+1);
    genresModel->appendRow(firstItem);
    // QStringList houses;
    QSqlQuery query("SELECT id, Genre FROM genres ", db);
    // QSqlQuery query("SELECT name FROM streets");
    // query.prepare("SELECT id, Genre FROM genres ");

    // if(!query.exec()){
    //     qDebug()<<"Ошибка при получении домов: " << query.lastError().text();
    //     return;
    // }
    while (query.next()) {
        int id=query.value(0).toInt();
        QString name = query.value(1).toString();
        QStandardItem *item=new QStandardItem(name);
        item->setData(id,Qt::UserRole+1);

        genresModel->appendRow(item);
        // houses.append(query.value(0).toString());
    }

    // housesModel.setStringList(houses);
    db.close();
    emit genresChanged();
}

int DatabaseHandler::getIndexFromNameForAlbum(const QString& name){
    QStandardItem *item;
    qDebug()<<"Поступившее название улицы: "<<name;
    for(int i=0;i<albumsModel->rowCount();i++){
        item = albumsModel->item(i);
        QString elementName = item->data(Qt::DisplayRole).toString();
        qDebug()<<elementName;
        if(elementName==name)
            return i;
    }
    qDebug()<<-1;
    return -1;
}

int DatabaseHandler::getIndexFromNameForArtist(const QString& name){
    QStandardItem *item;
    qDebug()<<"Поступившее название улицы: "<<name;
    for(int i=0;i<artistsModel->rowCount();i++){
        item = artistsModel->item(i);
        QString elementName = item->data(Qt::DisplayRole).toString();
        qDebug()<<elementName;
        if(elementName==name)
            return i;
    }
    qDebug()<<-1;
    return -1;
}

int DatabaseHandler::getIndexFromNameForGenre(const QString& name){
    QStandardItem *item;
    qDebug()<<"Поступившее название улицы: "<<name;
    for(int i=0;i<genresModel->rowCount();i++){
        item = genresModel->item(i);
        QString elementName = item->data(Qt::DisplayRole).toString();
        qDebug()<<elementName;
        if(elementName==name)
            return i;
    }
    qDebug()<<-1;
    return -1;
}

void DatabaseHandler::addPlaylist(const QString &name){
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    if (!db.open()) {
        qDebug() << "Ошибка подключения:" << db.lastError().text();
        return;
    }
    QSqlQuery query(db);
    query.prepare("INSERT INTO playlists (name, userId) VALUES (:name, :userId)");
    query.bindValue(":name",name);
    int userId=1;
    query.bindValue(":userId",userId);
    if(!query.exec()){
        qDebug()<<"Ошибка при добавлении нового плейлиста: "<<query.lastError().text();
        return;
    }
    db.close();
    loadPlaylists();
    emit playlistsChanged();
}


void DatabaseHandler::addArtist(const QString &name){
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    if (!db.open()) {
        qDebug() << "Ошибка подключения:" << db.lastError().text();
        return;
    }
    QSqlQuery query(db);
    query.prepare("INSERT INTO artists (name) VALUES (:name)");
    query.bindValue(":name",name);
    if(!query.exec()){
        qDebug()<<"Ошибка при добавлении нового исполнителя: "<<query.lastError().text();
        return;
    }
    db.close();
    loadArtists();
    emit artistsChanged();

}
void DatabaseHandler::addAlbum(const QString &name){
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    if (!db.open()) {
        qDebug() << "Ошибка подключения:" << db.lastError().text();
        return;
    }



    QSqlQuery query(db);
    query.prepare("INSERT INTO albums (name) VALUES(:name)");
    query.bindValue(":name",name);

    if(!query.exec()){
        qDebug()<<"Ошибка при добавлении нового альбома: "<< query.lastError().text();
        return;
    }
    db.close();
    loadAlbums();
    emit albumsChanged();


}
void DatabaseHandler::addGenre(const QString &name){
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    if (!db.open()) {
        qDebug() << "Ошибка подключения:" << db.lastError().text();
        return;
    }
    QSqlQuery query(db);
    query.prepare("INSERT INTO genres (genre) VALUES(:name)");
    query.bindValue(":name",name);
    qDebug()<<name;
    if(!query.exec()){
        qDebug()<<"Ошибка при добавлении нового жанра: "<< query.lastError().text();
        return;
    }
    db.close();
    loadGenres();

}

QStandardItemModel* DatabaseHandler::getArtistsModel() {
    return artistsModel;
}
QStandardItemModel* DatabaseHandler::getAlbumsModel(){
    return albumsModel;
}
QStandardItemModel* DatabaseHandler::getGenresModel() {
    return genresModel;
}
QStandardItemModel* DatabaseHandler::getPlaylistsModel(){
    return playlistsModel;
}
