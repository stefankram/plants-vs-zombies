#include "sun.h"
#include "mainwindow.h"
#include "body.h"
#include <QDebug>
#include <QString>

Sun::Sun(QObject *) //Constructor
{}

Sun::Sun(QString origin)
{
    width = 35;
    height = 35;
    speed = 1.44;
    life = 99;
    imageUrl = ":/images/sun.png";
    imageUrlWidth = 35;
    imageUrlHeight = 35;
    objectType = origin;    //Set the object type to indicate which part of the program created the sun
    if (objectType == "sunflower")
    {
        //Create a new timer
        multiUseTimer = new QTimer();
        //Set as single shot
        multiUseTimer->setSingleShot(true);
        //Connect the timer to another function that will stop the sun from moving
        connect(multiUseTimer,SIGNAL(timeout()),this,SLOT(stopSunflowerSunMovement()));
        //Start the timer with a random number so it will fall to a random row
        multiUseTimer->start(250);
    }
    else
    {
        //Start timer to drop the sun
        multiUseTimer = new QTimer();
        //Set the timer as a single shot
        multiUseTimer->setSingleShot(true);
        //Connect the timer to the generate sun slot
        connect(multiUseTimer,SIGNAL(timeout()),this,SLOT(generateSun()));
        //Wait for 10 seconds to generate sun
        multiUseTimer->start(10000);
        //Set the visibility of the object to false
        this->setVisible(false);
    }
}

Sun::~Sun()
{}

QRectF Sun::boundingRect() const
{
    return QRectF(0,0,width,height);
}

void Sun::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (this->getLife() <= 0)
    {
        //Remove the sun object from the game scene
        MainWindow::gameScene->removeItem(this);
        for (int i = 0; i < MainWindow::lawnMowerObjects.size(); i++)
        {
            if (this == MainWindow::lawnMowerObjects.at(i))
            {
                MainWindow::lawnMowerObjects.removeAt(i);
                break;
            }
        }
    }
    //Draw picture of the sun object
    painter->drawPixmap(0,0,imageUrlWidth,imageUrlHeight,QPixmap(imageUrl));
    //Make sure sun value is always on top
    this->setZValue(600);
}

void Sun::generateSun()
{
    //Set the visibility of the object to true
    this->setVisible(true);
    //Create a new body object with a downward velocity representing the sun falling
    Body body(MainWindow::columns.at(Body::random(0, (MainWindow::columns.size()-1))).x() + 18.5,134,speed,90);
    //Set the body to the sun object
    this->setBody(body);
    //Add the sun object to the scene
    MainWindow::gameScene->addItem(this);
    //Create a new timer
    multiUseTimer = new QTimer();
    //Set as single shot
    multiUseTimer->setSingleShot(true);
    //Connect the timer to another function that will stop the sun from moving
    connect(multiUseTimer,SIGNAL(timeout()),this,SLOT(stopSunMovement()));
    //Start the timer with a random number so it will fall to a random row
    multiUseTimer->start(Body::random(1000,4000));
}

void Sun::stopSunMovement()
{
    //Stop the sun object from moving
    this->setXVelocity(0);
    this->setYVelocity(0);
    //Create a new timer
    multiUseTimer = new QTimer();
    //Set as single shot
    multiUseTimer->setSingleShot(true);
    //Connect the timer to another function that will delete the sun object after 7.5 seconds
    connect(multiUseTimer,SIGNAL(timeout()),this,SLOT(deleteSun()));
    //Start the timer
    multiUseTimer->start(7500);
}

void Sun::stopSunflowerSunMovement()
{
    //Stop the sun object from moving
    this->setXVelocity(0);
    this->setYVelocity(0);
    //Create a new timer
    multiUseTimer = new QTimer();
    //Set as single shot
    multiUseTimer->setSingleShot(true);
    //Connect the timer to another function that will delete the sun object after 7.5 seconds
    connect(multiUseTimer,SIGNAL(timeout()),this,SLOT(deleteSunflowerSun()));
    //Start the timer
    multiUseTimer->start(7250);
}

void Sun::deleteSun()
{
    //Remove the sun object from the game scene
    MainWindow::gameScene->removeItem(this);
    //Set the visibility of the object to false
    this->setVisible(false);
    //Create a new timer
    multiUseTimer = new QTimer();
    //Set as single shot
    multiUseTimer->setSingleShot(true);
    //Restart the timer by connecting it to the sun generating function
    connect(multiUseTimer,SIGNAL(timeout()),this,SLOT(generateSun()));
    //Start the timer with 10 seconds
    multiUseTimer->start(10000);
}

void Sun::deleteSunflowerSun()
{
    //Remove the sun object from the game scene
    MainWindow::gameScene->removeItem(this);
    //Set the visibility of the object to false
    this->setVisible(false);
}

//If the user clicks on this object
void Sun::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    //Stop any timers that are going
    multiUseTimer->stop();
    //Add 25 to the current sun amount
    MainWindow::currentSunAmount += 25;
    //Remove the sun object from the scene
    MainWindow::gameScene->removeItem(this);
    //Set the visibility of the sun object to false
    this->setVisible(false);
    //If the sun was not generated be the sunflower, restart the timer
    if (objectType != "sunflower")
    {
        //Create a new timer
        multiUseTimer = new QTimer();
        //Set as single shot
        multiUseTimer->setSingleShot(true);
        //Restart the timer by connecting it to the sun generating function
        connect(multiUseTimer,SIGNAL(timeout()),this,SLOT(generateSun()));
        //Start the timer with 10 seconds
        multiUseTimer->start(10000);
    }
}
