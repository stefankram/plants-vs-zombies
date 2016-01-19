#include "cherrybomb.h"
#include "mainwindow.h"
#include "gameobject.h"
#include <QTimer>
#include <QMovie>
#include <QLabel>
#include <QDebug>

int CherryBomb::cost = 150;

CherryBomb::CherryBomb(QObject *)   //Default constructor
{
    //Set up plant variables
    width = 72;
    height = 72;
    life = 9999;
    range = 1;
    damage = 90;
    rate = 1;
    splash = 3;
    slow = 0;
    bomb = 1;
    seeding = 50;
    sun = 0;
    need = 0;
    itemCost = 150;
    objectType = "CherryBomb";
    imageUrl = ":/images/cherry-bomb.gif";          //File path of the cherry bomb gif
    imageUrlWidth = 62;                             //Width of the image
    imageUrlHeight = 46;                            //Height of the image
    explosianImage = ":/images/cherry-bomb-explosian"; //File path of the explosian gif
    explosianImageWidth = 114;                         //Width of the image
    explosianImageHeight = 158;                        //Height of the image
    //Set up timer
    multiUseTimer = new QTimer();
    multiUseTimer->stop();
}

CherryBomb::~CherryBomb()   //Destructor
{}

void CherryBomb::setupGameObject()   //Setup and initialize the game object
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
    //Create a new timer that will make the cherry bomb explode after 2 seconds
    multiUseTimer = new QTimer();
    multiUseTimer->setSingleShot(true);
    connect(multiUseTimer,SIGNAL(timeout()),this,SLOT(explodeCherryBomb()));
    multiUseTimer->start(2000);
}

QRectF CherryBomb::boundingRect() const
{
    return QRectF(0,0,width,height);
}

void CherryBomb::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //Set the opacity to 0 making it transparent
    painter->setOpacity(0);
    //Set the size of the cherry bomb object
    painter->drawRect(boundingRect());
}

void CherryBomb::explodeCherryBomb()
{
    //Put in explosian
    QMovie *explosian_gif = new QMovie(explosianImage);
    //Set the movie as the current plant image
    gameObjectGif->setMovie(explosian_gif);
    //Set the geometry to match the new image
    gameObjectGif->setGeometry(x()-21,y()-43,114,158);
    //Start the new gif
    gameObjectGif->movie()->start();
    //Subtract health from zombies in the surrounding area
    for (int i = 0; i < MainWindow::zombieObjects.size(); i++)
    {
        qDebug() << x() << "," << y();
        if (MainWindow::zombieObjects.at(i)->x() >= (x() - 73) &&
                MainWindow::zombieObjects.at(i)->x() <= (x() + 144) &&
                MainWindow::zombieObjects.at(i)->y() >= (y() - 73) &&
                MainWindow::zombieObjects.at(i)->y() <= (y() + 144))
        {
            MainWindow::zombieObjects.at(i)->setLife(MainWindow::zombieObjects.at(i)->getLife() - damage);
        }
    }
    //Create a new timer to destroy the cherry bomb when the explosian finishes
    multiUseTimer = new QTimer();
    multiUseTimer->setSingleShot(true);
    connect(multiUseTimer,SIGNAL(timeout()),this,SLOT(destroyCherryBomb()));
    multiUseTimer->start(800);
}

void CherryBomb::destroyCherryBomb()
{
    //Remove the gif from the game scene
    delete gameObjectGif;
    gameObjectGif = 0;
    //Remove plant from the game scene
    MainWindow::gameScene->removeItem(this);
    //Remove the plant from the plants list
    for (int i = 0; i < MainWindow::plantObjects.size(); i++)
    {
        if (this == MainWindow::plantObjects.at(i))
        {
            MainWindow::plantObjects.removeAt(i);
            break;
        }
    }
}
