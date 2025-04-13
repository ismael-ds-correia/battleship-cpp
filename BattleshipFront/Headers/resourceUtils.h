#ifndef RESOURCEUTILS_H
#define RESOURCEUTILS_H

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QDebug>

// Função que resolve o caminho do recurso baseado no local do executável.
// relativePath deve ser, por exemplo, "Textures" ou "Sounds/attack1.wav", etc.
inline QString resolveResourcePath(const QString &relativePath) {
    QString exeDir = QCoreApplication::applicationDirPath();

    // Caminho para o deploy (ao lado do .exe)
    QString deployPath = QDir::cleanPath(exeDir + "/" + relativePath);

    // Caminho para o desenvolvimento (supondo que os recursos estão três níveis acima da pasta do executável)
    QString devPath = QDir::cleanPath(exeDir + "/../../../" + relativePath);

    if (QFile::exists(deployPath)) {
        qDebug() << "[Path Resolver] Usando deployPath:" << deployPath;
        return deployPath;
    } else if (QFile::exists(devPath)) {
        qDebug() << "[Path Resolver] Usando devPath:" << devPath;
        return devPath;
    } else {
        qDebug() << "[Path Resolver] Arquivo não encontrado:" << deployPath << "nem" << devPath;
        return "";
    }
}

#endif // RESOURCEUTILS_H
