#include "sunflower.h"
#include "mainwindow.h"
#include "gameobject.h"
#include "sun.h"
#include "body.h"
#include <QTimer>
#include <QMovie>
#include <QLabel>

int SunFlower::cost = 50;

SunFlower::SunFlower(QObject *)  //Default constructor
{
    width = 72;
    height = 71;
    life = 4;
    range = 0;
    damage = 0;
    rate = 24;
    splash = 0;
    slow = 0;
    bomb = 0;
    seeding = 7.5;
    sun = 1;
    need = 0;
    itemCost = 50;
    imageUrl = ":/images/sunflower.gif";
    imageUrlWidth = 62;
    imageUrlHeight = 62;
    imageUrlAlt = ":/images/sun.png";
    imageUrlAltWidth = 35;
    imageUrlAltHeight = 35;
    //Setup timer to produce sun
    multiUseTimer = new QTimer();
    //Connect it to the display sun function
    connect(multiUseTimer,SIGNAL(timeout()),this,SLOT(displaySun()));
    //Start the timer
    multiUseTimer->start(rate * 1000);    //Convert seconds to milliseconds
}

SunFlower::~SunFlower() //Destructor
{}

void SunFlower::setupGameObject()
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

QRectF SunFlower::boundingRect() const
{
    return QRectF(0,0,width,height);
}

void SunFlower::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
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
    //Set the opacity to 0 making it transparent
    painter->setOpacity(0);
    //Set the size of the chomper object
    painter->drawRect(boundingRect());
}

void SunFlower::displaySun()
{
    //Stop timer
    multiUseTimer->stop();
    //Create a new sun object
    GameObject *sun = new Sun("sunflower");
    //Create the body of the sun object
    Body body(this->x() + 5,this->y(),sun->getSpeed(),90);
    //Set the body of the sun object
    sun->setBody(body);
    //Add the sun object to the scene
    MainWindow::gameScene->addItem(sun);
    //Start timer with a new time
    multiUseTimer->start((rate + 7.5) * 1000);
}
