#include "peashooterbullet.h"
#include "mainwindow.h"
#include "gameobject.h"

PeaShooterBullet::PeaShooterBullet()    //Default constructor
{
    damage = 1;
    rate = 1.92;
    life = 99;
    imageUrl = ":/images/pea-shooter-bullet.png";   //File location of the pea shooter bullet
    imageUrlWidth = 26;                         //Image width
    imageUrlHeight = 26;                        //Image height
}

QRectF PeaShooterBullet::boundingRect() const
{
    return QRectF(0,0,imageUrlWidth,imageUrlHeight);
}

void PeaShooterBullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (this->getLife() <= 0)
    {
        //Remove the bullet from the game scene
        MainWindow::gameScene->removeItem(this);
        //Remove the bullet from the bullet list
        for (int j = 0; j < MainWindow::bulletObjects.size(); j++)
        {
            if (this == MainWindow::bulletObjects.at(j))
            {
                MainWindow::bulletObjects.removeAt(j);
                break;
            }
        }
    }
    //Loop through all the zombie objects
    for (int i = 0; i < MainWindow::zombieObjects.size(); i++)
    {
        //Check if the bullet collides with one of the zombie objects
        if (this->collidesWithItem(MainWindow::zombieObjects.at(i)))
        {
            //Decrease the life of the zombie by 1
            MainWindow::zombieObjects.at(i)->setLife(MainWindow::zombieObjects.at(i)->getLife() - damage);
            //Remove the bullet from the game scene
            MainWindow::gameScene->removeItem(this);
            //Remove the bullet from the bullet list
            for (int j = 0; j < MainWindow::bulletObjects.size(); j++)
            {
                if (this == MainWindow::bulletObjects.at(j))
                {
                    MainWindow::bulletObjects.removeAt(j);
                    break;
                }
            }
            break;
        }
    }
    //Check if the bullet leaves the bounds of the graphics view
    if (this->scenePos().x() > MainWindow::GAME_WIDTH)
    {
        //Remove the bullet from the game scene
        MainWindow::gameScene->removeItem(this);
        //Remove the bullet from the bullet list
        for (int j = 0; j < MainWindow::bulletObjects.size(); j++)
        {
            if (this == MainWindow::bulletObjects.at(j))
            {
                MainWindow::bulletObjects.removeAt(j);
                break;
            }
        }
    }
    //Draw the picture of the bullet
    painter->drawPixmap(QPoint(0,0),QPixmap(imageUrl));
    //Set the z value so the bullet goes over other objects
    this->setZValue(500);
}
