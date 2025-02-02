#include "../Headers/boardCell.h"

BoardCell::BoardCell(int row, int col, const QPixmap& pixmap, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(pixmap, parent), row(row), col(col) {}

void BoardCell::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    emit cellClicked(row, col);
    QGraphicsPixmapItem::mousePressEvent(event);
}

void BoardCell::handleCellClick(int row, int col) {
    qDebug() << "click : linha " << row << " coluna " << col;
}

