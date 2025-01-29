#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include "controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class BoardCell : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    explicit BoardCell(int row, int col, const QPixmap& texture, QGraphicsItem* parent = nullptr);

signals:
    void cellClicked(int row, int col);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    int row, col;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    QGraphicsView* boardView;
    QGraphicsScene* scene;
    Controller* controller;
    QPixmap waterTexture;
    QPixmap shipTexture;

    void loadTextures();

public:
    explicit MainWindow(Controller* ctrl, QWidget *parent = nullptr);

    void updateBoard();
    ~MainWindow();

private slots:
    void handleCellClick(int row, int col);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
