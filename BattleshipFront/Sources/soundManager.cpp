#include "../Headers/soundManager.h"

#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QRandomGenerator>

SoundManager::SoundManager(QObject* parent)
    : QObject(parent),
    attackSound(new QSoundEffect(this)) {

    loadSounds();
}

SoundManager::~SoundManager() {
    qDeleteAll(attackSounds);
}

void SoundManager::loadSounds() {
    // Define o caminho base onde os sons estão localizados.
    QString basePath = "../../Sounds/";

    // Carrega as 4 variantes de som de ataque
    for (int i = 1; i <= 4; ++i) {
        QString path = basePath + QString("attack%1.wav").arg(i);
        QSoundEffect *effect = new QSoundEffect(this); // Passa o 'this' para gerenciamento de memória
        effect->setSource(QUrl::fromLocalFile(path));
        effect->setVolume(0.5f);

        // Conecta o sinal de carregamento para verificar se o som foi carregado com sucesso
        connect(effect, &QSoundEffect::loadedChanged, [effect]() {
            if (effect->isLoaded())
                qDebug() << "Som carregado:" << effect->source();
            else
                qDebug() << "Falha ao carregar som:" << effect->source();
        });

        attackSounds.append(effect);
    }
}

void SoundManager::playAttackSound() {
    if (!attackSounds.isEmpty()) {
        // Seleciona um índice aleatório dentre os sons carregados
        int index = QRandomGenerator::global()->bounded(attackSounds.size());
        QSoundEffect *selectedSound = attackSounds[index];

        // Verifica se o som já está carregado antes de tocar
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
    if (backgroundMusic)
        backgroundMusic->play();
}

void SoundManager::stopBackgroundMusic() {
    if (backgroundMusic)
        backgroundMusic->stop();
}
