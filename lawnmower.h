#ifndef LAWNMOWER_H
#define LAWNMOWER_H

#include "gameobject.h"

class LawnMower : public GameObject
{
public:
    //Default constructor
    LawnMower();
    //Size and position of the game object
    QRectF boundingRect() const;
    //Set appearance of the game object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    //Setup and initialize the object
    void setupGameObject() {}
    //Destructor
    ~LawnMower();

private:
    bool collidedWithZombie;
};

#endif // LAWNMOWER_H
