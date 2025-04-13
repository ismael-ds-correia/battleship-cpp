#include "../Headers/boardRenderer.h"
#include "../Headers/boardCell.h"
#include <QFile>
#include <QDir>

//construtor ta gigante, essa formatação foi a melhor forma que eu achei pra deixar isso
//minimamente legivel
BoardRenderer::BoardRenderer(
    QGraphicsScene* scene,
    ShipController* shipController,
    BoardController* boardController,
    SelectorSpace* selectorSpace,
    PlayerController* playerController,
    SoundManager* soundManager,
    RobotController* enemyController,
    bool attackMode)
    : scene(scene),
    shipController(shipController),
    boardController(boardController),
    selectorSpace(selectorSpace),
    playerController(playerController),
    attackMode(attackMode),
    attackerController(nullptr),
    enemyController(nullptr),
    soundManager(soundManager),
    hideShips(false)
{
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
        return;
    }

    if (attackMode) {
        // Obtemos o estado atual do board (inimigo)
        Position (&boardState)[10][10] = boardController->getBoardState();

        if (boardState[row][col].isAttacked()) {
            qDebug() << "Esta célula já foi atacada.";
            return;
        }

        if (attackerController && enemyController) {
            bool hit = attackerController->attackOpponent(enemyController->getPlayer(), row, col);

            // A função attackOpponent já emite o sinal attackResult(row, col, hit),
            // que poderá ser conectado para atualizar a interface.
            qDebug() << "Ataque realizado na célula (" << row << "," << col << "): " << (hit ? "Acertou" : "Errou");

            if (enemyController->getPlayer()->getFleet().isDestroyed()) {
                qDebug() << "Fim de Jogo";
                emit gameOver(true);
            }
        } else {
            qDebug() << "Controles de ataque não configurados corretamente.";
        }

        emit cellClicked(row, col);
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
        updatePlacementIndicators();
        qDebug() << "Barco posicionado com sucesso.";
    } else {
        selectorSpace->onPlacementFailed();
        qDebug() << "Falha ao posicionar o barco.";
    }

    renderBoard();
}

void BoardRenderer::onShipDestroyed(Ship* ship) {
    Position (&boardState)[10][10] = boardController->getBoardState();
    int cellSize = 32;
    int margin = 0;

    std::vector<std::pair<int,int>> shipCells;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (boardState[i][j].getShipReference() == ship) {
                shipCells.push_back({i, j});
            }
        }
    }

    for (const auto &cell : shipCells) {
        int row = cell.first;
        int col = cell.second;
        for (int dRow = -1; dRow <= 1; ++dRow) {
            for (int dCol = -1; dCol <= 1; ++dCol) {
                int newRow = row + dRow;
                int newCol = col + dCol;
                if (newRow < 0 || newRow >= 10 || newCol < 0 || newCol >= 10)
                    continue;
                if (boardState[newRow][newCol].getShipReference() == ship)
                    continue;
                // Se estivermos no modo de ataque (por exemplo, board do inimigo),
                // podemos ignorar o fato da célula já ter sido marcada.
                if (!attackMode && boardState[newRow][newCol].isAttacked())
                    continue;
                // Se for IA (attackMode true), ou se não estiver marcada, marcamos a célula
                boardState[newRow][newCol].attack();

                QPointF pos = calculatePosition(newRow, newCol);
                QGraphicsPixmapItem* overlayItem = new QGraphicsPixmapItem(scaledWaterHitTexture);
                overlayItem->setZValue(100);
                overlayItem->setPos(pos);
                scene->addItem(overlayItem);
            }
        }
    }
    scene->update();
    renderShips();
}

