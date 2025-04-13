#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "resourceUtils.h"

class SoundManager : public QObject {
    Q_OBJECT
public:
    explicit SoundManager(QObject* parent = nullptr);
    ~SoundManager();

    void loadSounds();
    void loadBackgroundMusic();

    void playHitSound();
    void playWaterSplashSound();
    void playBackgroundMusic();
    void stopBackgroundMusic();

private:
    QVector<QSoundEffect*> hitSounds;
    QVector<QSoundEffect*> waterSplashSounds;

    QMediaPlayer* backgroundMusic = nullptr;
    QAudioOutput* audioOutput = nullptr;
};

#endif // SOUNDMANAGER_H
