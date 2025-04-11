#include "../Headers/soundManager.h"

#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QFile>

SoundManager::SoundManager(QObject* parent)
    : QObject(parent),
    attackSound(new QSoundEffect(this)),
    explosionSound(new QSoundEffect(this)) {

    connect(attackSound, &QSoundEffect::statusChanged, this, [this]() {
        qDebug() << "[attackSound] Status:" << attackSound->status();
        //qDebug() << "[attackSound] Erro:" << attackSound->error();
    });


    loadSounds();
}

SoundManager::~SoundManager() {
    // Os ponteiros já são gerenciados pelo QObject, não é necessário deletá-los manualmente.
}

void SoundManager::loadSounds() {
    QString appDir = QCoreApplication::applicationDirPath();
    QString currentDir = QDir::currentPath();
    qDebug() << "applicationDirPath:" << appDir;
    qDebug() << "currentPath:" << currentDir;

    // Constrói o caminho absoluto
    QString attackPath = "../../Sounds/attack1.wav";
    QString explosionPath = "../../Sounds/attack2.wav";

    qDebug() << "Caminho do ataque:" << attackPath;
    qDebug() << "Arquivo de ataque existe?" << QFile::exists(attackPath);
    qDebug() << "Caminho da explosão:" << explosionPath;
    qDebug() << "Arquivo de explosão existe?" << QFile::exists(explosionPath);

    attackSound->setSource(QUrl::fromLocalFile(attackPath));
    attackSound->setVolume(0.5f);

    explosionSound->setSource(QUrl::fromLocalFile(explosionPath));
    explosionSound->setVolume(0.5f);
}

void SoundManager::playAttackSound() {
    if (attackSound)
        attackSound->play();
}

void SoundManager::playExplosionSound() {
    if (explosionSound)
        explosionSound->play();
}

void SoundManager::playBackgroundMusic() {
    if (backgroundMusic)
        backgroundMusic->play();
}

void SoundManager::stopBackgroundMusic() {
    if (backgroundMusic)
        backgroundMusic->stop();
}
