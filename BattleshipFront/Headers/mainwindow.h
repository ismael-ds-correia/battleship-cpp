#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QPixmap>
#include <QPointF>
#include <QVBoxLayout>
#include "boardController.h"
#include "boardRenderer.h"
#include "selectorSpace.h"

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
    QGraphicsScene* selectorScene;
    BoardController* boardController;
    ShipController* shipController;
    BoardRenderer* boardRenderer;
    SelectorSpace* selectorSpace;

    void loadTextures();

public:
    explicit MainWindow(BoardController* boardController, ShipController* shipController, QWidget *parent = nullptr);

    void positionShipsRandomly();
    ~MainWindow();


private slots:
    //void handleCellClick(int row, int col);
    void onRandomizeButtonClicked();
    void updateBoard();
    void onClearButtonClicked();

};
#endif // MAINWINDOW_H
