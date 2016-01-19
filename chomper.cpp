#include "chomper.h"
#include "mainwindow.h"
#include "gameobject.h"
#include <QTimer>
#include <QMovie>
#include <QLabel>
#include <QDebug>

int Chomper::cost = 150;

Chomper::Chomper(QObject *) //Default constructor
{
    width = 72;
    height = 71;
    life = 4;
    range = 1;
    damage = 150;
    rate = 42;
    splash = 0;
    slow = 0;
    bomb = 0;
    seeding = 7.5;
    sun = 0;
    need = 0;
    itemCost = 150;
    objectType = "Chomper";
    imageUrl = ":/images/chomper.gif";          //File path of the chomper gif
    imageUrlWidth = 62;                         //Width of the image
    imageUrlHeight = 62;                        //Height of the image
    imageUrlAlt = ":/images/chomper-eating.gif";//File path of the chomper eating gif
    imageUrlAltWidth = 62;                      //Width of the image
    imageUrlAltHeight = 62;                     //Height of the image
    //Setup timer
    multiUseTimer = new QTimer();
    multiUseTimer->setSingleShot(true);
    connect(multiUseTimer,SIGNAL(timeout()),this,SLOT(doneEating()));
    multiUseTimer->stop();
}

Chomper::~Chomper() //Destructor
{}

void Chomper::setupGameObject()
{
    //Create a new QMovie to create the plant gif
    QMovie *plant_gif = new QMovie(imageUrl);
    //Create a new label to hold a gif of the plant
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
}

QRectF Chomper::boundingRect() const
{
    return QRectF(0,0,width,height);
}

void Chomper::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //Check if life is less than or equal to zero
    if (this->getLife() <= 0)
    {
        //Delete the image of the object from the game scene
        delete gameObjectGif;
        gameObjectGif = 0;
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
    //Loop through all zombie objects
    for (int i = 0; i < MainWindow::zombieObjects.size(); i++)
    {
        //If this object collides with a zombie
        if (this->collidesWithItem(MainWindow::zombieObjects.at(i)))
        {
            //Set the plant to eat the zombie if it is not already eating one
            if (!multiUseTimer->isActive())
            {
                //Kill the zombie
                MainWindow::zombieObjects.at(i)->setLife(MainWindow::zombieObjects.at(i)->getLife() - damage);
                //Create a new QMoive object of the plant eating
                QMovie *plant_eating = new QMovie(imageUrlAlt);
                //Set the movie to the already existing label
                gameObjectGif->setMovie(plant_eating);
                //Start the gif
                gameObjectGif->movie()->start();
                //Start a timer for the length of time the chomper is eating
                multiUseTimer->start(rate * 1000);    //Convert rate to milliseconds
            }
        }
    }
    //Set the opacity to 0 making it transparent
    painter->setOpacity(0);
    //Set the size of the chomper object
    painter->drawRect(boundingRect());
}

void Chomper::doneEating()
{
    //Once chomper is done eating, set plant image back to normal
    QMovie *chomper_normal = new QMovie(imageUrl);
    //Set the movie to the already existing label
    gameObjectGif->setMovie(chomper_normal);
    //Start the gif
    gameObjectGif->movie()->start();
}
