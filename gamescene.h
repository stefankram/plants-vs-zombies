#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>

class GameScene : public QGraphicsScene
{
    Q_OBJECT

public:
    //Default constructor
    GameScene();
    //Get the event when a mouse press occurs
    void mousePressEvent(QGraphicsSceneMouseEvent *pressEvent);
    //Get the event when a mouse movement occurs
    void mouseMoveEvent(QGraphicsSceneMouseEvent *moveEvent);
};

#endif // GAMESCENE_H
