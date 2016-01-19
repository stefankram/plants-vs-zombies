#ifndef CHOMPER_H
#define CHOMPER_H

#include "gameobject.h"
#include <QObject>

class Chomper : public QObject, public GameObject
{
    Q_OBJECT

public:
    //Default constructor
    explicit Chomper(QObject *parent = 0);
    //Size and position of the game object
    QRectF boundingRect() const;
    //Set the appearence of the game object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    //Setup and initialize the game object
    void setupGameObject();
    //Cost of the object
    static int cost;
    //Destructor
    ~Chomper();

private slots:
    //Slot when chomper is done eating
    void doneEating();
};

#endif // CHOMPER_H
