#ifndef WALLNUT_H
#define WALLNUT_H

#include "gameobject.h"

class WallNut : public GameObject
{
public:
    //Default constructor
    WallNut();
    //Size and position of the game object
    QRectF boundingRect() const;
    //Set the appearence of the game object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    //Cost of the object
    static int cost;
    //Setup and initialization function
    void setupGameObject();
    //Destructor
    ~WallNut();
};

#endif // WALLNUT_H
