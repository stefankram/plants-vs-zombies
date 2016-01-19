#include "gamescene.h"
#include <QGraphicsSceneMouseEvent>
#include "mainwindow.h"

GameScene::GameScene() : QGraphicsScene()   //Default constructor
{}

//Get the event when the mouse is pressed
void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *pressEvent)
{
    //Set the coordinates of the last mouse pressed event
    MainWindow::mousePressCoordinates = pressEvent->scenePos();
    QGraphicsScene::mousePressEvent(pressEvent);
}

//Get the event when the mouse is moved
void GameScene::mouseMoveEvent(QGraphicsSceneMouseEvent *moveEvent)
{
    //Set the coordinates of the last mouse move event
    MainWindow::mouseCoordinates = moveEvent->scenePos();
    QGraphicsScene::mouseMoveEvent(moveEvent);
}
