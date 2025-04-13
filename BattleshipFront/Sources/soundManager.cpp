#include "../Headers/soundManager.h"

#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QRandomGenerator>

SoundManager::SoundManager(QObject* parent)
    : QObject(parent) {

    loadSounds();
    loadBackgroundMusic();
}

SoundManager::~SoundManager() {
    qDeleteAll(hitSounds);
}

void SoundManager::loadSounds() {
    QString basePath = "../../Sounds/";

    //carrega as 4 variantes de som de ataque
    for (int i = 1; i <= 4; ++i) {
        QString path = basePath + QString("attack%1.wav").arg(i);
        QSoundEffect *effect = new QSoundEffect(this);
        effect->setSource(QUrl::fromLocalFile(path));
        effect->setVolume(0.1f);

        hitSounds.append(effect);
    }

    for (int i = 1; i <= 5; ++i) {
        QString path = basePath + QString("waterSplash%1.wav").arg(i);
        QSoundEffect *effect = new QSoundEffect(this);
        effect->setSource(QUrl::fromLocalFile(path));
        effect->setVolume(1.0f);

        waterSplashSounds.append(effect);
    }
}

void SoundManager::playHitSound() {
    if (!hitSounds.isEmpty()) {
        //seleciona um índice aleatório dentre os sons carregados
        int index = QRandomGenerator::global()->bounded(hitSounds.size());
        QSoundEffect *selectedSound = hitSounds[index];

        if (selectedSound->isLoaded()) {
            selectedSound->play();
        } else {
            qDebug() << "Som não carregado, não foi possível reproduzir:" << selectedSound->source();
        }
    } else {
        qDebug() << "Nenhum som de ataque foi carregado.";
    }
}

void SoundManager::playWaterSplashSound() {
    if (!waterSplashSounds.isEmpty()) {
        // Seleciona um índice aleatório dentre os sons carregados
        int index = QRandomGenerator::global()->bounded(waterSplashSounds.size());
        QSoundEffect *selectedSound = waterSplashSounds[index];

        if (selectedSound->isLoaded()) {
            selectedSound->play();
        } else {
            qDebug() << "Som não carregado, não foi possível reproduzir:" << selectedSound->source();
        }
    } else {
        qDebug() << "Nenhum som de water splash foi carregado.";
    }
}

void SoundManager::loadBackgroundMusic() {
    backgroundMusic = new QMediaPlayer(this);

    audioOutput = new QAudioOutput(this);
    backgroundMusic->setAudioOutput(audioOutput);

    QString musicPath = "../../Sounds/background.mp3";
    backgroundMusic->setSource(QUrl::fromLocalFile(musicPath));

    audioOutput->setVolume(0.1);

    //conecta o sinal para detectar o fim da mídia e reiniciar a reprodução
    connect(backgroundMusic, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::MediaStatus::EndOfMedia) {
            backgroundMusic->setPosition(0);
            backgroundMusic->play();
        }
    });
}

void SoundManager::playBackgroundMusic() {
    if (backgroundMusic) {
        backgroundMusic->play();
    } else {
        qDebug() << "Música de fundo não carregada.";
    }
}

void SoundManager::stopBackgroundMusic() {
    if (backgroundMusic)
        backgroundMusic->stop();
}
