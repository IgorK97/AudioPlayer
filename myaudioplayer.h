#ifndef MYAUDIOPLAYER_H
#define MYAUDIOPLAYER_H
#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>

class MyAudioPlayer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool playing READ playing WRITE setPlaying NOTIFY playingChanged)
    Q_PROPERTY(int loops READ loops WRITE setLoops NOTIFY loopsChanged)
    Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(qint64 duration READ duration WRITE setDuration NOTIFY durationChanged)
    Q_PROPERTY(bool seekable READ seekable NOTIFY seekableChanged)
    Q_PROPERTY(qreal volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool muted READ muted WRITE setMuted NOTIFY mutedChanged)
public:
    MyAudioPlayer(QObject* parent=nullptr);
    bool playing() const;
    void setPlaying(bool newPlaying);
    qreal volume() const;
    bool muted() const;

    qint64 position() const;
    qint64 duration() const;
    void setDuration(qint64 dur);

    bool seekable()const;
    int loops() const;
    Q_INVOKABLE void setLoops(int newLoops);
    Q_INVOKABLE void setPosition(qint64 pos);
    Q_INVOKABLE void stepForward();
    Q_INVOKABLE void stepBack();
    Q_INVOKABLE void playPause();

    Q_INVOKABLE void setVolume(qreal volume);
    Q_INVOKABLE void setMuted(bool muted);
public slots:
    void changeAudioSource(const QUrl &source);
// private slots:

    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void updatePosition(qint64 pos);
    void updateDuration(qint64 dur);
    void updateSeekable(bool seekable);

signals:
    void playingChanged();
    void loopsChanged();
    void positionChanged();
    void durationChanged();
    void seekableChanged();
    void volumeChanged();
    void mutedChanged();
    void itsTimeToSwitch();
private:
     QMediaPlayer m_mediaPlayer;
     bool m_playing = false;
     qint64 m_position=0;
     qint64 m_duration=0;
};

#endif // MYAUDIOPLAYER_H
