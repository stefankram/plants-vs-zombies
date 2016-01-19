#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QStringList>
#include "gameobject.h"
#include <QList>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QMovie>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //Constructor
    explicit MainWindow(QWidget *parent = 0);
    //Destructor
    ~MainWindow();
    //Size of the game
    enum {GAME_WIDTH = 744, GAME_HEIGHT = 462};
    //List of Game objects
    static QList<GameObject *> zombieObjects;   //List of all zombie objects
    static QList<GameObject *> plantObjects;    //List of all plant objects
    static QList<GameObject *> bulletObjects;   //List of all bullet objects
    static QList<GameObject *> lawnMowerObjects;//List of all lawn mower objects
    //Amount of sun the user has
    static int currentSunAmount;
    //Current mouse coordinates of user's mouse
    static QPointF mouseCoordinates;
    static QPointF mousePressCoordinates;
    //QGraphicsScene to hold all QGraphicItems
    static QGraphicsScene *gameScene;
    //Coordinates of the top left corner of each row
    static QList<QPointF> rows;
    //Coordinates of the top left corner of each column
    static QList<QPointF> columns;

private slots:
    //Button Slots - Menu
    void on_startButton_clicked();
    void on_quitButton_clicked();
    void on_newButton_clicked();
    void on_deleteButton_clicked();
    //Button Slots - Gameplay
    void peaShooterButtonClicked();
    void sunflowerButtonClicked();
    void cherryBombButtonClicked();
    void wallnutButtonClicked();
    void potatoMineButtonClicked();
    void snowPeaButtonClicked();
    void chomperButtonClicked();
    void repeaterButtonClicked();
    void menuButtonClicked();
    //Button Slots - Placement
    void peaShooterPlacement();
    void sunflowerPlacement();
    void cherryBombPlacement();
    void wallnutPlacement();
    void potatoMinePlacement();
    void snowPeaPlacement();
    void chomperPlacement();
    void repeaterPlacement();
    //Timer Slots
    void zombieGenerator();     //Generate zombies for current level
    void refreshGameObjects();  //Refresh game objects

    void on_resumeButton_clicked();

    void on_restartButton_clicked();

private:
    //Button timers for seeding
    QTimer *peaShooterButtonTimer;
    QTimer *sunflowerButtonTimer;
    QTimer *cherryBombButtonTimer;
    QTimer *wallnutButtonTimer;
    QTimer *potatoMineButtonTimer;
    QTimer *snowPeaButtonTimer;
    QTimer *chomperButtonTimer;
    QTimer *repeaterButtonTimer;

    //Private functions
    void readLevelFile();           //Read data from the level file
    void readPlayerFile();          //Read data from the user file
    void initialSetup();            //Initial setup of the game
    void levelGenerator();          //Generate content for the current level
    void showMainMenu();            //Display main menu
    void showPauseMenu();           //Display the pause menu
    void hideMenu();                //Hide menu
    void enableGameplayButtons();   //Enable gameplay buttons
    void disableGameplayButtons();  //Disable gameplay buttons
    void disablePlantButtons();     //Disable only the plant buttons
    void plantGenerator(GameObject *plant); //Generate a plant object given the GameObject parameters
    void enablePlantButtons();      //Enable only the plant buttons
    void updateMenu();              //Update items on the menu
    void createGameplayUi();        //Setup timers, buttons, pictures, labels of the UI
    void setupGameScene();          //Add buttons, pictures, labels to the game scene
    void setDirtRow(int x, int y); //Set the dirt rows
    void setGrassRow(int x, int y, int type); //Set the grass rows
    void setLawnMower(int x, int y);    //Set the lawn mower at the x and y coordinates
    void deleteLevelData();

    //Private members
    Ui::MainWindow *ui;             //The UI
    QStringList levelData;          //List to hold level data found in the level file
    QStringList playerData;         //List to hold player data found in the player file
    QString currentLevel;           //Current level data
    QString currentUser;            //Current user data
    int zombieCounter;              //Number of zombies placed on the scene
    QList<int> zombieStartRows;     //Y coordinate of the available rows
    //Timers
    QTimer *gameTimer;              //Timer to move objects on timeout
    QTimer *zombieTimer;            //Timer to display the next zombie on timeout
    QTimer *refreshTimer;           //Timer to refresh objects in the game scene
    //Button Timers
    QTimer *plantTimer;             //Timer for placement of plant objects
    //Variables for current level
    QString levelNumber;            //Current level number
    QStringList zombieSequence;     //Current zombie sequence
    QString availableRows;          //Available rows for this level
    int startTime;                  //Time till the first zombie appears
    int startingInterval;           //Time till the next zombie appears
    int intermediateInterval;       //Time till next zombie appears after decrement
    float intervalDecrement;        //Value to decrement from starting time
    int availableRowsBottomY;       //Bottom Y values of the available rows
    int availableRowsTopY;          //Top Y value of the available rows
    //UI buttons
    QPushButton *peaShooterButton;
    QPushButton *sunflowerButton;
    QPushButton *cherryBombButton;
    QPushButton *wallnutButton;
    QPushButton *potatoMineButton;
    QPushButton *snowPeaButton;
    QPushButton *chomperButton;
    QPushButton *repeaterButton;
    QPushButton *menuButton;
    //UI Labels
    QLabel *sunValue;
    //UI Images
    QLabel *sunImage;
    //Labels to hold the hover image of each plant
    QLabel *plantHover;
};

#endif // MAINWINDOW_H
