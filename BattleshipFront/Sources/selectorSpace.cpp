#include "../../Headers/selectorSpace.h"

SelectorSpace::SelectorSpace(QWidget *parent) : QWidget(parent), Horizontal(true), isDragging(false) {
    shipInfos = {
        {"../../Textures/carrierH.png", QSize(192, 32), 6},//porta-aviões (6 casas)
        {"../../Textures/battleshipH.png", QSize(128, 32), 4},//navio de guerra (4 casas)
        {"../../Textures/cruiserH.png", QSize(96, 32), 3},//encourçado (3 casas)
        {"../../Textures/cruiserH.png", QSize(96, 32), 3},//encourçado (3 casas)
        {"../../Textures/subH.png", QSize(32, 32), 1}//submarino (1 casa)
    };
    setupShips();
}

void SelectorSpace::setupShips() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);
    layout->setSpacing(10);
    this->setLayout(layout);

    int offsetX = 5;
    int offsetY = 5;
    int spacing = 10;
    int index = 0;
    float scaleFactor = 1.5;


    for (const ShipInfo &ship : shipInfos) {
        QPixmap originalPixmap(ship.texturePath);
        QSize newSize = originalPixmap.size() * scaleFactor;

        // Redimensionar o pixmap mantendo a proporção
        QPixmap scaledPixmap = originalPixmap.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        QLabel *shipLabel = new QLabel(this);
        shipLabel->setPixmap(scaledPixmap);
        shipLabel->setFixedSize(newSize);
        shipLabel->setAttribute(Qt::WA_DeleteOnClose);
        shipLabel->setStyleSheet("background-color: transparent");
        shipLabel->move(offsetX, offsetY);
        shipLabel->setScaledContents(true);

        shipLabel->setProperty("shipSize", ship.shipSize);
        shipLabel->setProperty("shipIndex", index);

        shipLabel->installEventFilter(this);//captura eventos de mouse

        offsetY += newSize.height() + spacing;
        index++;
        originalPixmaps[shipLabel] = originalPixmap;
    }
}

void SelectorSpace::restoreShip(Ship &ship) {
    float scaleFactor = 1.5;
    bool restored = false; //usado meramente para depuração
    // Procura uma entrada em shipInfos que corresponda ao tamanho do navio removido.
    // Se houver mais de uma com o mesmo tamanho, restaura a primeira que não estiver presente.
    for (int i = 0; i < shipInfos.size(); i++) {
        if (shipInfos[i].shipSize == ship.getSize()) {
            // Verifica se já existe um QLabel para esse navio (identificado pelo índice 'i')
            bool alreadyExists = false;
            QList<QLabel*> labels = findChildren<QLabel*>();
            for (QLabel* label : labels) {
                if (label->property("shipIndex").toInt() == i) {
                    alreadyExists = true;
                    break;
                }
            }
            if (!alreadyExists) {
                // Cria o QLabel para o navio removido com base nos dados de shipInfos[i]
                ShipInfo info = shipInfos[i];

                // Usar o mesmo padrão de escala que setupShips
                QPixmap originalPixmap(info.texturePath);
                QSize newSize = originalPixmap.size() * scaleFactor;
                QPixmap scaledPixmap = originalPixmap.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

                QLabel* shipLabel = new QLabel(this);
                shipLabel->setPixmap(scaledPixmap);
                shipLabel->setFixedSize(newSize);
                shipLabel->setAttribute(Qt::WA_DeleteOnClose);
                shipLabel->setStyleSheet("background-color: transparent");
                shipLabel->setScaledContents(true);

                // Posição usando os mesmos cálculos de setupShips
                int offsetX = 5;
                int spacing = 10;
                int offsetY = 5 + i * (newSize.height() + spacing);
                shipLabel->move(offsetX, offsetY);

                shipLabel->setProperty("shipSize", info.shipSize);
                shipLabel->setProperty("shipIndex", i);
                shipLabel->installEventFilter(this);

                // Armazenar o pixmap original
                originalPixmaps[shipLabel] = originalPixmap;

                // Garantir que a rotação começa como horizontal
                shipRotation[shipLabel] = false;

                shipLabel->show();

                qDebug() << "Navio restaurado para shipInfos[" << i << "]";
                restored = true;
                break; // Restaura apenas uma entrada para o navio removido
            }
        }
    }
    if (!restored) {
        qDebug() << "Nenhum navio correspondente encontrado para restaurar";
    }
}

