#ifndef SUNFLOWER_H
#define SUNFLOWER_H

#include "gameobject.h"
#include <QObject>

class SunFlower : public QObject, public GameObject
{
    Q_OBJECT

public:
    //Default constructor
    explicit SunFlower(QObject *parent = 0);
    //Size and position of the game object
    QRectF boundingRect() const;
    //Set the appearence of the game object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    //Cost of the sunflower
    static int cost;
    //Setup and initialize the sunflower object
    void setupGameObject();
    //Destructor
    ~SunFlower();

private slots:
    //Display sun
    void displaySun();
};

#endif // SUNFLOWER_H
