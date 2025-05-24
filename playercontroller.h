#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QMediaPlayer>
#include <QAbstractListModel>
#include <QSqlDatabase>
#include <QAudioOutput>
#include <QStack>
#include "imagehandler.h"
#include "myaudioplayer.h"

Q_MOC_INCLUDE("AudioInfo.h")

class AudioInfo;

class PlayerController : public QAbstractListModel
{
    Q_OBJECT


    Q_PROPERTY(AudioInfo* currentTrack READ currentTrack WRITE setCurrentTrack NOTIFY currentTrackChanged)
    Q_PROPERTY(QString coverImage READ coverImage WRITE setCoverImage NOTIFY coverImageChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString artist READ artist WRITE setArtist NOTIFY artistChanged)
    Q_PROPERTY(QString album READ album WRITE setAlbum NOTIFY albumChanged)
    Q_PROPERTY(bool shuffle READ shuffle WRITE setShuffle NOTIFY shuffleChanged)
    Q_PROPERTY(QString currentPlaylist READ currentPlaylist WRITE setCurrentPlaylist NOTIFY currentPlaylistChanged)

public:
    enum Role {
        AudioTitleRole = Qt::UserRole + 1,
        AudioFilePathRole,
        AudioArtistRole,
        AudioAlbumRole,
        AudioGenreRole,
        AudioDurationRole,
        AudioSongTextRole,
        // AudioImageBase64Role,
        AudioIndexRole,
        AudioInFavoriteRole,
        AudioModelIndexRole
    };

    explicit PlayerController(const QString &connectionName, MyAudioPlayer* map, QObject *parent = nullptr);



    bool shuffle()const;
    Q_INVOKABLE void setShuffle(bool newShuffle);

    QString currentPlaylist()const;
    Q_INVOKABLE void setCurrentPlaylist(QString newPlaylist);


    QString title()const;
    void setTitle(QString newTitle);

    QString artist()const;
    void setArtist(QString newArtist);

    QString album()const;
    void setAlbum(QString newAlbum);


    Q_INVOKABLE void switchToNextTrack();


    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    AudioInfo *currentTrack() const;
    QString coverImage() const;
    void setCurrentTrack(AudioInfo *newCurrentTrack);
    void setCoverImage(QString newCoverImage);
    Q_INVOKABLE void setCurrentTrack(int posNumber);

    Q_INVOKABLE QVariantMap getTrack(int posNumber);
    Q_INVOKABLE void filterTracks(const QString& artist, const QString& album,
                                  const QString &genre);
    Q_INVOKABLE void loadFavorites();
    Q_INVOKABLE void loadTracks();
    Q_INVOKABLE void loadPlaylist(const QString& name);


    Q_INVOKABLE void addToPlaylist(int trackId, QString playlistName);
    Q_INVOKABLE void removeFromPlaylist(int trackId);

public slots:
    void switchToPreviousTrack();


    int addAudioToDb(const QString& title,
                      const QString &filePath,
                      const int&artistId,
                      const int &albumId,
                      const int &genreId,
                      const QString &duration,
                      const QString& songText);

    void openAudio(const QUrl& newFilePath);
    void updateAudio(const int& index,
                      const QString& title,
                      const QString &artist,
                      const QString &album,
                      const QString& genre);
    // bool checkAudio(const QUrl& filePath);
    void removeAudio(int index);
    void addToFavorites(int index);
    // void addToPlayList(int index, QString playList);
    // void removeFromPlayList(int index, QString playList);
    void removeFromFavorites(int index);
    void addToModel(const int& index,
                    const QString& title,
                    const QString &artist,
                    const QString &album,
                    const QString& genre,
                    const QString &songText,
                    const QString& duration,
                    const QUrl& filePath,
                    const bool& isFavorite,
                    const int& modelIndex);



signals:

    void currentTrackChanged();
    void coverImageChanged();

    void titleChanged();
    void artistChanged();
    void albumChanged();
    void shuffleChanged();
    void currentPlaylistChanged();
private:
    ImageHandler* imageHandler;
    QSqlDatabase db;
    bool isInDatabase(const QString& filePath, int& id);
    bool isFavorite(const int& id);
    QString formatDuration(const QString& durationStr);
    QString connectionName;
    QString m_title;
    QString m_artist;
    QString m_album;


    int getIdFromDb(const QString &table, const QString &column, const QString &value);
    int insertValueToDb(const QString &table, const QString& column, const QString &value);
    QList<AudioInfo*> m_audioList;
    AudioInfo *m_currentTrack = nullptr;
    QString m_coverImage="file:///C:/Users/IgorA/source/repos/MyProjects/QtProjects/MyAudioPlayer2/build/Desktop_Qt_6_10_0_MSVC2022_64bit-Debug/MyAudioPlayer2/assets/images/defaultImage.png";

    bool m_shuffle=0;
    QString m_currentPlaylist="";
    QStack<int> m_playHistory;
    QStack<int> m_playFuture;
    MyAudioPlayer* m_myAudioPlayer;
};

#endif // PLAYERCONTROLLER_H
