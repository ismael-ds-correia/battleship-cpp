#include "../../Headers/selectorSpace.h"

SelectorSpace::SelectorSpace(QWidget *parent) : QWidget(parent), Horizontal(true), isDragging(false) {
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
    int index = 0;

    for (const ShipInfo &ship : ships) {
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

bool SelectorSpace::eventFilter(QObject *obj, QEvent *event) {
    QLabel *label = qobject_cast<QLabel*>(obj);
    if (label) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

            if (mouseEvent->button() == Qt::LeftButton) {
                //pega o tamanho do barco a partir do QLabel
                int shipSize = label->property("shipSize").toInt();
                int shipIndex = label->property("shipIndex").toInt();
                qDebug() << "Ship selected with size:" << shipSize;

                selectedShipIndex = shipIndex;
                selectedShipSize = shipSize; //quando precisar posicionar o barco, basta passar shipSize para BoardController

            } else if (mouseEvent->button() == Qt::RightButton) {
                rotateShip(label);
            }

            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void SelectorSpace::markShipAsPlaced(int shipIndex) {
    //procura todos os QLabel filhos deste widget
    QList<QLabel*> shipLabels = findChildren<QLabel*>();

    for (QLabel* label : shipLabels) {
        if (label->property("shipIndex").toInt() == shipIndex) {
            //remove o label do barco utilizado do SelectorSpace
            label->hide();
            label->deleteLater();
            break;
        }
    }

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
