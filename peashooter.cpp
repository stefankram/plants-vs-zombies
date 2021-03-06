#include "peashooter.h"
#include "mainwindow.h"
#include "gameobject.h"
#include "peashooterbullet.h"
#include "body.h"
#include <QTimer>
#include <QMovie>
#include <QLabel>
#include <QDebug>

int PeaShooter::cost = 100;

PeaShooter::PeaShooter(QObject *)   //Default constructor
{
    width = 72;
    height = 71;
    life = 4;
    range = 9999;
    damage = 1;
    rate = 1.5;
    splash = 0;
    slow = 0;
    bomb = 0;
    seeding = 7.5;
    sun = 0;
    need = 0;
    itemCost = 100;
    objectType = "PeaShooter";
    imageUrl = ":/images/pea-shooter.gif";  //File path of the pea shooter gif
    imageUrlWidth = 62;                     //Image width
    imageUrlHeight = 62;                    //Image height
}

PeaShooter::~PeaShooter()   //Destructor
{}

void PeaShooter::setupGameObject()
{
    //Create a new QMovie to hold the plant gif
    QMovie *plant_gif = new QMovie(imageUrl);
    //Create a new label to hold the plant gif
    gameObjectGif = new QLabel();
    //Set the label to have a transparent background
    gameObjectGif->setAttribute(Qt::WA_TranslucentBackground);
    //Set the movie to the label
    gameObjectGif->setMovie(plant_gif);
    //Set the geometry of the label
    gameObjectGif->setGeometry((72-imageUrlWidth)/2 + scenePos().x(),(72-imageUrlHeight)/2+ scenePos().y(), imageUrlWidth, imageUrlHeight);
    //Start the movie
    gameObjectGif->movie()->start();
    //Add the QLabel to the game scene
    MainWindow::gameScene->addWidget(gameObjectGif);
    //Setup Timer
    multiUseTimer = new QTimer();
    connect(multiUseTimer,SIGNAL(timeout()),this,SLOT(fireBullet()));
    multiUseTimer->start(this->getRate() * 1000);
}

QRectF PeaShooter::boundingRect() const
{
    return QRectF(0,0,width,height);
}

void PeaShooter::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //Check if the life is less than or equal to zero
    if (this->getLife() <= 0)
    {
        //Delete the image of the object from the game scene
        delete gameObjectGif;
        gameObjectGif = 0;
        //Stop the bullet timer
        multiUseTimer->stop();
        //Remove the object from the game scene
        MainWindow::gameScene->removeItem(this);
        //Remove the object from the plant list
        for (int i = 0; i < MainWindow::plantObjects.size(); i++)
        {
            if (this == MainWindow::plantObjects.at(i))
            {
                MainWindow::plantObjects.removeAt(i);
                break;
            }
        }
    }
    //Set the opacity to 0 making it transparent
    painter->setOpacity(0);
    //Set the size of the chomper object
    painter->drawRect(boundingRect());
}

void PeaShooter::fireBullet()
{
    //Loop through all zombie objects
    for (int i = 0; i < MainWindow::zombieObjects.size(); i++)
    {
        //Check if the zombie is in range of the pea shooter
        if ((this->scenePos().x() < MainWindow::zombieObjects.at(i)->scenePos().x()) &&
                (this->scenePos().y()-72 <= MainWindow::zombieObjects.at(i)->scenePos().y()) &&
                (this->scenePos().y() >= MainWindow::zombieObjects.at(i)->scenePos().y()))
        {
            //Create a new bullet
            GameObject *bullet = new PeaShooterBullet();
            //Create a new body object with the bullet attributes
            Body body(this->scenePos().x()+20,this->scenePos().y()+20,bullet->getRate(),0);
            //Set the body to the bullet object
            bullet->setBody(body);
            //Add the bullet to the game scene
            MainWindow::gameScene->addItem(bullet);
            //Add the bullet to the bullet list
            MainWindow::bulletObjects.append(bullet);
            //Break out of loop so that the bullet is only fired once
            break;
        }
    }
}
