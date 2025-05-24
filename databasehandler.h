#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringListModel>
#include <QStandardItemModel>

class DatabaseHandler : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStandardItemModel* artistsModel READ getArtistsModel NOTIFY artistsChanged)
    Q_PROPERTY(QStandardItemModel* albumsModel READ getAlbumsModel NOTIFY albumsChanged)
    Q_PROPERTY(QStandardItemModel* genresModel READ getGenresModel NOTIFY genresChanged)
    Q_PROPERTY(QStandardItemModel* playlistsModel READ getPlaylistsModel NOTIFY playlistsChanged)

public:
    explicit DatabaseHandler(QString connectionName, QObject *parent = nullptr);
    ~DatabaseHandler();

    QStandardItemModel* getArtistsModel();
    QStandardItemModel* getAlbumsModel();
    QStandardItemModel* getGenresModel();
    QStandardItemModel* getPlaylistsModel();

    Q_INVOKABLE void renamePlaylist(QString oldName, QString newName);

    Q_INVOKABLE void loadArtists();
    Q_INVOKABLE void loadAlbums();
    Q_INVOKABLE void loadGenres();
    Q_INVOKABLE void loadPlaylists();

    Q_INVOKABLE void addArtist(const QString& name);
    Q_INVOKABLE void addAlbum(const QString& name);
    Q_INVOKABLE void addGenre(const QString& name);
    Q_INVOKABLE void addPlaylist(const QString& name);

    Q_INVOKABLE void deleteArtist(int artistIdx);
    Q_INVOKABLE void deleteAlbum(int albumIdx);
    Q_INVOKABLE void deleteGenre(int genreIdx);
    Q_INVOKABLE void deletePlaylist(const QString& name);



    Q_INVOKABLE int getIndexFromNameForArtist(const QString& name);
    Q_INVOKABLE int getIndexFromNameForAlbum(const QString& name);
    Q_INVOKABLE int getIndexFromNameForGenre(const QString& name);

signals:
    void artistsChanged();
    void albumsChanged();
    void genresChanged();
    void playlistsChanged();

private:
    QSqlDatabase db;
    QString connectionName;
    QStandardItemModel* artistsModel;
    QStandardItemModel* albumsModel;
    QStandardItemModel* genresModel;
    QStandardItemModel* playlistsModel;
};

#endif // DATABASEHANDLER_H
