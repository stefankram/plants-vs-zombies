#include "bucketzombie.h"
#include "mainwindow.h"
#include "gameobject.h"
#include <QLabel>
#include <QTimer>
#include <QMovie>
#include <QDebug>

BucketZombie::BucketZombie(QObject *)    //Default constructor
{
    width = 36;
    height = 62;
    life = 10;
    armour = 55;
    attack = 1;
    rate = 0.5;
    speed = 0.288;
    speedIsSlower = false;
    objectType = "BucketZombie";
    imageUrl = ":/images/bucket-head-zombie.gif";   //File location of buckert zombie gif
    imageUrlWidth = 36;                             //Image width
    imageUrlHeight = 62;                            //Image height
    //Create a new label to place the zombie gif
    gameObjectGif = new QLabel();
    multiUseTimer = new QTimer();
}

BucketZombie::~BucketZombie()   //Destructor
{}

void BucketZombie::setupGameObject()    //Setup and initialize the game object
{
    //Set the label to have a transparent background
    gameObjectGif->setAttribute(Qt::WA_TranslucentBackground);
    //Create a new qmovie with the zombie gif
    QMovie *zombie_gif = new QMovie(imageUrl);
    //Set the position and size of the zombie gif
    gameObjectGif->setGeometry(this->scenePos().x()+5,this->scenePos().y()+5,imageUrlWidth,imageUrlHeight);
    //Set the zombie gif to the label
    gameObjectGif->setMovie(zombie_gif);
    //Start the gif
    gameObjectGif->movie()->start();
    //Add the label to the game scene
    MainWindow::gameScene->addWidget(gameObjectGif);

}

QRectF BucketZombie::boundingRect() const
{
    return QRectF(0,0,width,height);
}

void BucketZombie::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //Loop through all plant zombies
    for (int i = 0; i < MainWindow::plantObjects.size(); i++)
    {
        //If the zombie collides with a plant
        if (this->collidesWithItem(MainWindow::plantObjects.at(i)) && !multiUseTimer->isActive())
        {
            //Subtract health from plant object
            MainWindow::plantObjects.at(i)->setLife(MainWindow::plantObjects.at(i)->getLife() - attack);
            //Set as single shot
            multiUseTimer->setSingleShot(true);
            //Connect to attack function
            connect(multiUseTimer,SIGNAL(timeout()),this,SLOT(changeDirection()));
            //Start the timer to attack the zombie
            multiUseTimer->start(rate * 1000);
            break;
        }
    }
    //Set the position of the zombie gif to match the position of the game object
    gameObjectGif->setGeometry(this->scenePos().x()+5,this->scenePos().y()+5,imageUrlWidth,imageUrlHeight);
    //Check if the zombies health is less than or equal to zero
    if (this->getLife() <= 0)
    {
        //Remove the zombie gif from the game scene
        delete gameObjectGif;
        gameObjectGif = 0;
        //Remove the zombie object from the game scene
        MainWindow::gameScene->removeItem(this);
        //Remove the zombie object from the list of zombie objects
        for (int i = 0; i < MainWindow::zombieObjects.size(); i++)
        {
            if (this == MainWindow::zombieObjects.at(i))
            {
                MainWindow::zombieObjects.removeAt(i);
                break;
            }
        }
    }
    //Set z value
    this->setZValue(401);
    //Set the game object to be transparent
    painter->setOpacity(0);
    //Set the size of the game object
    painter->drawRect(boundingRect());
}

void BucketZombie::attackPlant()
{
    this->setXVelocity(speed);
}

void BucketZombie::changeDirection()
{
    this->setPos(this->scenePos().x() + 2, this->scenePos().y());
}
