#ifndef POTATOMINE_H
#define POTATOMINE_H

#include "gameobject.h"
#include <QObject>

class PotatoMine : public QObject, public GameObject
{
    Q_OBJECT

public:
    //Default constructor
    PotatoMine();
    //Size and position of the game object
    QRectF boundingRect() const;
    //Set appearence of the game object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    //Setup and initialize game object
    void setupGameObject();
    //Cost of the plant
    static int cost;
    //Destructor
    ~PotatoMine();

private slots:
    //Slot when the potato mine will become active
    void activateMine();
    //Slot to make the potato mine explode
    void explodeMine();
};

#endif // POTATOMINE_H
