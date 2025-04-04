#include "../Headers/boardRenderer.h"
#include "../Headers/boardCell.h"
#include "../Headers/battleWindow.h"

//construtor ta gigante, essa formatação foi a melhor forma que eu achei pra deixar isso
//minimamente legivel
BoardRenderer::BoardRenderer(
    QGraphicsScene* scene,
    ShipController* shipController,
    BoardController* boardController,
    SelectorSpace* selectorSpace,
    PlayerController* playerController,
    PlayerController* enemyController,
    bool attackMode)
    : scene(scene),
    shipController(shipController),
    boardController(boardController),
    selectorSpace(selectorSpace),
    playerController(playerController),
    hideShips(false),
    attackMode(attackMode),
    attackerController(nullptr),
    enemyController(nullptr) {
    // Se estivermos em modo de ataque, o playerController passado originalmente
    // é o atacante e o enemyController é o que está sendo atacado.
    if (attackMode) {
        attackerController = playerController;
        this->enemyController = enemyController;
    }

    loadTextures();
    renderWater();
}


void BoardRenderer::setHideShips(bool hide) {
    hideShips = hide;
}


void BoardRenderer::renderBoard() {
    //renderWater();
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

void BoardRenderer::handleCellClick(int row, int col) {
    qDebug() << "Iniciando handleCellClick " << row << col;
    if (!isInteractive){
        qDebug() << "Clique ignorado por ser do próprio tabuleiro";
        return; // Ignora cliques se não for interativo
    }

    // Se estivermos no modo de ataque, processamos o clique como um comando de ataque.
    if (attackMode) {
        // Obtemos o estado atual do board (inimigo)
        Position (&boardState)[10][10] = boardController->getBoardState();

        // Se a célula já foi atacada, ignoramos o clique.
        if (boardState[row][col].isAttacked()) {
            qDebug() << "Esta célula já foi atacada.";
            return;
        }

        // Dispara o ataque: reutilizamos o método do backend.
        // OBSERVAÇÃO: Para isso, usamos o método attackOpponent do attackerController,
        // que espera um ponteiro para o Player do oponente. Para isso, vamos precisar de um getter.
        if (attackerController && enemyController) {
            bool hit = attackerController->attackOpponent(enemyController->getPlayer(), row, col);
            // A função attackOpponent já emite o sinal attackResult(row, col, hit),
            // que poderá ser conectado para atualizar a interface.
            qDebug() << "Ataque realizado na célula (" << row << "," << col << "): " << (hit ? "Acertou" : "Errou");

            if (enemyController->getPlayer()->getFleet().isDestroyed()) {
                qDebug() << "Fim de Jogo";
                emit gameOver(true); // Jogador venceu
            }
        } else {
            qDebug() << "Controles de ataque não configurados corretamente.";
        }

        return;
    }

    //retorna o estado atual do board
    Position (&boardState)[10][10] = boardController->getBoardState();
    Ship* clickedShip = boardState[row][col].getShipReference();

    //Se a célula contém um navio e nenhum navio está selecionado no SelectorSpace,
    //interpretamos o clique como remoção.
    if (clickedShip && selectorSpace->getSelectedShipIndex() < 0) {
        qDebug() << "Removendo barco da célula (" << row << "," << col << ")";

        boardController->removeShip(*clickedShip);
        // Restaura o navio no SelectorSpace utilizando o objeto Ship removido
        selectorSpace->restoreShip(*clickedShip);
        renderBoard();
        return;
    }

    //se tiver um navio selecionado, tenta posicioná-lo na célula clicada
    int shipIndex = selectorSpace->getSelectedShipIndex();
    if (shipIndex < 0) {
        qDebug() << "Nenhum barco selecionado!";
        return;
    }

    bool horizontal = selectorSpace->isSelectedShipHorizontal();
    bool success = playerController->placeShipFromFleet(shipIndex, row, col, horizontal);

    if (success) {
        selectorSpace->markShipAsPlaced(shipIndex);
        selectorSpace->clearSelectedShip(); // redundante, mas para garantir
        qDebug() << "Barco posicionado com sucesso.";
    } else {
        qDebug() << "Falha ao posicionar o barco.";
    }

    renderBoard(); //atualiza o render do tabuleiro
}

void BoardRenderer::onShipDestroyed(Ship* ship) {
    // Obtém o estado atual do board.
    Position (&boardState)[10][10] = boardController->getBoardState();
    int cellSize = 32;
    int margin = 0;

    // Vetor para armazenar as coordenadas das células que pertencem ao navio.
    std::vector<std::pair<int,int>> shipCells;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (boardState[i][j].getShipReference() == ship) {
                shipCells.push_back({i, j});
            }
        }
    }

    // Para cada célula que faz parte do navio, marque as células adjacentes.
    for (const auto &cell : shipCells) {
        int row = cell.first;
        int col = cell.second;
        for (int dRow = -1; dRow <= 1; ++dRow) {
            for (int dCol = -1; dCol <= 1; ++dCol) {
                int newRow = row + dRow;
                int newCol = col + dCol;
                // Verifica se a posição está dentro dos limites do board.
                if (newRow < 0 || newRow >= 10 || newCol < 0 || newCol >= 10)
                    continue;
                // Se a célula adjacente faz parte do próprio navio, não processa.
                if (boardState[newRow][newCol].getShipReference() == ship)
                    continue;
                // Se a célula ainda não foi atacada, marca-a e desenha o overlay.
                if (!boardState[newRow][newCol].isAttacked()) {
                    boardState[newRow][newCol].attack();

                    // Adiciona overlay com a textura de água atingida.
                    QPointF pos = calculatePosition(newRow, newCol);
                    QGraphicsPixmapItem* overlayItem = new QGraphicsPixmapItem(scaledWaterHitTexture);
                    overlayItem->setZValue(100);
                    overlayItem->setPos(pos);
                    scene->addItem(overlayItem);
                }
            }
        }
    }
    scene->update();
    renderShips();
}