bool SelectorSpace::eventFilter(QObject *obj, QEvent *event) {
    QLabel *label = qobject_cast<QLabel*>(obj);
    if (!label)
        return QWidget::eventFilter(obj, event);

    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

        // Se for clique do botão esquerdo
        if (mouseEvent->button() == Qt::LeftButton) {

            // Se o navio clicado já está selecionado, limpar a seleção
            if (selectedShipLabel == label) {
                clearSelectedShip();
                emit shipSelected(); // Opcional: emitir o sinal para notificar que a seleção mudou (ficou vazia)
                return true;
            }

            // Caso contrário, se houver uma seleção anterior, limpar os efeitos atuais.
            if (currentEffect) {
                currentEffect->deleteLater();
                currentEffect = nullptr;
            }
            if (blurAnimation) {
                blurAnimation->stop();
                blurAnimation->deleteLater();
                blurAnimation = nullptr;
            }
            if (colorAnimation) {
                colorAnimation->stop();
                colorAnimation->deleteLater();
                colorAnimation = nullptr;
            }

            // Configura o efeito de destaque para o navio selecionado
            currentEffect = new QGraphicsDropShadowEffect(label);
            currentEffect->setColor(QColor(255, 255, 255, 255)); // Branco
            currentEffect->setBlurRadius(10);
            currentEffect->setOffset(0);
            label->setGraphicsEffect(currentEffect);

            // Animação do blur
            blurAnimation = new QPropertyAnimation(currentEffect, "blurRadius", this);
            blurAnimation->setDuration(1000);
            blurAnimation->setStartValue(10);
            blurAnimation->setEndValue(20);
            blurAnimation->setLoopCount(-1); // Infinito
            blurAnimation->start();

            // Animação da cor (opacidade)
            colorAnimation = new QPropertyAnimation(currentEffect, "color", this);
            colorAnimation->setDuration(1000);
            colorAnimation->setStartValue(QColor(255, 255, 255, 255));
            colorAnimation->setEndValue(QColor(255, 255, 255, 127));
            colorAnimation->setLoopCount(-1);
            colorAnimation->start();

            // Pega o tamanho do barco a partir das propriedades do QLabel
            int shipSize = label->property("shipSize").toInt();
            int shipIndex = label->property("shipIndex").toInt();

            qDebug() << "Ship selected with size:" << shipSize;
            selectedShipIndex = shipIndex;
            selectedShipSize = shipSize;
            selectedShipLabel = label;

            // Emite o sinal para notificar que um navio foi selecionado
            emit shipSelected();

        } else if (mouseEvent->button() == Qt::RightButton) {
            rotateShip(label);
        }
        return true;
    }
    return QWidget::eventFilter(obj, event);
}


void SelectorSpace::onPlacementFailed() {
    if (currentEffect) {
        // Pausa animações atuais
        blurAnimation->pause();
        colorAnimation->pause();

        // Animação para vermelho
        QPropertyAnimation* redAnim = new QPropertyAnimation(currentEffect, "color", this);
        redAnim->setDuration(500);
        redAnim->setStartValue(QColor(255, 0, 0, 255));
        redAnim->setEndValue(QColor(255, 0, 0, 127));
        redAnim->setLoopCount(2); // Pisca duas vezes

        connect(redAnim, &QPropertyAnimation::finished, this, [this]() {
            // Restaura a cor branca e retoma as animações
            currentEffect->setColor(QColor(255, 255, 255, 255));
            blurAnimation->resume();
            colorAnimation->resume();
            //redAnim->deleteLater();
        });

        redAnim->start();
    }
}


