#include "../Headers/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(
    BoardController* boardController,
    ShipController* shipController,
    PlayerController* playerController,
    QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    boardController(boardController),
    shipController(shipController),
    playerController(playerController) {

    ui->setupUi(this);//instancia inicial do Ui

    //cria o Scene do tabuleiro
    scene = new QGraphicsScene(this);
    ui->boardView->setScene(scene);

    //cria o Scene do selector
    selectorScene = new QGraphicsScene(this);

    setWindowTitle("Batalha batalhesca");

    //conecta as interações do front com os comandos do back
    connect(playerController, &PlayerController::playerUpdated, this, &MainWindow::updateBoard);

    connect(ui->randomizer, &QPushButton::clicked, this, &MainWindow::onRandomizeButtonClicked);
    connect(ui->clear, &QPushButton::clicked, this, &MainWindow::onClearButtonClicked);

    //sessão de testes
    selectorSpace = new SelectorSpace(this);
    ui->selectorContainer->setLayout(new QVBoxLayout);
    ui->selectorContainer->layout()->addWidget(selectorSpace);
    selectorSpace->show();

    boardRenderer = new BoardRenderer(scene, shipController, boardController, selectorSpace, playerController);

    updateBoard();
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::onRandomizeButtonClicked() {
    playerController->positionShipsRandomly();

    selectorSpace->clearShips();
}


void MainWindow::onClearButtonClicked() {
    // Reinicia o estado do board (apenas a camada de visualização/estado interno do frontend)
    playerController->resetBoard();

    updateBoard();  // Atualiza a interface do tabuleiro

    // Recria o SelectorSpace para restaurar os navios disponíveis
    if (selectorSpace) {
        ui->selectorContainer->layout()->removeWidget(selectorSpace);
        delete selectorSpace;
    }
    selectorSpace = new SelectorSpace(this);
    ui->selectorContainer->layout()->addWidget(selectorSpace);
    selectorSpace->show();
}


void MainWindow::updateBoard() {
    boardRenderer->renderBoard();
}

