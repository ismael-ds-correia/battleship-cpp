#include "../Headers/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Controller* ctrl, QWidget *parent)
    : QMainWindow(parent), controller(ctrl) {

    //cria a cena e o view que será usado
    scene = new QGraphicsScene(this);
    boardView = new QGraphicsView(scene, this);
    setCentralWidget(boardView);

    //tamanho da janela
    setWindowTitle("Batalha batalhesca");
    resize(800, 600);

    loadTextures();

    // Conecta o sinal de atualização do tabuleiro ao slot de atualização da view.
    connect(controller, &Controller::boardUpdated, this, &MainWindow::updateBoard);
    //sessão de testes
    Ship testShip("teste", 3);
    controller->placeShip(2, 3, testShip);

    //^sessão de teste^
    updateBoard();
}

void MainWindow::loadTextures() {
    waterTexture.load("../../Textures/water.png");
    shipTexture.load("../../Textures/boat.png");
}

MainWindow::~MainWindow() {
    delete ui;
}

//subclasse para lidar com os cliques

BoardCell::BoardCell(int row, int col, const QPixmap& pixmap, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(pixmap, parent), row(row), col(col) {}

    //basicamente, cria uma função aprimorada de uma função já existente
    void BoardCell::mousePressEvent(QGraphicsSceneMouseEvent* event) {
        emit cellClicked(row, col); //emite o sinal com as coordenadas
        QGraphicsPixmapItem::mousePressEvent(event); // chama a implementação original
    }

    void MainWindow::handleCellClick(int row, int col) {
        qDebug() << "click : linha " << row << " coluna " << col;
    }

void MainWindow::updateBoard() {
    scene->clear();

    int cellSize = 40; //tamanho em pixels das celulas
    int margin = 0; //margem de espaçamento entre as celulas

    //redimenciona as texturas para um tamanho ideal
    QPixmap scaledWaterTexture = waterTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledShipTexture = shipTexture.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    Position (&boardState)[10][10] = controller->getBoardState();

    //matriz responsavel pela geração do mapa
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            QPixmap* texture = nullptr;

            if (boardState[i][j].getShipReference() != nullptr) { // Condição de se há barco, aplica textura
                texture = &scaledShipTexture;
            } else {
                texture = &scaledWaterTexture;
            }

            BoardCell* cell = new BoardCell(i, j, *texture);
            cell->setPos(j * (cellSize+margin), i * (cellSize+margin));

            //conecta o clique da celula ao handleCellClick
            connect(cell, &BoardCell::cellClicked, this, &MainWindow::handleCellClick);

            scene->addItem(cell);
        }
    }
}

