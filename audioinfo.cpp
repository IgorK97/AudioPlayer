#include "AudioInfo.h"

AudioInfo::AudioInfo(QObject *parent)
    : QObject{parent}
{

}

int AudioInfo::index() const
{
    return m_index;
}

void AudioInfo::setIndex(int newIndex)
{
    if (m_index == newIndex)
        return;
    m_index = newIndex;
    emit indexChanged();
}

int AudioInfo::modelIndex()const{
    return m_modelIndex;
}

void AudioInfo::setModelIndex(int newModelIndex){
    if(m_modelIndex==newModelIndex)
        return;
    m_modelIndex=newModelIndex;
    emit modelIndexChanged();
}

bool AudioInfo::inFavorite() const {
    return m_inFavorite;
}

void AudioInfo::setInFavorite(bool value){
    if(m_inFavorite==value)
        return;
    m_inFavorite=value;
    emit inFavoriteChanged();
}

QString AudioInfo::title() const
{
    return m_title;
}

void AudioInfo::setTitle(const QString &newTitle)
{
    if (m_title == newTitle)
        return;
    m_title = newTitle;
    emit titleChanged();
}

QString AudioInfo::artist() const
{
    return m_artist;
}

void AudioInfo::setArtist(const QString &newArtist)
{
    if (m_artist == newArtist)
        return;
    m_artist = newArtist;
    emit artistChanged();
}



QUrl AudioInfo::filePath() const
{
    return m_filePath;
}

void AudioInfo::setFilePath(const QUrl &newFilePath)
{
    if (m_filePath == newFilePath)
        return;
    m_filePath = newFilePath;
    emit filePathChanged();
}

QString AudioInfo::genre() const
{
    return m_genre;
}

void AudioInfo::setGenre(const QString& newGenre){
    if(m_genre==newGenre)
        return;
    m_genre=newGenre;
    emit genreChanged();
}

QString AudioInfo::songText()const
{
    return m_songText;
}

void AudioInfo::setSongText(const QString& newSongText){
    if(m_songText==newSongText)
        return;
    m_songText=newSongText;
    emit songTextChanged();
}

QString AudioInfo::album()const
{
    return m_album;
}

void AudioInfo::setAlbum(const QString& newAlbum){
    if(m_album==newAlbum)
        return;
    m_album=newAlbum;
    emit albumChanged();
}

QString AudioInfo::duration()const
{
    return m_duration;
}

void AudioInfo::setDuration(const QString& newDuration){
    if(m_duration==newDuration)
        return;
    m_duration=newDuration;
    emit durationChanged();
}
