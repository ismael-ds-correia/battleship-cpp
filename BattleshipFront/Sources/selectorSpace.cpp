#include "../../Headers/selectorSpace.h"

SelectorSpace::SelectorSpace(QWidget *parent) : QWidget(parent), isHorizontal(true), isDragging(false) {
    setupShips();
}

void SelectorSpace::setupShips() {
    struct ShipInfo {
        QString texturePath;
        QSize size;
    };

    QVector<ShipInfo> ships = {
        {"../../Textures/carrierH.png", QSize(192, 32)},   // Porta-aviões (6 casas)
        {"../../Textures/battleshipH.png", QSize(128, 32)}, // Navio de guerra (4 casas)
        {"../../Textures/cruiserH.png", QSize(96, 32)},    // Encourçado (3 casas)
        {"../../Textures/cruiserH.png", QSize(96, 32)},    // Segundo encourçado (3 casas)
        {"../../Textures/subH.png", QSize(32, 32)}   // Submarino (1 casa)
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

        shipLabel->installEventFilter(this); // Captura eventos de mouse

        offsetY += ship.size.height() + spacing;
    }
}

//função que capta o input do mouse para arrastar e rotacionar
bool SelectorSpace::eventFilter(QObject *obj, QEvent *event) {
    QLabel *label = qobject_cast<QLabel*>(obj);
    if (label) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

            if (mouseEvent->button() == Qt::LeftButton) {
                // Iniciar o arrasto
                QDrag *drag = new QDrag(this);
                QMimeData *mimeData = new QMimeData;
                mimeData->setText("ship");
                drag->setMimeData(mimeData);

                QPixmap pixmap = label->pixmap(Qt::ReturnByValue);
                drag->setPixmap(pixmap);
                drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));

                drag->exec(Qt::MoveAction);
            }
            else if (mouseEvent->button() == Qt::RightButton) {
                rotateShip(label);
            }

            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
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

SelectorSpace::~SelectorSpace() {
}

    // QPixmap carrierTexture("../../Textures/battleshipH.png");
    // QPixmap battleshipTexture("../../Textures/battleshipH.png");
    // QPixmap cruiserTexture("../../Textures/cruiserH.png");
    // QPixmap submarineTexture("../../Textures/subH.png");

