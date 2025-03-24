#include "../Headers/mainwindow.h"
#include "Headers/battleWindow.h"
#include "ui_mainwindow.h"

#include "robotplayer.h"

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
    connect(ui->start, &QPushButton::clicked, this, &MainWindow::onStartButtonClicked);

    //sessão de testes
    selectorSpace = new SelectorSpace(this);
    ui->selectorContainer->setLayout(new QVBoxLayout);
    ui->selectorContainer->layout()->addWidget(selectorSpace);
    selectorSpace->show();

    boardRenderer = new BoardRenderer(scene, shipController, boardController, selectorSpace, playerController);

    boardRenderer->setInteractive(true);
    boardRenderer->renderCoordinates();

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
    //reinicia o estado do board (apenas a camada de visualização/estado interno do frontend)
    playerController->resetBoard();

    updateBoard();

    // Recria o SelectorSpace para restaurar os navios disponíveis
    if (selectorSpace) {
        ui->selectorContainer->layout()->removeWidget(selectorSpace);
        delete selectorSpace;
    }
    selectorSpace = new SelectorSpace(this);
    ui->selectorContainer->layout()->addWidget(selectorSpace);
    selectorSpace->show();

    boardRenderer->setSelectorSpace(selectorSpace);
}


void MainWindow::onStartButtonClicked() {
    RobotPlayer* enemyPlayer = new RobotPlayer(); //nome da IA não está sendo passada

    BoardController* enemyBoardController = new BoardController(enemyPlayer);

    //cria um RobotController para a IA(a classe RobotController herda de PlayerController).
    RobotController* enemyController = new RobotController(enemyPlayer);

    enemyBoardController->randomizeShips();

    BattleWindow* battleWindow = new BattleWindow(
        boardController,
        enemyBoardController,
        shipController,
        playerController,
        enemyController
    );

    battleWindow->show();

    // Opcional: fecha ou oculta a tela inicial.
    this->close();
}


void MainWindow::updateBoard() {
    boardRenderer->renderBoard();
}

