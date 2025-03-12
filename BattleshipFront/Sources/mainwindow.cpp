#include "../Headers/mainwindow.h"
#include "Headers/battleWindow.h"
#include "ui_mainwindow.h"

//ADICIONAR BOTÃO DE REMOVER NAVIOS DO TABULEIRO

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
    connect(ui->randomizer, &QPushButton::clicked, this, &MainWindow::onRandomizeButtonClicked);
    connect(boardController, &BoardController::boardUpdated, this, &MainWindow::updateBoard);
    connect(ui->clear, &QPushButton::clicked, this, &MainWindow::onClearButtonClicked);
    connect(ui->start, &QPushButton::clicked, this, &MainWindow::onStartButtonClicked);

    //sessão de testes
    selectorSpace = new SelectorSpace(this);
    ui->selectorContainer->setLayout(new QVBoxLayout);
    ui->selectorContainer->layout()->addWidget(selectorSpace);
    selectorSpace->show();

    boardRenderer = new BoardRenderer(scene, shipController, boardController, selectorSpace, playerController);
    boardRenderer->renderCoordinates();

    updateBoard();
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::onRandomizeButtonClicked() {
    boardController->randomizeShips();
    selectorSpace->clearShips();
    ui->clear->setEnabled(true); //botão de limpar fica true
}


void MainWindow::onClearButtonClicked() {
    boardController->clearBoard(); //remove todos os barcos do tabuleiro

    updateBoard(); //atualiza o tabuleiro

    if (selectorSpace) {
        ui->selectorContainer->layout()->removeWidget(selectorSpace);
        delete selectorSpace; //libera a memória do objeto anterior
    }

    // Devolve os navios ao selectorSpace
    //selectorSpace->setupShips();
    selectorSpace = new SelectorSpace(this);
    //ui->selectorContainer->setLayout(new QVBoxLayout);
    ui->selectorContainer->layout()->addWidget(selectorSpace);
    selectorSpace->show();
}


void MainWindow::onStartButtonClicked() {
    // Cria os objetos referentes ao adversário (robô)
    // Supondo que a classe Player possua um construtor que recebe o nome
    Player* enemyPlayer = new Player("Robô");

    // Crie um novo BoardController para o robô
    BoardController* enemyBoardController = new BoardController(enemyPlayer);

    // Crie um PlayerController para o robô
    PlayerController* enemyController = new PlayerController(enemyPlayer);

    enemyBoardController->randomizeShips();

    // Crie a tela de batalha, passando os controllers do jogador e do robô.
    // Aqui, usamos o boardController, shipController e playerController que já existem para o jogador.
    BattleWindow* battleWindow = new BattleWindow(boardController, enemyBoardController, shipController, playerController, enemyController);

    battleWindow->show();

    // Opcional: Fecha ou oculta a tela inicial
    this->close();
}


void MainWindow::updateBoard() {
    boardRenderer->renderBoard();
}

