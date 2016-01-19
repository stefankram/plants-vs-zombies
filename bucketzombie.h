#ifndef BUCKETZOMBIE_H
#define BUCKETZOMBIE_H

#include "gameobject.h"
#include <QObject>

class BucketZombie : public QObject, public GameObject
{
    Q_OBJECT

public:
    //Default constructor
    BucketZombie(QObject *parent = 0);
    //Current position and size of the game object
    QRectF boundingRect() const;
    //Set appearence of the game object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    //Setup and initialize the zombie object
    void setupGameObject();
    //Destructor
    ~BucketZombie();

private slots:
    void changeDirection();
    //Attack the plant object
    void attackPlant();
};

#endif // BUCKETZOMBIE_H
