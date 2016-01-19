#ifndef BODY_H
#define BODY_H

class Body
{
private:
    //X coordinate of the current body object
    int xCoordinate;
    //Y coordinate of the current body object
    int yCoordinate;
    //Velocity of the current body object
    double velocity;
    //Direction of the current body object
    double direction;

public:
    //Default constructor
    Body();
    //Overloaded constructor
    Body(int x, int y, double velocity, double direction);
    //Set the position of the body object
    void setPos(int xCoordinate, int yCoordinate);
    //Set the velocity of the body object in px/s
    void setVelocity(double velocity);
    //Set the direction of the body object in degress
    void setDirection(double direction);
    //Move the object according to velocity and direction
    void move(double time);
    //Get a random number between the two numbers
    static double random(double small, double large);
    //Get the x coordinate of the current object
    int x() const;
    //Get the y coordinate of the current object
    int y() const;
    //Get the velocity of the current object
    double getVelocity() const;
    //Get the direction of the current object
    double getDirection() const;
};

#endif // BODY_H
