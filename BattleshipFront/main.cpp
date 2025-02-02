#include "Headers/mainwindow.h"

#include <QApplication>
#include "boardController.h"
#include "shipController.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    BoardController boardController; //instancia o controllador
    ShipController shipController;

    MainWindow mainWindow(&boardController, &shipController); //cria a janela do jogo
    mainWindow.show();

    return app.exec();
}
