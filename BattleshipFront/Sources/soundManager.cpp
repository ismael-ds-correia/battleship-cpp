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
    qDeleteAll(attackSounds);
}

void SoundManager::loadSounds() {
    QString basePath = "../../Sounds/";

    //carrega as 4 variantes de som de ataque
    for (int i = 1; i <= 4; ++i) {
        QString path = basePath + QString("attack%1.wav").arg(i);
        QSoundEffect *effect = new QSoundEffect(this); // Passa o 'this' para gerenciamento de memória
        effect->setSource(QUrl::fromLocalFile(path));
        effect->setVolume(0.5f);

        attackSounds.append(effect);
    }
}

void SoundManager::loadBackgroundMusic() {
    // Cria o QMediaPlayer
    backgroundMusic = new QMediaPlayer(this);

    // Cria o QAudioOutput e associa ao QMediaPlayer
    audioOutput = new QAudioOutput(this);
    backgroundMusic->setAudioOutput(audioOutput);

    // Define o caminho para o arquivo MP3 (verifique se o caminho está correto ou utilize um recurso via .qrc)
    QString musicPath = "../../Sounds/background.mp3";
    backgroundMusic->setSource(QUrl::fromLocalFile(musicPath));

    // Ajusta o volume (o QAudioOutput aceita valores entre 0.0 e 1.0)
    audioOutput->setVolume(0.5);

    // Conecta o sinal para detectar o fim da mídia e reiniciar a reprodução
    connect(backgroundMusic, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::MediaStatus::EndOfMedia) {
            backgroundMusic->setPosition(0);
            backgroundMusic->play();
        }
    });
}

void SoundManager::playAttackSound() {
    if (!attackSounds.isEmpty()) {
        //seleciona um índice aleatório dentre os sons carregados
        int index = QRandomGenerator::global()->bounded(attackSounds.size());
        QSoundEffect *selectedSound = attackSounds[index];

        if (selectedSound->isLoaded()) {
            selectedSound->play();
        } else {
            qDebug() << "Som não carregado, não foi possível reproduzir:" << selectedSound->source();
        }
    } else {
        qDebug() << "Nenhum som de ataque foi carregado.";
    }
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