void SelectorSpace::markShipAsPlaced(int shipIndex) {
    QList<QLabel*> shipLabels = findChildren<QLabel*>();
    for (QLabel* label : shipLabels) {
        if (label->property("shipIndex").toInt() == shipIndex) {
            label->hide();
            label->deleteLater();
            // Se o barco posicionado for o mesmo que está selecionado, reseta a seleção
            if (label == selectedShipLabel) {
                clearSelectedShip();
            }
            break;
        }
    }
}

void SelectorSpace::rotateShip(QLabel *shipLabel) {
    if (!originalPixmaps.contains(shipLabel)) {
        // Armazenar o pixmap original em tamanho real (não escalonado)
        originalPixmaps[shipLabel] = QPixmap(shipInfos[shipLabel->property("shipIndex").toInt()].texturePath);
    }

    float scaleFactor = 1.5; // Mesmo fator usado em outros lugares
    bool isVertical = !shipRotation.value(shipLabel, false); // Inverte o estado atual

    // Pega a imagem original (sem escala)
    QPixmap originalPixmap = originalPixmaps.value(shipLabel);
    QTransform transform;

    if (isVertical) {
        // Rotaciona a imagem original
        transform.rotate(90);
        QPixmap rotatedPixmap = originalPixmap.transformed(transform, Qt::SmoothTransformation);

        // Escala para o tamanho desejado (invertendo largura/altura)
        QSize rotatedSize(originalPixmap.height() * scaleFactor, originalPixmap.width() * scaleFactor);
        QPixmap finalPixmap = rotatedPixmap.scaled(rotatedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        shipLabel->setPixmap(finalPixmap);
        shipLabel->setFixedSize(rotatedSize);
    } else {
        // Usa a imagem original horizontal e escala
        QSize originalSize(originalPixmap.width() * scaleFactor, originalPixmap.height() * scaleFactor);
        QPixmap scaledPixmap = originalPixmap.scaled(originalSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        shipLabel->setPixmap(scaledPixmap);
        shipLabel->setFixedSize(originalSize);
    }

    // Define o novo estado de rotação
    shipRotation[shipLabel] = isVertical;
    shipLabel->update();
}

void SelectorSpace::clearShips() {
    // Remove todos os navios do espaço de seleção
    QList<QLabel*> shipLabels = findChildren<QLabel*>();
    for (QLabel* label : shipLabels) {
        label->deleteLater();
    }
    shipRotation.clear(); //limpa o mapa de rotações
    originalPixmaps.clear();
}

bool SelectorSpace::isSelectedShipHorizontal() const {
    if (selectedShipLabel) {
        return !shipRotation.value(selectedShipLabel, false); //false = horizontal, true = vertical
    }
    return true;
}

void SelectorSpace::clearSelectedShip() {
    selectedShipIndex = -1;
    selectedShipSize = 0;
    selectedShipLabel = nullptr;

    if (currentEffect) {
        currentEffect->deleteLater();
        currentEffect = nullptr;
    }
    if (blurAnimation) {
        blurAnimation->stop();
        blurAnimation->deleteLater();
        blurAnimation = nullptr;
    }
    if (colorAnimation) {
        colorAnimation->stop();
        colorAnimation->deleteLater();
        colorAnimation = nullptr;
    }

    emit shipSelected();
}

int SelectorSpace::getSelectedShipSize() {
    return selectedShipSize;
}

int SelectorSpace::getSelectedShipIndex() {
    return selectedShipIndex;

}

bool SelectorSpace::isHorizontal() {
    if (!Horizontal) {
        return false;
    }
    return true;
}


SelectorSpace::~SelectorSpace() {
    qDebug() << "Destruindo SelectorSpace!";
}
