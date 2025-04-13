#include "../Headers/boardCell.h"

BoardCell::BoardCell(int row, int col, const QPixmap& pixmap, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(pixmap, parent), row(row), col(col) {
    //setCursor(Qt::PointingHandCursor);
    setAcceptHoverEvents(true);
}

void BoardCell::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    emit cellClicked(row, col);
    QGraphicsPixmapItem::mousePressEvent(event);
}

void BoardCell::handleCellClick(int row, int col) {
    qDebug() << "click : linha " << row << " coluna " << col;
}

void BoardCell::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    setOpacity(0.8);
    update();
}

void BoardCell::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    setOpacity(1.0);
    update();
}
