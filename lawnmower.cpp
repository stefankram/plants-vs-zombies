#include "lawnmower.h"
#include "mainwindow.h"
#include <QMovie>
#include <QLabel>

LawnMower::LawnMower()  //Default constructor
{
    //Game object variables
    width = 62;
    height = 62;
    life = 99;
    imageUrl = ":/images/lawn-mower.png";
    imageUrlWidth = 62;
    imageUrlHeight = 53;
    //Private variables
    collidedWithZombie = false;
}

LawnMower::~LawnMower() //Destructor
{}

QRectF LawnMower::boundingRect() const
{
    return QRectF(0,0,width,height);
}

void LawnMower::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //Loop through all zombie objects
    for (int i = 0; i < MainWindow::zombieObjects.size(); i++)
    {
        //Check if the lawn mower collides with a zombie
        if (this->collidesWithItem(MainWindow::zombieObjects.at(i)))
        {
            //On the first collision, change the velocity of the lawn mower
            if (collidedWithZombie == false)
            {
                //Create a new body with a positive velocity
                Body body(this->scenePos().x(),this->scenePos().y() + 9.5,2,0);
                //Set the body to the current object
                this->setBody(body);
                //Set collidedWithZombie to true signifying the first zombie has been hit
                collidedWithZombie = true;
            }
            //Delete the zombie from the game scene
            MainWindow::zombieObjects.at(i)->setLife(0);
        }
    }
    //Check if the lawn mower leaves the bounds of the game scene
    if (this->scenePos().x() > MainWindow::GAME_WIDTH)
    {
        //Remove the lawn mower from the game scene
        MainWindow::gameScene->removeItem(this);
        //Remove the lawn mower from the lawn mower list
        for (int j = 0; j < MainWindow::bulletObjects.size(); j++)
        {
            if (this == MainWindow::bulletObjects.at(j))
            {
                MainWindow::bulletObjects.removeAt(j);
                break;
            }
        }
    }
    if (this->getLife() <= 0)
    {
        //Remove the lawn mower from the game scene
        MainWindow::gameScene->removeItem(this);
        //Remove the lawn mower from the lawn mower list
        for (int j = 0; j < MainWindow::bulletObjects.size(); j++)
        {
            if (this == MainWindow::bulletObjects.at(j))
            {
                MainWindow::bulletObjects.removeAt(j);
                break;
            }
        }
    }
    //Draw the picture of the lawn mower
    painter->drawPixmap(QPoint(0,0),QPixmap(imageUrl));
}
