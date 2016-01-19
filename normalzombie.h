#ifndef NORMALZOMBIE_H
#define NORMALZOMBIE_H

#include "gameobject.h"

class NormalZombie : public GameObject
{
public:
    //Constructor
    NormalZombie();
    //Current position and size of the game object
    QRectF boundingRect() const;
    //Set the appearence of the game object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    //Setup and initialize the zombie object
    void setupGameObject();
    //Destructor
    ~NormalZombie();
};

#endif // NORMALZOMBIE_H
