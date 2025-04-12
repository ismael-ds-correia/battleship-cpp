#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>
#include <QSoundEffect>
#include <QMediaPlayer>

class SoundManager : public QObject {
    Q_OBJECT
public:
    explicit SoundManager(QObject* parent = nullptr);
    ~SoundManager();

    void loadSounds();

    void playAttackSound();
    void playExplosionSound();
    void playBackgroundMusic();
    void stopBackgroundMusic();

private:
    QSoundEffect* attackSound;
    QVector<QSoundEffect*> attackSounds;

    QMediaPlayer* backgroundMusic;
};

#endif // SOUNDMANAGER_H
