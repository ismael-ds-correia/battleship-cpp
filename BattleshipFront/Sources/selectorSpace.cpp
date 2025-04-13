#include "../../Headers/selectorSpace.h"

SelectorSpace::SelectorSpace(QWidget *parent)
    : QWidget(parent), Horizontal(true), isDragging(false) {

    QString texturesPath = resolveResourcePath("Textures");

    shipInfos = {
        {texturesPath + "/carrierH.png", QSize(192, 32), 6},       // porta-aviões (6 casas)
        {texturesPath + "/battleshipH.png", QSize(128, 32), 4},      // navio de guerra (4 casas)
        {texturesPath + "/cruiserH.png", QSize(96, 32), 3},           // encourçado (3 casas)
        {texturesPath + "/cruiserH.png", QSize(96, 32), 3},           // encourçado (3 casas)
        {texturesPath + "/subH.png", QSize(32, 32), 1}                // submarino (1 casa)
    };
    setupShips();
}

void SelectorSpace::setupShips() {
    //clearShips();

    int offsetX = 5;
    int offsetY = 5;
    int spacing = 10;
    int index = 0;

    for (const ShipInfo &ship : shipInfos) {
        QLabel *shipLabel = new QLabel(this);
        shipLabel->setPixmap(QPixmap(ship.texturePath));
        shipLabel->setFixedSize(ship.size);
        shipLabel->setAttribute(Qt::WA_DeleteOnClose);
        shipLabel->setStyleSheet("background-color: transparent");
        shipLabel->move(offsetX, offsetY);//ajusta a posição inicial

        shipLabel->setProperty("shipSize", ship.shipSize);
        shipLabel->setProperty("shipIndex", index);

        shipLabel->installEventFilter(this);//captura eventos de mouse

        offsetY += ship.size.height() + spacing;
        index++;
    }
}

void SelectorSpace::restoreShip(Ship &ship) {
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
                QLabel* shipLabel = new QLabel(this);
                shipLabel->setPixmap(QPixmap(info.texturePath));
                shipLabel->setFixedSize(info.size);
                shipLabel->setAttribute(Qt::WA_DeleteOnClose);
                shipLabel->setStyleSheet("background-color: transparent");

                //define a posição padrão para exibição. O tamanho do barco é usado para calcular um offset
                int offsetX = 5;
                int spacing = 10;
                int offsetY = 5 + i * (info.size.height() + spacing);
                shipLabel->move(offsetX, offsetY);

                shipLabel->setProperty("shipSize", info.shipSize);
                shipLabel->setProperty("shipIndex", i);
                shipLabel->installEventFilter(this);
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
    // Se ainda não armazenamos o pixmap original, faz isso agora
    if (!originalPixmaps.contains(shipLabel)) {
        originalPixmaps[shipLabel] = shipLabel->pixmap();
    }

    // Obtém o estado atual de rotação para esse QLabel (false = horizontal, true = vertical)
    bool isVertical = shipRotation.value(shipLabel, false);

    // Sempre usa o pixmap original como base para a transformação
    QPixmap originalPixmap = originalPixmaps.value(shipLabel);
    QPixmap newPixmap;

    if (!isVertical) {
        // Rotaciona 90 graus para vertical
        QTransform transform;
        transform.rotate(90);
        newPixmap = originalPixmap.transformed(transform, Qt::SmoothTransformation);
    } else {
        // Retorna para o estado original horizontal
        newPixmap = originalPixmap;
    }

    shipLabel->setPixmap(newPixmap);
    shipLabel->setFixedSize(newPixmap.size());

    // Alterna o estado de rotação para este QLabel
    shipRotation[shipLabel] = !isVertical;
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
