#ifndef BOARDRENDERER_H
#define BOARDRENDERER_H

#include <QGraphicsScene>
#include "boardController.h"
#include "shipController.h"


class BoardRenderer : public QObject {
    Q_OBJECT

public:
    BoardRenderer(QGraphicsScene* scene, ShipController* shipController, BoardController* boardController);

    void renderBoard();
    void loadTextures();
    void renderWater();
    void renderShips();

private slots:
    void handleCellClick(int row, int col); //função de testes, remover depois

private:
    QGraphicsScene* scene;
    ShipController* shipController;
    BoardController* boardController;

    QPixmap waterTexture;
    QPixmap shipFistEdgeTextureH;
    QPixmap shipMiddleTextureH;
    QPixmap shipLastEdgeTextureH;
    QPixmap shipFistEdgeTextureV;
    QPixmap shipMiddleTextureV;
    QPixmap shipLastEdgeTextureV;

    QPixmap scaledWaterTexture;
    QPixmap scaledShipFistEdgeTextureH;
    QPixmap scaledShipMiddleTextureH;
    QPixmap scaledShipLastEdgeTextureH;
    QPixmap scaledShipFistEdgeTextureV;
    QPixmap scaledShipMiddleTextureV;
    QPixmap scaledShipLastEdgeTextureV;

};

#endif // BOARDRENDERER_H
