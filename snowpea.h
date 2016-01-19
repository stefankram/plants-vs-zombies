#ifndef SNOWPEA_H
#define SNOWPEA_H

#include "gameobject.h"
#include <QObject>

class SnowPea : public QObject, public GameObject
{
    Q_OBJECT

public:
    //Default constructor
    SnowPea(QObject *parent = 0);
    //Get size and position of this game object
    QRectF boundingRect() const;
    //Set the appearence of this game object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    //Setup and initialize the game object
    void setupGameObject();
    //Cost of the object
    static int cost;
    //Destructor
    ~SnowPea();

private slots:
    //Slot to fire bullet from snow pea
    void fireBullet();
};

#endif // SNOWPEA_H
