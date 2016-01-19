#include "body.h"
#include <QtMath>
#include <QtGlobal>
#include <QTime>

#define PI 3.14159265359    //Define PI

Body::Body()    //Default Constructor
{}

Body::Body(int x, int y, double velocity, double direction) :   //Overloaded constructor
    xCoordinate(x),
    yCoordinate(y),
    velocity(velocity),
    direction(direction)
{}

void Body::setPos(int xCoordinate, int yCoordinate) //Set position of the body object
{
    this->xCoordinate = xCoordinate;
    this->yCoordinate = yCoordinate;
}

void Body::setVelocity(double velocity) //Set the velocity of the body object
{
    this->velocity = velocity;
}

void Body::setDirection(double direction)   //Set the direction of the body object
{
    this->direction = direction;
}

void Body::move(double time)    //Move the body object
{
    //Get distance traveled based on velocity and time
    double distance = velocity * time;
    //Find the new x coordinate
    double x_value = distance * qCos(direction * (PI / 180));
    //Add the new x coordinate to the old x coordinate
    if (x_value < 0)
    {
        xCoordinate += static_cast<int>(x_value - 0.5);
    }
    else
    {
        xCoordinate += static_cast<int>(x_value + 0.5);
    }
    //Find the new y coordinate
    double y_value = distance * qSin(direction * (PI / 180));
    //Add the new y coordinate to the old y coordinate
    if (y_value < 0)
    {
        yCoordinate += static_cast<int>(y_value - 0.5);
    }
    else
    {
        yCoordinate += static_cast<int>(y_value + 0.5);
    }
}

double Body::random(double small, double large) //Random number generator
{
    qrand(); //Throw out first set of random numbers
    //Get a random decimal number between 0 and 1
    double random_number = qrand() / (static_cast<double>(RAND_MAX) + 1);
    //Convert the deciaml number to the range x1 to x2
    return small + (large - small) * random_number;
}

int Body::x() const //Get the x coordinate
{
    return xCoordinate;
}

int Body::y() const //Get the y coordinate
{
    return yCoordinate;
}

double Body::getVelocity() const    //Get the velocity
{
    return velocity;
}

double Body::getDirection() const   //Get the direction
{
    return direction;
}
