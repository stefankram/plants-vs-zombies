#ifndef CONEHEADZOMBIE_H
#define CONEHEADZOMBIE_H

#include "gameobject.h"

class ConeheadZombie : public GameObject
{
public:
    //Default constructor
    ConeheadZombie();
    //Current position and size of the game object
    QRectF boundingRect() const;
    //Set the appearence of the game object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    //Setup and initialize the zombie object
    void setupGameObject();
    //Destructor
    ~ConeheadZombie();
};

#endif // CONEHEADZOMBIE_H
