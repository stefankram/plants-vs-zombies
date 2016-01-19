#include "gameobject.h"
#include "mainwindow.h"
#include <QtMath>

#define PI 3.14159265359

GameObject::GameObject() :  //Default constructor
    xDistance(0),
    yDistance(0)
{}

GameObject::~GameObject()   //Destructor
{}

void GameObject::setBody(Body b)    //Set body of the game object
{
    body = b;
    //Get the x and y distance traveled
    xDistance = body.getVelocity() * qCos(body.getDirection() * (PI/180));
    yDistance = body.getVelocity() * qSin(body.getDirection() * (PI/180));
    //Set the position of the game object on the graphics view
    this->setPos(body.x(),body.y());
}

void GameObject::move(double time)  //Move the object
{
    body.move(time);
}

void GameObject::advance(int phase) //Set the position of the object
{
    //Don't do anything if the phase is 0
    if (!phase) return;
    //Get the current position
    QPointF p = this->pos();
    //Add the distance traveled to the current position
    setPos(p.x()+xDistance, p.y()+yDistance);
}

//Various get functions
int GameObject::getArmour()
{
    return armour;
}
int GameObject::getAttack()
{
    return attack;
}
int GameObject::getBomb()
{
    return bomb;
}
int GameObject::getCost()
{
    return itemCost;
}
int GameObject::getDamage()
{
    return damage;
}
int GameObject::getHeight()
{
    return height;
}
int GameObject::getLife()
{
    return life;
}
int GameObject::getNeed()
{
    return need;
}
int GameObject::getRange()
{
    return range;
}
int GameObject::getSlow()
{
    return slow;
}
float GameObject::getRate()
{
    return rate;
}
float GameObject::getSeeding()
{
    return seeding;
}
int GameObject::getSplash()
{
    return splash;
}
float GameObject::getSpeed()
{
    return speed;
}
int GameObject::getSun()
{
    return sun;
}
int GameObject::getWidth()
{
    return width;
}
QString GameObject::getImageUrl()
{
    return imageUrl;
}
int GameObject::getImageUrlWidth()
{
    return imageUrlWidth;
}
int GameObject::getImageUrlHeight()
{
    return imageUrlHeight;
}
QString GameObject::getImageUrlAlt()
{
    return imageUrlAlt;
}
int GameObject::getImageUrlAltWidth()
{
    return imageUrlAltWidth;
}
int GameObject::getImageUrlAltHeight()
{
    return imageUrlAltHeight;
}
QString GameObject::getObjectType()
{
    return objectType;
}
bool GameObject::isSlower()
{
    return speedIsSlower;
}

//Varoius set functions
void GameObject::setLife(int life)
{
    this->life = life;
}
void GameObject::setXVelocity(double vx)
{
    xDistance = vx;
}
void GameObject::setYVelocity(double vy)
{
    yDistance = vy;
}
void GameObject::setSpeed(float s)
{
    speed = s;
}
void GameObject::setArmour(int armour)
{
    this->armour = armour;
}
void GameObject::setDamage(int damage)
{
    this->damage = damage;
}
void GameObject::setSlower(bool b)
{
    speedIsSlower = b;
}