void BoardRenderer::renderShips() {
    //remove os barcos antigos da cena, se houver
    for (QGraphicsItem* item : shipItems) {
        scene->removeItem(item);
        delete item;
    }
    shipItems.clear();

    int cellSize = 32;
    int margin = 0;

    Position (&boardState)[10][10] = boardController->getBoardState();

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            Ship* ship = boardState[i][j].getShipReference();
            if (!ship) continue;

            std::pair<int, int> startPos = shipController->getShipStartPosition(boardState, ship);
            int startRow = startPos.first;
            int startCol = startPos.second;

            if (startRow == -1 || startCol == -1) continue; // Se não encontrar a posição inicial do barco, pula para a próxima célula

            //se não estiver na posição inicial do barco, não renderiza novamente
            if (i != startRow || j != startCol) continue;

            if (hideShips && !ship->isDestroyed()) {
                BoardCell* waterCell = new BoardCell(startRow, startCol, scaledWaterTexture);
                waterCell->setPos(startCol * (cellSize + margin), startRow * (cellSize + margin));
                scene->addItem(waterCell);
                continue;
            }

            int shipSize = ship->getSize();
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
    if (row < 0 || col < 0 || row >= 10 || col >= 10) {
        qDebug() << "Coordenadas inválidas recebidas:" << row << col;
        return; // Ignora coordenadas inválidas.
    }

    QPointF pos = calculatePosition(row, col);
    QGraphicsPixmapItem* overlayItem = new QGraphicsPixmapItem();
    overlayItem->setZValue(100);
    overlayItem->setPos(pos);

    if (hit) {
        overlayItem->setPixmap(shipHitTexture->currentPixmap().scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        // Conecta o sinal frameChanged do QMovie para atualizar este overlay
        // (use uma conexão direta ou lambda para atualizar apenas este item)
        connect(shipHitTexture, &QMovie::frameChanged, this, [=](int /*frame*/) {
            overlayItem->setPixmap(shipHitTexture->currentPixmap().scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            scene->update();
        });
        if (soundManager) {
            soundManager->playHitSound();
        }
    } else {
        overlayItem->setPixmap(scaledWaterHitTexture);
        if (soundManager) {
            soundManager->playWaterSplashSound();
        }
    }

    scene->addItem(overlayItem);
    scene->update();

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
    QFont coordFont("Segoe UI", 10, QFont::Bold);

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
        textItem->setFont(coordFont);
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
        textItem->setFont(coordFont);
        scene->addItem(textItem);
    }
}

void BoardRenderer::updatePlacementIndicators() {
    // Remove (e deleta) indicadores antigos, se existirem
    for (QGraphicsRectItem* item : placementIndicators) {
        scene->removeItem(item);
        delete item;
    }
    placementIndicators.clear();

    // Se não há navio selecionado, não adiciona nenhum overlay e finaliza
    if (selectorSpace->getSelectedShipIndex() < 0) {
        return;
    }

    // Obtemos o boardState para checar as posições
    Position (&boardState)[10][10] = boardController->getBoardState();

    bool horizontal = selectorSpace->isSelectedShipHorizontal();
    int shipSize = selectorSpace->getSelectedShipSize();

    int cellSize = 32;
    int margin = 0;

    // Para cada célula do tabuleiro
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            bool valid = true;
            // Para a verificação, se o navio está na horizontal:
            if (horizontal) {
                if(col + shipSize > 10) {
                    valid = false;
                } else {
                    for (int k = 0; k < shipSize; ++k) {
                        if (!boardState[row][col + k].isValidPosition()) {
                            valid = false;
                            break;
                        }
                    }
                }
            } else {
                // Se vertical
                if(row + shipSize > 10) {
                    valid = false;
                } else {
                    for (int k = 0; k < shipSize; ++k) {
                        if (!boardState[row + k][col].isValidPosition()) {
                            valid = false;
                            break;
                        }
                    }
                }
            }

            // Cria o overlay para a célula
            QGraphicsRectItem* overlay = new QGraphicsRectItem(col * (cellSize + margin),
                                                               row * (cellSize + margin),
                                                               cellSize, cellSize);
            overlay->setZValue(50);  // Valor intermediário para aparecer sobre a água mas abaixo de textos/ataques
            // Seleciona cor: verde semi-transparente para válido e vermelho semi-transparente para inválido
            QColor color = valid ? QColor(0, 255, 0, 125) : QColor(255, 0, 0, 125);
            overlay->setBrush(color);
            overlay->setPen(Qt::NoPen);
            scene->addItem(overlay);
            placementIndicators.push_back(overlay);
        }
    }
}

void BoardRenderer::setSelectorSpace(SelectorSpace* newSelectorSpace) {
    selectorSpace = newSelectorSpace;
}

void BoardRenderer::loadTextures() {
    int cellSize = 32; // Tamanho das células do tabuleiro

    QString texturesPath = resolveResourcePath("Textures");

    if (texturesPath.isEmpty()) {
        qDebug() << "[BoardRenderer] Erro: Pasta de Texturas não encontrada.";
        return;
    }

    qDebug() << "[Path Resolver] Usando texturesPath:" << texturesPath;

    waterTexture.load(texturesPath + "/water.png");
    waterHitTexture.load(texturesPath + "/waterHit.png");

    shipHitTexture = new QMovie(texturesPath + "/fire.gif");
    shipHitTexture->start();

    submarineTextureH.load(texturesPath + "/subH.png");
    battleshipTextureH.load(texturesPath + "/battleshipH.png");
    cruiserTextureH.load(texturesPath + "/cruiserH.png");
    carrierTextureH.load(texturesPath + "/carrierH.png");

    submarineTextureV.load(texturesPath + "/subV.png");
    battleshipTextureV.load(texturesPath + "/battleshipV.png");
    cruiserTextureV.load(texturesPath + "/cruiserV.png");
    carrierTextureV.load(texturesPath + "/carrierV.png");

    scaledWaterTexture = waterTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    scaledWaterHitTexture = waterHitTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    scaledSubmarineTextureH = submarineTextureH.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledCruiserTextureH = cruiserTextureH.scaled(96, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledBattleshipTextureH = battleshipTextureH.scaled(128, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledCarrierTextureH = carrierTextureH.scaled(192, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    scaledSubmarineTextureV = submarineTextureV.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledCruiserTextureV = cruiserTextureV.scaled(cellSize, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledBattleshipTextureV = battleshipTextureV.scaled(cellSize, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledCarrierTextureV = carrierTextureV.scaled(cellSize, 192, Qt::KeepAspectRatio, Qt::SmoothTransformation);

}

