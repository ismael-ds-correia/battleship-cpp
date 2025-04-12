#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QAudioOutput>

class SoundManager : public QObject {
    Q_OBJECT
public:
    explicit SoundManager(QObject* parent = nullptr);
    ~SoundManager();

    void loadSounds();
    void loadBackgroundMusic();

    void playAttackSound();
    void playExplosionSound();
    void playBackgroundMusic();
    void stopBackgroundMusic();

private:
    QVector<QSoundEffect*> attackSounds;

    QMediaPlayer* backgroundMusic = nullptr;
    QAudioOutput* audioOutput = nullptr;
};

#endif // SOUNDMANAGER_H
