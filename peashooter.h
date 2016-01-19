#ifndef PEASHOOTER_H
#define PEASHOOTER_H

#include "gameobject.h"
#include <QObject>

class PeaShooter : public QObject, public GameObject
{
    Q_OBJECT

public:
    //Default constructor
    explicit PeaShooter(QObject *parent = 0);
    //Size and position of the game object
    QRectF boundingRect() const;
    //Set the appearence of the game object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    //Setup and initialize the game object
    void setupGameObject();
    //Cost of the object
    static int cost;
    //Destructor
    ~PeaShooter();

private slots:
    //Slot to fire bullet from pea shooter
    void fireBullet();
};

#endif // PEASHOOTER_H
