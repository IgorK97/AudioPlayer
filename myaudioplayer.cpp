#include "myaudioplayer.h"
#include <QMediaDevices>
#include <QAudioDevice>
#include <QAudioOutput>
#include <QMediapLayer>
#include <QDebug>

MyAudioPlayer::MyAudioPlayer(QObject *parent):QObject(parent) {
    const auto &audioOutputs = QMediaDevices::audioOutputs();
    if (!audioOutputs.isEmpty()) {
        m_mediaPlayer.setAudioOutput(new QAudioOutput(&m_mediaPlayer));
    }

    connect(&m_mediaPlayer, &QMediaPlayer::positionChanged, this, &MyAudioPlayer::updatePosition);
    connect(&m_mediaPlayer, &QMediaPlayer::durationChanged, this, &MyAudioPlayer::updateDuration);
    connect(&m_mediaPlayer, &QMediaPlayer::seekableChanged, this, &MyAudioPlayer::updateSeekable);
    connect(&m_mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &MyAudioPlayer::handleMediaStatusChanged);
}

bool MyAudioPlayer::seekable()const {
    return m_mediaPlayer.isSeekable();
}

qreal MyAudioPlayer::volume()const{
    return m_mediaPlayer.audioOutput()->volume();
}
bool MyAudioPlayer::muted()const{
    return m_mediaPlayer.audioOutput()->isMuted();
}
qint64 MyAudioPlayer::position()const{
    return m_position;
}
qint64 MyAudioPlayer::duration()const{
    return m_duration;
}
void MyAudioPlayer::setDuration(qint64 dur){
    if(m_duration==dur)
        return;
    m_duration=dur;
    emit durationChanged();
}
void MyAudioPlayer::setVolume(qreal volume){
    if(m_mediaPlayer.audioOutput()->volume()!=volume){
        m_mediaPlayer.audioOutput()->setVolume(volume);
        emit volumeChanged();
    }
}
void MyAudioPlayer::setMuted(bool muted) {
    if (m_mediaPlayer.audioOutput()->isMuted() != muted) {
        m_mediaPlayer.audioOutput()->setMuted(muted);
        emit mutedChanged();
    }
}

void MyAudioPlayer::handleMediaStatusChanged(QMediaPlayer::MediaStatus status){
    if(status==QMediaPlayer::EndOfMedia){
        if(m_mediaPlayer.loops()==QMediaPlayer::Once){
            emit itsTimeToSwitch();
        }
        // else if(m_mediaPlayer.loops() != QMediaPlayer::Infinite){
        //     setPlaying(false);
        // }
    }
}

void MyAudioPlayer::updatePosition(qint64 pos) {
    if(m_position != pos) {
        m_position = pos;
        emit positionChanged();
    }
}

void MyAudioPlayer::updateDuration(qint64 dur) {
    if(m_duration != dur) {
        m_duration = dur;
        emit durationChanged();
    }
}

void MyAudioPlayer::updateSeekable(bool s){
    emit seekableChanged();
}

void MyAudioPlayer::setPosition(qint64 position){
    m_mediaPlayer.setPosition(position);
}

bool MyAudioPlayer::playing() const
{
    return m_playing;
}

void MyAudioPlayer::setPlaying(bool newPlaying){
    if(m_playing==newPlaying)
        return;
    m_playing=newPlaying;
    if(m_playing==false)
        m_mediaPlayer.stop();
    emit playingChanged();
}

int MyAudioPlayer::loops()const{
    return m_mediaPlayer.loops();
}

void MyAudioPlayer::setLoops(int newLoops){
    if(m_mediaPlayer.loops()==newLoops)
        return;
    m_mediaPlayer.setLoops(newLoops);
    emit loopsChanged();
}

void MyAudioPlayer::stepForward(){
    qint64 pos = qMin(m_mediaPlayer.duration(),
                      m_mediaPlayer.position() + 10000);
    m_mediaPlayer.setPosition(pos);
}

void MyAudioPlayer::stepBack(){
    qint64 pos = qMax(0,
                      m_mediaPlayer.position() - 10000);
    m_mediaPlayer.setPosition(pos);
}


void MyAudioPlayer::playPause()
{
    // m_playing = !m_playing;
    // emit playingChanged();

    if (m_playing) {
        m_mediaPlayer.pause();
        m_playing=false;
        emit playingChanged();
    } else if(m_mediaPlayer.hasAudio()) {
        m_mediaPlayer.play();
        m_playing=true;
        emit playingChanged();
    }
}

void MyAudioPlayer::changeAudioSource(const QUrl &source)
{
    m_mediaPlayer.stop();
    m_mediaPlayer.setSource(source);

    setPlaying(true);
    // if (m_playing) {
    m_mediaPlayer.play();
    qDebug()<<"I am playing!";
    // }
}

