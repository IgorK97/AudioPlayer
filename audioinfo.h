#ifndef AUDIOINFO_H
#define AUDIOINFO_H

#include <QObject>
#include <QUrl>
#include <qqml.h>

class AudioInfo : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged REQUIRED)
    Q_PROPERTY(int modelIndex READ modelIndex WRITE setModelIndex NOTIFY modelIndexChanged REQUIRED)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString artist READ artist WRITE setArtist NOTIFY artistChanged)
    Q_PROPERTY(QString album READ album WRITE setAlbum NOTIFY albumChanged)
    Q_PROPERTY(QString genre READ genre WRITE setGenre NOTIFY genreChanged)
    Q_PROPERTY(QString duration READ duration WRITE setDuration NOTIFY durationChanged)
    Q_PROPERTY(QString songText READ songText WRITE setSongText NOTIFY songTextChanged)

    Q_PROPERTY(QUrl filePath READ filePath WRITE setFilePath NOTIFY filePathChanged REQUIRED)
    Q_PROPERTY(bool inFavorite READ inFavorite WRITE setInFavorite NOTIFY inFavoriteChanged)

public:
    explicit AudioInfo(QObject *parent = nullptr);

    int index() const;
    void setIndex(int newIndex);

    int modelIndex() const;
    void setModelIndex(int newModelIndex);

    bool inFavorite() const;
    void setInFavorite(bool value);

    QString title() const;
    void setTitle(const QString &newTitle);

    QString artist() const;
    void setArtist(const QString &newArtist);

    QString album() const;
    void setAlbum(const QString &newAlbum);

    QString genre() const;
    void setGenre(const QString &newGenre);

    QString duration() const;
    void setDuration(const QString &newDuration);

    QString songText() const;
    void setSongText(const QString &newSongText);


    QUrl filePath() const;
    void setFilePath(const QUrl &newFilePath);

signals:
    void indexChanged();
    void modelIndexChanged();
    void titleChanged();
    void artistChanged();
    void albumChanged();
    void genreChanged();
    void durationChanged();
    void songTextChanged();

    void filePathChanged();
    void inFavoriteChanged();

private:
    int m_index;
    int m_modelIndex;
    QString m_title;
    QString m_artist;

    QString m_album;
    QString m_genre;
    QString m_songText;
    QString m_duration;
    QUrl m_filePath;
    bool m_inFavorite;
};

#endif // AUDIOINFO_H
