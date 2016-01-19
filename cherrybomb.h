#ifndef CHERRYBOMB_H
#define CHERRYBOMB_H

#include "gameobject.h"
#include <QObject>
#include <QTimer>

class CherryBomb : public QObject, public GameObject
{
    Q_OBJECT

public:
    //Default constructor
    explicit CherryBomb(QObject *parent = 0);
    //Size and position of this game object
    QRectF boundingRect() const;
    //Set appearence of the game object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    //Setup and initialize game object
    void setupGameObject();
    //Cost of the object
    static int cost;
    //Destructor
    ~CherryBomb();

private slots:
    //Slot to make the cherry bomb explode
    void explodeCherryBomb();
    //Slot to destroy the cherry bomb
    void destroyCherryBomb();
};

#endif // CHERRYBOMB_H
