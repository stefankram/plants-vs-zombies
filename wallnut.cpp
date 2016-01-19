#include "wallnut.h"
#include "mainwindow.h"
#include "gameobject.h"
#include <QLabel>
#include <QMovie>

int WallNut::cost = 50;

WallNut::WallNut()  //Default constructor
{
    width = 72;
    height = 71;
    life = 72;
    range = 0;
    damage = 0;
    rate = 0;
    splash = 0;
    slow = 0;
    bomb = 0;
    seeding = 30;
    sun = 0;
    need = 0;
    itemCost = 50;
    imageUrl = ":/images/wallnut.gif";
    imageUrlWidth = 56;
    imageUrlHeight = 62;
}

WallNut::~WallNut() //Destructor
{}

void WallNut::setupGameObject()
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

QRectF WallNut::boundingRect() const
{
    return QRectF(0,0,width,height);
}

void WallNut::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
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
