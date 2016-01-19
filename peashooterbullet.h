#ifndef PEASHOOTERBULLET_H
#define PEASHOOTERBULLET_H

#include "gameobject.h"

class PeaShooterBullet : public GameObject
{
public:
    //Default constructor
    PeaShooterBullet();
    //Size and position of the game object
    QRectF boundingRect() const;
    //Set appearence of the game object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*);
    //Empty initialization
    void setupGameObject()
    {}
};

#endif // PEASHOOTERBULLET_H
