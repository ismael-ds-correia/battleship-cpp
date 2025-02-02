#include "../Headers/boardRenderer.h"
#include "../Headers/boardCell.h"

BoardRenderer::BoardRenderer(QGraphicsScene* scene, ShipController* shipController, BoardController* boardController)
    : scene(scene), shipController(shipController), boardController(boardController) {
    loadTextures();
}

void BoardRenderer::loadTextures() {
    int cellSize = 40; //tamanho das celulas do tabuleiro

    waterTexture.load("../../Textures/water.png");

    shipFistEdgeTextureH.load("../../Textures/ShipFistEdgeH.png");
    shipMiddleTextureH.load("../../Textures/ShipMiddleH.png");
    shipLastEdgeTextureH.load("../../Textures/ShipLastEdgeH.png");

    shipFistEdgeTextureV.load("../../Textures/ShipFistEdgeV.png");
    shipMiddleTextureV.load("../../Textures/ShipMiddleV.png");
    shipLastEdgeTextureV.load("../../Textures/ShipLastEdgeV.png");


    //redimensiona as texturas para tamanhos ideiais
    scaledWaterTexture = waterTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //barco Horizontal
    scaledShipFistEdgeTextureH = shipFistEdgeTextureH.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledShipMiddleTextureH = shipMiddleTextureH.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledShipLastEdgeTextureH = shipLastEdgeTextureH.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //barco Vertical

    scaledShipFistEdgeTextureV = shipFistEdgeTextureV.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledShipMiddleTextureV = shipMiddleTextureV.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledShipLastEdgeTextureV = shipLastEdgeTextureV.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

}

void BoardRenderer::renderBoard() {
    scene->clear();
    renderWater();
    renderShips();
}

void BoardRenderer::renderWater() {
    int cellSize = 40;
    int margin = 0;

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            BoardCell* waterCell = new BoardCell(i, j, scaledWaterTexture);
            waterCell->setPos(j * (cellSize+margin), i * (cellSize+margin));

            connect(waterCell, &BoardCell::cellClicked, this, &BoardRenderer::handleCellClick);

            scene->addItem(waterCell);
        }
    }
}

//função de testes, remover quando finalizar
void BoardRenderer::handleCellClick(int row, int col) {
    qDebug() << "click: " << row << col;
}

void BoardRenderer::renderShips() {
    int cellSize = 40;
    int margin = 0;

    Position (&boardState)[10][10] = boardController->getBoardState();

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            Ship* ship = boardState[i][j].getShipReference();
            if (!ship) continue;//se não houver barco na celula, vai para a proxima

            std::pair<int, int> startPos = shipController->getShipStartPosition(boardController->getBoardState(), ship);

            int startRow = startPos.first;
            int startCol = startPos.second;

            if (startRow == -1 || startCol == -1) continue;  // Se não encontrar a posição inicial do barco, passa para a próxima célula

            int shipSize = ship->getSize();
            QPixmap* texture = nullptr;

            if (ship->isHorizontal()) { //se for horizontal
                if (j == startCol) {
                    texture = &scaledShipFistEdgeTextureH;
                } else if (j == startCol + shipSize - 1) {
                    texture = &scaledShipLastEdgeTextureH;
                } else {
                    texture = &scaledShipMiddleTextureH;
                }
            } else { //se for vertical
                if (i == startRow) {
                    texture = &scaledShipFistEdgeTextureV;
                } else if (i == startRow + shipSize - 1) {
                    texture = &scaledShipLastEdgeTextureV;
                } else {
                    texture = &scaledShipMiddleTextureV;
                }
            }

            // Cria uma célula para o barco com a textura correta
            BoardCell* shipCell = new BoardCell(i, j, *texture);
            shipCell->setPos(j * (cellSize + margin), i * (cellSize + margin));
            scene->addItem(shipCell);
        }
    }
}




