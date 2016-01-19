#include "potatomine.h"
#include "mainwindow.h"
#include "gameobject.h"
#include <QTimer>
#include <QMovie>
#include <QLabel>
#include <QDebug>

int PotatoMine::cost = 25;

PotatoMine::PotatoMine()
{
    width = 72;
    height = 71;
    life = 4;
    range = 1;
    damage = 90;
    rate = 15;
    splash = 0;
    slow = 0;
    bomb = 1;
    seeding = 30;
    sun = 0;
    need = 0;
    itemCost = 25;
    objectType = "PotatoMine";
    imageUrl = ":/images/potato-mine-inactive.gif"; //File path of the inactive potato mine gif
    imageUrlWidth = 62;                             //Image width
    imageUrlHeight = 46;                            //Image height
    imageUrlAlt = ":/images/potato-mine-active.gif";//File path of the active potato mine gif
    imageUrlAltWidth = 62;                          //Image width
    imageUrlAltHeight = 46;                         //Image height
    explosianImage = ":/images/cherry-bomb-explosian"; //File path of the explosian gif
    explosianImageWidth = 114;                         //Width of the image
    explosianImageHeight = 158;                        //Height of the image
    //Setup timer
    multiUseTimer = new QTimer();
    multiUseTimer->setSingleShot(true);
    connect(multiUseTimer,SIGNAL(timeout()),this,SLOT(activateMine()));
    multiUseTimer->start(rate * 1000);  //Convert seconds to milliseconds
}

PotatoMine::~PotatoMine()   //Destructor
{}

void PotatoMine::setupGameObject()
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

QRectF PotatoMine::boundingRect() const
{
    return QRectF(0,0,width,height);
}

void PotatoMine::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
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
    //Check if the timer is still running
    if (!multiUseTimer->isActive())
    {
        //Check if a zombie collides with the mine
        for (int i = 0; i < MainWindow::zombieObjects.size(); i++)
        {
            if (this->collidesWithItem(MainWindow::zombieObjects.at(i)))
            {
                //Create a new QMovie of the explosian
                QMovie *activated_mine = new QMovie(explosianImage);
                //Replace the label with the new gif
                gameObjectGif->setMovie(activated_mine);
                //Set the label geometry to match the new image
                gameObjectGif->setGeometry(x()-21,y()-43,114,158);
                //Start the movie
                gameObjectGif->movie()->start();
                //Create a new timer
                multiUseTimer = new QTimer();
                multiUseTimer->setSingleShot(true);
                //Connect the timer to delete this game object after explosian finishes
                connect(multiUseTimer,SIGNAL(timeout()),this,SLOT(explodeMine()));
                //Start the timer
                multiUseTimer->start(800);
                //Lower the life of the zombie
                MainWindow::zombieObjects.at(i)->setLife(MainWindow::zombieObjects.at(i)->getLife() - damage);
                break;
            }
        }
    }
    //Set the opacity to 0 making it transparent
    painter->setOpacity(0);
    //Set the size of the cherry bomb object
    painter->drawRect(boundingRect());
}

void PotatoMine::activateMine()
{
    //Create a new QMovie of the activated mine
    QMovie *activated_mine = new QMovie(imageUrlAlt);
    //Replace the label with the new gif
    gameObjectGif->setMovie(activated_mine);
    //Start the movie
    gameObjectGif->movie()->start();
}

void PotatoMine::explodeMine()
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
