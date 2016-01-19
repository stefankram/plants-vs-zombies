#ifndef NEWSPAPERZOMBIE_H
#define NEWSPAPERZOMBIE_H

#include "gameobject.h"

class NewspaperZombie : public GameObject
{
public:
    //Default constructor
    NewspaperZombie();
    //Current position and size of the game object
    QRectF boundingRect() const;
    //Set the appearence of the game object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    //Setup and initialize the zombie object
    void setupGameObject();
    //Destructor
    ~NewspaperZombie();
};

#endif // NEWSPAPERZOMBIE_H
