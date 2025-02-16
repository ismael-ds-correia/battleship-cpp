#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "boardController.h"
#include "boardRenderer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::MainWindow* ui;

    QGraphicsView* boardView;
    QGraphicsScene* scene;
    BoardController* boardController;
    ShipController* shipController;
    BoardRenderer* boardRenderer;

    void loadTextures();

public:
    explicit MainWindow(BoardController* boardController, ShipController* shipController, QWidget *parent = nullptr);
    void positionShipsRandomly();
    ~MainWindow();


private slots:
    //void handleCellClick(int row, int col);
    void onRandomizeButtonClicked();
    void updateBoard();

};
#endif // MAINWINDOW_H
