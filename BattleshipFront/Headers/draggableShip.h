#ifndef DRAGGABLESHIP_H
#define DRAGGABLESHIP_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

class DraggableShip : public QGraphicsPixmapItem {
public:
    DraggableShip(const QPixmap &texture, int size, QGraphicsItem *parent = nullptr);

    void setShipSize(int size);
    int getShipSize() const;

    void rotateShip();
    bool isHorizontal() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    bool horizontal;
    int shipSize;
    QPointF clickOffset;
    QPointF initialPosition;
};

#endif // DRAGGABLESHIP_H
