#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "body.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QString>
#include <QLabel>
#include <QTimer>

class GameObject : public QGraphicsItem
{
public:
    //Default constructor
    explicit GameObject();
    //Set the body of the game object
    void setBody(Body b);
    //Move the game object given the time interval
    void move(double time);
    //Pure virtual function describing the position and size of the derived game objects
    virtual QRectF boundingRect() const = 0;
    //Pure virtual function seting th eappearence of the game objects
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) = 0;
    //Setup and initialize derived game objects
    virtual void setupGameObject() = 0;
    //Destructor
    virtual ~GameObject();
    //Get various game object attributes
    int getWidth();
    int getHeight();
    int getCost();
    int getLife();
    int getRange();
    int getDamage();
    float getRate();
    int getSplash();
    int getSlow();
    int getBomb();
    float getSeeding();
    int getSun();
    int getNeed();
    int getArmour();
    int getAttack();
    float getSpeed();
    QString getImageUrl();
    int getImageUrlWidth();
    int getImageUrlHeight();
    QString getImageUrlAlt();
    int getImageUrlAltWidth();
    int getImageUrlAltHeight();
    QString getObjectType();
    bool isSlower();
    //Set various game objects
    void setSpeed(float s);
    void setXVelocity(double vx);
    void setYVelocity(double vy);
    void setLife(int life);
    void setArmour(int armour);
    void setDamage(int damage);
    void setSlower(bool b);
    //Gif image of all derived objects
    QLabel *gameObjectGif;
    //Timer for any use in derived objects
    QTimer *multiUseTimer;

protected:
    //Body of the game object
    Body body;
    //Advance function to move the game object
    void advance(int phase);
    //Various game object attributes
    int width;
    int height;
    int life;
    int range;
    int damage;
    float rate;
    int splash;
    int slow;
    int bomb;
    int itemCost;
    float seeding;
    int sun;
    int need;
    int armour;
    int attack;
    float speed;
    QString imageUrl;
    int imageUrlWidth;
    int imageUrlHeight;
    QString imageUrlAlt;
    int imageUrlAltWidth;
    int imageUrlAltHeight;
    QString explosianImage;
    int explosianImageWidth;
    int explosianImageHeight;
    QString objectType;
    bool speedIsSlower;

private:
    //X distance traveled by the game object
    double xDistance;
    //Y distance traveled by the game object
    double yDistance;
};

#endif // GAMEOBJECT_H
