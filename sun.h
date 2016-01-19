#ifndef SUN_H
#define SUN_H

#include "gameobject.h"
#include <QGraphicsScene>
#include <QObject>
#include <QString>

class Sun : public QObject, public GameObject
{
    Q_OBJECT

public:
    //Default Constructor
    explicit Sun(QObject *parent = 0);
    //Overloaded constructor
    Sun(QString origin);
    //Position and size of the sun object
    QRectF boundingRect() const;
    //Appearence of the sun object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    //Empty setup and initalization
    void setupGameObject() {}
    //Destructor
    ~Sun();

public slots:
    //Slot to capture whether the user clicks on the sun
    void mousePressEvent(QGraphicsSceneMouseEvent *pressEvent);

private slots:
    //Generate the sun
    void generateSun();
    //Stop the sun from moving down the map
    void stopSunMovement();
    //Delete the sun object
    void deleteSun();

    //Stop the movement of the sun
    void stopSunflowerSunMovement();
    //Delete the sunflower sun
    void deleteSunflowerSun();
};

#endif // SUN_H
