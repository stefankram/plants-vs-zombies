#ifndef FLAGZOMBIE_H
#define FLAGZOMBIE_H

#include "gameobject.h"

class FlagZombie : public GameObject
{
public:
    //Default constructor
    FlagZombie();
    //Current position and size of the game object
    QRectF boundingRect() const;
    //Set appearence of the game object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    //Setup and initialize zombie object
    void setupGameObject();
    //Destructor
    ~FlagZombie();
};

#endif // FLAGZOMBIE_H