void BoardRenderer::renderShips() {
    // Remove os barcos antigos da cena, se houver
    for (QGraphicsItem* item : shipItems) {
        scene->removeItem(item);
        delete item;  // Se for apropriado gerenciar a memória dessa forma
    }
    shipItems.clear();

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

            if (hideShips && !ship->isDestroyed()) {
                // Se a flag estiver ativa, desenha a água no lugar do navio
                BoardCell* waterCell = new BoardCell(startRow, startCol, scaledWaterTexture);
                waterCell->setPos(startCol * (cellSize + margin), startRow * (cellSize + margin));
                scene->addItem(waterCell);
                continue;
            }

            int shipSize = ship->getSize(); // Tamanho do barco
            QPixmap* texture = nullptr;

            if (shipSize == 6) {
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

            // Criar uma célula para o barco inteiro
            BoardCell* shipCell = new BoardCell(startRow, startCol, *texture);
            shipCell->setPos(startCol * (cellSize + margin), startRow * (cellSize + margin));

            scene->addItem(shipCell);
            shipItems.push_back(shipCell); // Armazena o item para futuras remoções
        }
    }
}

void BoardRenderer::onAttackResult(int row, int col, bool hit) {
    QPixmap overlayPixmap;
    if (hit) {
        overlayPixmap = scaledShipHitTexture; // Textura para acerto (fogo)
    } else {
        overlayPixmap = scaledWaterHitTexture; // Textura para água
    }

    if (overlayPixmap.isNull()) {
        qDebug() << "Falha ao carregar a textura de overlay!";
        return;
    }

    QPointF pos = calculatePosition(row, col); // Converte coordenadas da grade para posição na cena.
    QGraphicsPixmapItem* overlayItem = new QGraphicsPixmapItem(overlayPixmap);
    overlayItem->setZValue(100);
    overlayItem->setPos(pos);
    scene->addItem(overlayItem);

    scene->update();
}

void BoardRenderer::setInteractive(bool interactive) {
    isInteractive = interactive;
}

QPointF BoardRenderer::calculatePosition(int row, int col) const {
    int cellSize = 32;
    int margin = 0;

    int x = col * (cellSize + margin);
    int y = row * (cellSize + margin);
    return QPointF(x, y);
}

void BoardRenderer::renderCoordinates() {
    int cellSize = 32;
    int margin = 0;

    // Adiciona rótulos para as colunas (A-J)
    for (int j = 0; j < 10; j++) {
        QString label = QString(QChar('A' + j));
        QGraphicsTextItem* textItem = new QGraphicsTextItem(label);
        QRectF textRect = textItem->boundingRect();
        // Posiciona o texto acima de cada coluna, centralizado na célula
        qreal x = j * (cellSize + margin) + (cellSize - textRect.width()) / 2;
        qreal y = -textRect.height() - 5;  // 5 pixels de margem
        textItem->setPos(x, y);
        scene->addItem(textItem);
    }

    // Adiciona rótulos para as linhas (0-9)
    for (int i = 0; i < 10; i++) {
        QString label = QString::number(i);
        QGraphicsTextItem* textItem = new QGraphicsTextItem(label);
        QRectF textRect = textItem->boundingRect();
        // Posiciona o texto à esquerda de cada linha, centralizado na célula
        qreal x = -textRect.width() - 5;  // 5 pixels de margem
        qreal y = i * (cellSize + margin) + (cellSize - textRect.height()) / 2;
        textItem->setPos(x, y);
        scene->addItem(textItem);
    }
}

void BoardRenderer::setSelectorSpace(SelectorSpace* newSelectorSpace) {
    selectorSpace = newSelectorSpace;
}

void BoardRenderer::loadTextures() {
    int cellSize = 32;//tamanho das celulas do tabuleiro

    waterTexture.load("../../Textures/water.png");

    waterHitTexture.load("../../Textures/waterHit.png");
    shipHitTexture.load("../../Textures/fire.gif");


    submarineTextureH.load("../../Textures/subH.png");
    battleshipTextureH.load("../../Textures/battleshipH.png");
    cruiserTextureH.load("../../Textures/cruiserH.png");
    carrierTextureH.load("../../Textures/carrierH.png");

    submarineTextureV.load("../../Textures/subV.png");
    battleshipTextureV.load("../../Textures/battleshipV.png");
    cruiserTextureV.load("../../Textures/cruiserV.png");
    carrierTextureV.load("../../Textures/carrierV.png");

    scaledWaterTexture = waterTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    scaledWaterHitTexture = waterHitTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledShipHitTexture = shipHitTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    scaledSubmarineTextureH = submarineTextureH.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledCruiserTextureH = cruiserTextureH.scaled(96, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledBattleshipTextureH = battleshipTextureH.scaled(128, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledCarrierTextureH = carrierTextureH.scaled(192, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    scaledSubmarineTextureV = submarineTextureV.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledCruiserTextureV = cruiserTextureV.scaled(cellSize, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledBattleshipTextureV = battleshipTextureV.scaled(cellSize, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledCarrierTextureV = carrierTextureV.scaled(cellSize, 192, Qt::KeepAspectRatio, Qt::SmoothTransformation);

}
