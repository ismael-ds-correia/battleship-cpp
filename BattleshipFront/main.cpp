#include "Headers/mainwindow.h"

#include <QApplication>
#include "controller.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Controller controller; //instancia o controllador

    MainWindow mainWindow(&controller); //cria a janela do jogo
    mainWindow.show();

    return app.exec();
}
