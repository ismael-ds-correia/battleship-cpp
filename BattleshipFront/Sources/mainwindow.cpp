#include "../Headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "player.h"

MainWindow::MainWindow(BoardController* boardController, ShipController* shipController, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), boardController(boardController), shipController(shipController) {

    ui->setupUi(this);
    scene = new QGraphicsScene(this);//cria a cena e o view que será usado
    ui->boardView->setScene(scene);

    setWindowTitle("Batalha batalhesca");

    boardRenderer = new BoardRenderer(scene, shipController, boardController);
    //boardRenderer->loadTextures();
    // Conecta o sinal de atualização do tabuleiro ao slot de atualização da view.
    connect(ui->randomizer, &QPushButton::clicked, this, &MainWindow::onRandomizeButtonClicked);
    connect(boardController, &BoardController::boardUpdated, this, &MainWindow::updateBoard);



    //sessão de testes
    /*Ship testShip1("teste1", 3);
    Ship testShip2("teste2", 4);
    testShip2.setOrientation(false);
    boardController->placeShip(2, 3, testShip1);
    boardController->placeShip(6, 2, testShip2);*/

    updateBoard();
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::onRandomizeButtonClicked() {
    boardController->randomizeShips();
}


//subclasse para lidar com os cliques


void MainWindow::updateBoard() {
    boardRenderer->renderBoard();
}

// void MainWindow::updateBoard() {
//     scene->clear();

//     int cellSize = 40; //tamanho em pixels das celulas
//     int margin = 0; //margem de espaçamento entre as celulas

//     //redimenciona as texturas para um tamanho ideal
//     QPixmap scaledWaterTexture = waterTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//     QPixmap scaledShipTexture = shipTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

//     Position (&boardState)[10][10] = controller->getBoardState();

//     //matriz responsavel pela geração do mapa
//     for (int i = 0; i < 10; ++i) {
//         for (int j = 0; j < 10; ++j) {
//             QPixmap* texture = nullptr;

//             if (boardState[i][j].getShipReference() != nullptr) { // Condição de se há barco, aplica textura
//                 texture = &scaledShipTexture;
//             } else {
//                 texture = &scaledWaterTexture;
//             }

//             BoardCell* cell = new BoardCell(i, j, *texture);
//             cell->setPos(j * (cellSize+margin), i * (cellSize+margin));

//             //conecta o clique da celula ao handleCellClick
//             connect(cell, &BoardCell::cellClicked, this, &MainWindow::handleCellClick);

//             scene->addItem(cell);
//         }
//     }
// }

