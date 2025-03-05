#include "../Headers/mainwindow.h"
#include "ui_mainwindow.h"

//ADICIONAR BOTÃO DE REMOVER NAVIOS DO TABULEIRO



MainWindow::MainWindow(BoardController* boardController, ShipController* shipController, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), boardController(boardController), shipController(shipController) {

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

    //sessão de testes
    selectorSpace = new SelectorSpace(this);
    ui->selectorContainer->setLayout(new QVBoxLayout);
    ui->selectorContainer->layout()->addWidget(selectorSpace);
    selectorSpace->show();

    boardRenderer = new BoardRenderer(scene, shipController, boardController, selectorSpace);
    //QPixmap boatTexture("../../Textures/subH.png");
    //DraggableShip *testShip = new DraggableShip(boatTexture, 1);
    //testShip->setPos(10, 10);
    //selectorScene->addItem(testShip);


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


void MainWindow::updateBoard() {
    boardRenderer->renderBoard();
}

