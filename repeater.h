#ifndef REPEATER_H
#define REPEATER_H

#include "gameobject.h"
#include <QObject>

class Repeater : public QObject, public GameObject
{
    Q_OBJECT

public:
    //Default constructor
    explicit Repeater(QObject *parent = 0);
    //Size and position of the current game object
    QRectF boundingRect() const;
    //Set appearence of the current game object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    //Setup and initialize the game object
    void setupGameObject();
    //Cost of the game object
    static int cost;
    //Destructor
    ~Repeater();

private slots:
    //Slot to fire bullet from repeater
    void shootBullet();
};

#endif // REPEATER_H
