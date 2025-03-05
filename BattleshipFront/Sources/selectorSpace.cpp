#include "../../Headers/selectorSpace.h"

SelectorSpace::SelectorSpace(QWidget *parent) : QWidget(parent), isHorizontal(true), isDragging(false) {
    setupShips();
}

void SelectorSpace::setupShips() {
    clearShips();

    struct ShipInfo {
        QString texturePath;
        QSize size;
        int shipSize;//tamanho do barco que vai ser passado para o back
    };

    QVector<ShipInfo> ships = {
        {"../../Textures/carrierH.png", QSize(192, 32), 6},   // Porta-aviões (6 casas)
        {"../../Textures/battleshipH.png", QSize(128, 32), 4}, // Navio de guerra (4 casas)
        {"../../Textures/cruiserH.png", QSize(96, 32), 3},    // Encourçado (3 casas)
        {"../../Textures/cruiserH.png", QSize(96, 32), 3},    // Segundo encourçado (3 casas)
        {"../../Textures/subH.png", QSize(32, 32), 1}   // Submarino (1 casa)
    };

    int offsetX = 5;
    int offsetY = 5;
    int spacing = 10;

    for (const ShipInfo &ship : ships) {
        QLabel *shipLabel = new QLabel(this);
        shipLabel->setPixmap(QPixmap(ship.texturePath));
        shipLabel->setFixedSize(ship.size);
        shipLabel->setAttribute(Qt::WA_DeleteOnClose);
        shipLabel->setStyleSheet("background-color: transparent");
        shipLabel->move(offsetX, offsetY); // Ajuste a posição inicial

        shipLabel->setProperty("shipSize", ship.shipSize);

        shipLabel->installEventFilter(this); // Captura eventos de mouse

        offsetY += ship.size.height() + spacing;
    }
}

bool SelectorSpace::eventFilter(QObject *obj, QEvent *event) {
    QLabel *label = qobject_cast<QLabel*>(obj);
    if (label) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

            if (mouseEvent->button() == Qt::LeftButton) {
                //pega o tamanho do barco a partir do QLabel
                int shipSize = label->property("shipSize").toInt();
                qDebug() << "Ship selected with size:" << shipSize;

                selectedShipSize = shipSize; //quando precisar posicionar o barco, basta passar shipSize para BoardController

            } else if (mouseEvent->button() == Qt::RightButton) {
                rotateShip(label);
            }

            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

int SelectorSpace::getSelectedShipSize() {
    return selectedShipSize;

}

void SelectorSpace::rotateShip(QLabel *shipLabel) {
    bool isVertical = shipRotation.value(shipLabel, false);

    QPixmap originalPixmap(shipLabel->pixmap(Qt::ReturnByValue));
    QPixmap newPixmap;

    if (!isVertical) {
        QTransform transform;
        transform.rotate(90);
        newPixmap = originalPixmap.transformed(transform, Qt::SmoothTransformation);
    } else {
        newPixmap = originalPixmap;
    }

    shipLabel->setPixmap(newPixmap);
    shipLabel->setFixedSize(newPixmap.size());

    shipRotation[shipLabel] = !isVertical;
    shipLabel->update();
}


void SelectorSpace::clearShips() {
    // Remove todos os navios do espaço de seleção
    QList<QLabel*> shipLabels = findChildren<QLabel*>();
    for (QLabel* label : shipLabels) {
        label->deleteLater();
    }
    shipRotation.clear(); // Limpa o mapa de rotações
}

SelectorSpace::~SelectorSpace() {
    qDebug() << "Destruindo SelectorSpace!";
}
