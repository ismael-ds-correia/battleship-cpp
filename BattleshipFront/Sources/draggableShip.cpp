#include "../Headers/draggableShip.h"

DraggableShip::DraggableShip(const QPixmap &texture, int size, QGraphicsItem *parent)
    : QGraphicsPixmapItem(texture, parent), shipSize(size) {
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);//essa opção é meramente visual
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

//adapta a função default de clicar do Qt para a Scene
void DraggableShip::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {

        clickOffset = mapToScene(event->pos()) - pos();//ajuste para que a textura fique exatamente em cima do mouse
        setZValue(1);
    }
    else if (event->button() == Qt::RightButton) {
        QPointF oldCenter = mapToScene(boundingRect().center());

        rotateShip();

        QPointF newCenter = mapToScene(boundingRect().center());

        setPos(pos() + (oldCenter - newCenter));
    }
    QGraphicsPixmapItem::mousePressEvent(event);
}

//adapta a função default de segurar e mover do Qt para o Scene
void DraggableShip::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        setPos(event->scenePos() - clickOffset);
    }
    QGraphicsPixmapItem::mouseMoveEvent(event);
}

//adapta a função default de soltar um objeto sendo movido do Qt
void DraggableShip::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    //⚠️aqui tem que ter mais logicas para lidar com posições invalidas⚠️
    setZValue(0);

    if (pos().x() < 0 || pos().y() < 0) {
        setPos(initialPosition);
    }

    QGraphicsPixmapItem::mouseReleaseEvent(event);
}

//rotação usando funções do Qt
void DraggableShip::rotateShip() {
    if (horizontal) {
        setTransform(QTransform().rotate(90));
    } else {
        setTransform(QTransform().rotate(0));
    }
    horizontal = !horizontal;
}

bool DraggableShip::isHorizontal() const {
    return horizontal;
}

void DraggableShip::setShipSize(int size) {
    shipSize = size;
}

int DraggableShip::getShipSize() const {
    return shipSize;
}
