#ifndef SNOWPEABULLET_H
#define SNOWPEABULLET_H

#include "gameobject.h"

class SnowPeaBullet : public GameObject
{
public:
    //Default constructor
    SnowPeaBullet();
    //Size and position of the game object
    QRectF boundingRect() const;
    //Set appearence of the game object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*);
    //Empty initialization
    void setupGameObject()
    {}
};

#endif // SNOWPEABULLET_H
