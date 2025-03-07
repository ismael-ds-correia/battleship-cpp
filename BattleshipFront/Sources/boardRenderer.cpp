#include "../Headers/boardRenderer.h"
#include "../Headers/boardCell.h"

//construtor ta gigante, essa formatação foi a melhor forma que eu achei pra deixar isso
//minimamente legivel
BoardRenderer::BoardRenderer(
    QGraphicsScene* scene,
    ShipController* shipController,
    BoardController* boardController,
    SelectorSpace* selectorSpace,
    PlayerController* playerController)
    : scene(scene),
    shipController(shipController),
    boardController(boardController),
    selectorSpace(selectorSpace),
    playerController(playerController) {

    loadTextures();
}


void BoardRenderer::renderBoard() {
    //scene->clear();
    renderWater();
    renderShips();
}

void BoardRenderer::renderWater() {
    int cellSize = 32;
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
    qDebug() << "Iniciando handleCellClick " << row << col;

    int shipIndex = selectorSpace->getSelectedShipIndex();
    qDebug() << "endereço de selectorSpace: " << selectorSpace;

    if (shipIndex < 0) {
        qDebug() << "Nenhum barco selecionado!";
        return;
    }

    bool horizontal = selectorSpace->isHorizontal();

    /*
    // Determinar o nome do barco baseado no tamanho
    QString shipName;
    if (shipSize == 6) {
        shipName = "carrier";
    } else if (shipSize == 4) {
        shipName = "battleship";
    } else if (shipSize == 3) {
        shipName = "cruiser";
    } else if (shipSize == 1) {
        shipName = "sub";
    } else {
        qDebug() << "Erro: tamanho de barco inválido!";
        return;
    }
    Ship tempShip(shipName.toStdString(), shipSize);
    */
    bool sucess = playerController->placeShipFromFleet(shipIndex, row, col, horizontal);

    if (sucess) {
        selectorSpace->markShipAsPlaced(shipIndex);
        qDebug() << "barco posicionado com sucesso";
    } else {
        qDebug() << "falha ao posicionar o barco";
    }

    //boardController->placeShip(row, col, tempShip); // Insere o barco no tabuleiro
    //qDebug() << "PlaceShipRealizado";

    renderBoard(); //atualiza a interface gráfica para renderizar o barco
}

void BoardRenderer::renderShips() {
    int cellSize = 32; // Tamanho da célula para o desenho das texturas
    int margin = 0;

    Position (&boardState)[10][10] = boardController->getBoardState();

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            Ship* ship = boardState[i][j].getShipReference();
            if (!ship) continue; // Se não houver barco, pula para a próxima célula

            std::pair<int, int> startPos = shipController->getShipStartPosition(boardState, ship);
            int startRow = startPos.first;
            int startCol = startPos.second;

            if (startRow == -1 || startCol == -1) continue; // Se não encontrar a posição inicial do barco, pula para a próxima célula

            // 🔹 Se não estiver na posição inicial do barco, não renderiza novamente
            if (i != startRow || j != startCol) continue;

            int shipSize = ship->getSize(); // Tamanho do barco
            QPixmap* texture = nullptr;

            if (shipSize == 5) {
                texture = ship->isHorizontal() ? &scaledCarrierTextureH : &scaledCarrierTextureV;
            }
            else if (shipSize == 4) {
                texture = ship->isHorizontal() ? &scaledBattleshipTextureH : &scaledBattleshipTextureV;
            }
            else if (shipSize == 3) {
                texture = ship->isHorizontal() ? &scaledCruiserTextureH : &scaledCruiserTextureV;
            }
            else if (shipSize == 1) {
                texture = ship->isHorizontal() ? &scaledSubmarineTextureH : &scaledSubmarineTextureH;
            }

            if (!texture) {
                qDebug() << "Erro: Nenhuma textura encontrada para um barco de tamanho" << shipSize;
                continue;
            }

            //Criar apenas uma célula para o barco inteiro
            BoardCell* shipCell = new BoardCell(startRow, startCol, *texture);
            shipCell->setPos(startCol * (cellSize + margin), startRow * (cellSize + margin));

            scene->addItem(shipCell);
        }
    }
}

void BoardRenderer::loadTextures() {
    int cellSize = 32;//tamanho das celulas do tabuleiro

    waterTexture.load("../../Textures/water.png");

    submarineTextureH.load("../../Textures/subH.png");
    battleshipTextureH.load("../../Textures/battleshipH.png");
    cruiserTextureH.load("../../Textures/cruiserH.png");
    carrierTextureH.load("../../Textures/carrierH.png");

    submarineTextureV.load("../../Textures/subV.png");
    battleshipTextureV.load("../../Textures/battleshipV.png");
    cruiserTextureV.load("../../Textures/cruiserV.png");
    carrierTextureV.load("../../Textures/carrierV.png");

    scaledWaterTexture = waterTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    scaledSubmarineTextureH = submarineTextureH.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledCruiserTextureH = cruiserTextureH.scaled(96, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledBattleshipTextureH = battleshipTextureH.scaled(128, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledCarrierTextureH = carrierTextureH.scaled(192, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    scaledSubmarineTextureV = submarineTextureV.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledCruiserTextureV = cruiserTextureV.scaled(cellSize, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledBattleshipTextureV = battleshipTextureV.scaled(cellSize, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledCarrierTextureV = carrierTextureV.scaled(cellSize, 192, Qt::KeepAspectRatio, Qt::SmoothTransformation);

}
