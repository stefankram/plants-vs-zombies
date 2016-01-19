#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QString>
#include <QApplication>
#include <QStandardItemModel>
#include <QTimer>
#include <QList>
#include "body.h"
#include "gameobject.h"
#include "bucketzombie.h"
#include "cherrybomb.h"
#include "chomper.h"
#include "coneheadzombie.h"
#include "flagzombie.h"
#include "newspaperzombie.h"
#include "normalzombie.h"
#include "peashooter.h"
#include "potatomine.h"
#include "repeater.h"
#include "snowpea.h"
#include "sunflower.h"
#include "wallnut.h"
#include "gamescene.h"
#include "sun.h"
#include <QMovie>
#include <QDateTime>
#include "lawnmower.h"
#include <QProcess>

//All plant objects on the game scene
QList<GameObject *> MainWindow::plantObjects;
//All zombie objects on the game scene
QList<GameObject *> MainWindow::zombieObjects;
//All bullet objects on the game scene
QList<GameObject *> MainWindow::bulletObjects;
//All Lawn mower objects
QList<GameObject *> MainWindow::lawnMowerObjects;
//The current sun amount
int MainWindow::currentSunAmount = 0;
//The current mouse coordinates
QPointF MainWindow::mouseCoordinates;
//The last pressed mouse coordinates
QPointF MainWindow::mousePressCoordinates;
//The graphics scene where all game objects are placed
QGraphicsScene *MainWindow::gameScene;
//Coordinates of the top left corner of each row
QList<QPointF> MainWindow::rows;
//Coordinates of the top left corner of each column
QList<QPointF> MainWindow::columns;

//Constructor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    levelData(),
    playerData(),
    currentLevel(""),
    currentUser(""),
    zombieCounter(0)
{
    //Set rows 1 to 5
    for (int i = 0; i < 5; i++)
    {
        rows.append(QPointF(72, (82 + (72 * i))));
    }
    //Set columns 1 to 9
    for (int i = 0; i < 9; i++)
    {
        columns.append(QPointF((72 + (72 * i)),82));
    }
    //Read level data
    readLevelFile();
    //Read player data
    readPlayerFile();
    //Initalize UI
    initialSetup();
}

void MainWindow::initialSetup()
{
    //Initialize and display the UI
    ui->setupUi(this);
    //Enable mouse tracking
    ui->gameView->setMouseTracking(true);
    //Hide resume button since game has not started yet
    ui->resumeButton->hide();

    //Set up the menu
    updateMenu();

    //Setup gameplay buttons
    createGameplayUi();

    //Create a new scene for the graphics scene
    gameScene = new GameScene();

    //Setup the game scene
    setupGameScene();

    //Set the initial sun amount
    currentSunAmount = 1000;
}

/* READ LEVEL DATA */
void MainWindow::readLevelFile()
{
    //Get the csv file containing the level data
    QFile level_file(QString("%1/pvz_levels.csv").arg(QDir::homePath()));
    //Try to open the file...
    if (level_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //Grab each line of text from the file
        while (!level_file.atEnd())
        {
            levelData.append(level_file.readLine().trimmed());
        }
        level_file.close();
        //Check if no data was read from the file
        if (levelData.isEmpty())
        {
            //Display error message and exit program
            QMessageBox file_empty_error;
            file_empty_error.critical(0,"Error","The file \"psv_levels.csv\" is empty");
            exit(0);
        }
        //Loop through each line in the file
        for (int i = 0; i < levelData.size(); i++)
        {
            //Check each line is in the correct format
            if (levelData.at(i).count(':') != 5)
            {
                //Display error message and exit program
                QMessageBox file_parse_error;
                file_parse_error.critical(0,"Error","The file \"psv_levels.csv\" is unreadable");
                exit(0);
            }
        }
    }
    else
    {
        //Display error message saying the file does not exist or cannot be accessed
        QMessageBox file_get_error;
        file_get_error.critical(0,"Error","The file \"psv_levels.csv\" does not exist or cannot be accessed");
        exit(0);
    }
}

/* READ USER DATA */
void MainWindow::readPlayerFile()
{
    //Get the csv file containing the user data
    QFile player_file(QString("%1/pvz_players.csv").arg(QDir::homePath()));
    //Try to open the file
    if (player_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //Grab each line from the file
        while (!player_file.atEnd())
        {
                playerData.append(player_file.readLine().trimmed());
        }
        player_file.close();
        //Remove whitespaces
        int index = 0;
        while (index < playerData.size())
        {
            if (playerData.at(index).isEmpty() || playerData.at(index).isNull())
            {
                playerData.removeAt(index);
                index = 0;
            }
            else
            {
                index++;
            }
        }
        //Check if no data was read from the file
        if (playerData.isEmpty())
        {
            qDebug() << "psv_players.csv is empty";
        }
        else
        {
            //Check if the player name contains alpha-numeric characters and is not greater than 10 characters in length
            QRegExp regex("^(([A-Z])|([a-z])|([0-9])|([\u00C0-\u1FFF\u2C00-\uD7FF])|([ ]))*$");
            for (int i = 0; i < playerData.size(); i++)
            {
                if (playerData.at(i).count(':') != 2 ||
                        playerData.at(i).split(':').at(1).length() > 10 ||
                        !regex.exactMatch(playerData.at(i).split(':').at(1)) ||
                        playerData.at(i).split(':').at(1).isEmpty() ||
                        playerData.at(i).split(':').at(0).isEmpty())
                {
                    //Display warning that the data in the file is not formatted correctly
                    QMessageBox player_read_error;
                    player_read_error.warning(0,"Warning","The file \"psv_players.csv\" has data that is not formatted correctly");
                    //Discard file contents
                    playerData.clear();
                    return;
                }
            }
            //Check that the level value is correct
            QRegExp regex2("^([0-9])*$");
            for (int i = 0; i < playerData.size(); i++)
            {
                if (playerData.at(i).split(':').at(2).toInt() < 0 ||
                        playerData.at(i).split(':').at(2).toInt() > 100 ||
                        !regex2.exactMatch(playerData.at(i).split(':').at(2)) ||
                        playerData.at(i).split(':').at(2).isEmpty())
                {
                    //Display warning that the data in the file is not formatted correctly
                    QMessageBox player_read_error;
                    player_read_error.warning(0,"Warning","The file \"psv_players.csv\" has data that is not formatted correctly");
                    //Discard file contents
                    playerData.clear();
                    return;
                }
            }
            //Sort the list
            for (int i = 0; i < playerData.size(); i++)
            {
                QString current_max_string = playerData.at(i);
                unsigned int current_max = playerData.at(i).split(':').at(0).toUInt(0,10);
                int current_max_index = i;
                for (int j = i+1; j < playerData.size(); j++)
                {
                    if (current_max < playerData.at(j).split(':').at(0).toUInt(0,10))
                    {
                        current_max_string = playerData.at(j);
                        current_max = playerData.at(j).split(':').at(0).toUInt(0,10);
                        current_max_index = j;
                    }
                }
                if (current_max_index != i)
                {
                    playerData.replace(current_max_index, playerData.at(i));
                    playerData.replace(i, current_max_string);
                }
            }
            //Set current user according to the most recent timestamp
            currentUser = playerData.at(0);
            //Set the current level according to the current user
            for (int i = 0; i < levelData.size(); i++)
            {
                if (currentUser.split(':').at(2) == levelData.at(i).split(':').at(0))
                {
                    currentLevel = levelData.at(i);
                }
            }
        }
    }
    else
    {
        qDebug() << "psv_players.csv does not exist";
    }
}

/* REFRESH THE MAIN MENU */
void MainWindow::updateMenu()
{
    if (playerData.isEmpty())
    {
        ui->deleteButton->setEnabled(false);
        ui->startButton->setEnabled(false);
        ui->restartButton->setEnabled(false);
        ui->quitButton->setEnabled(false);
        ui->newButton->setEnabled(true);
    }
    else
    {
        ui->restartButton->setEnabled(false);
        ui->quitButton->setEnabled(false);
        ui->newButton->setEnabled(true);
        ui->startButton->setEnabled(true);
        ui->deleteButton->setEnabled(true);
        ui->levelNumberLabel->setText(currentUser.split(':').at(2));
        ui->userComboBox->clear();
        QStringList player_names;
        for (int i = 0; i < playerData.size(); i++)
        {
            player_names.append(playerData.at(i).split(':').at(1));
        }
        ui->userComboBox->addItems(player_names);
    }
}

/* LEVEL GENERATOR */
void MainWindow::zombieGenerator()
{
    zombieTimer->stop();
    if (zombieCounter >= zombieSequence.size())
    {
        return;
    }
    else
    {
        if (zombieSequence.at(zombieCounter) == "1")
        {
            GameObject *zombie = new NormalZombie();
            Body::random(9,10);
            Body body(720,zombieStartRows.at(Body::random(0,zombieStartRows.size()-1)),zombie->getSpeed(),180);
            zombie->setBody(body);
            gameScene->addItem(zombie);
            zombieObjects.append(zombie);
            zombie->setupGameObject();
        }
        else if (zombieSequence.at(zombieCounter) == "2")
        {
            GameObject *zombie = new FlagZombie();
            Body::random(9,10);
            Body body(720,zombieStartRows.at(Body::random(0,zombieStartRows.size()-1)),zombie->getSpeed(),180);
            zombie->setBody(body);
            gameScene->addItem(zombie);
            zombieObjects.append(zombie);
            zombie->setupGameObject();
        }
        else if (zombieSequence.at(zombieCounter) == "3")
        {
            GameObject *zombie = new ConeheadZombie();
            Body::random(9,10);
            Body body(720,zombieStartRows.at(Body::random(0,zombieStartRows.size()-1)),zombie->getSpeed(),180);
            zombie->setBody(body);
            gameScene->addItem(zombie);
            zombieObjects.append(zombie);
            zombie->setupGameObject();
        }
        else if (zombieSequence.at(zombieCounter) == "4")
        {
            GameObject *zombie = new BucketZombie();
            Body::random(9,10);
            Body body(720,zombieStartRows.at(Body::random(0,zombieStartRows.size()-1)),zombie->getSpeed(),180);
            zombie->setBody(body);
            gameScene->addItem(zombie);
            zombieObjects.append(zombie);
            zombie->setupGameObject();
        }
        else if (zombieSequence.at(zombieCounter) == "5")
        {
            GameObject *zombie = new NewspaperZombie();
            Body::random(9,10);
            Body body(720,zombieStartRows.at(Body::random(0,zombieStartRows.size()-1)),zombie->getSpeed(),180);
            zombie->setBody(body);
            gameScene->addItem(zombie);
            zombieObjects.append(zombie);
            zombie->setupGameObject();
        }
        else
        {
            qDebug() << "Zombie does not exist";
            return;
        }
    }
    zombieTimer->start(startingInterval * 1000);
    zombieCounter++;
    startingInterval = startingInterval - intervalDecrement;
}

/* BUTTON SLOTS */
void MainWindow::on_deleteButton_clicked()
{
    QString player_name = ui->nameLineEdit->text();
    int size_player_data = playerData.size();
    for (int i = 0; i < size_player_data; i++)
    {
        if (player_name == playerData.at(i).split(":").at(1))
        {
            playerData.removeAt(i);
            break;
        }
    }
    if (size_player_data != playerData.size())
    {
        //Create a new message box asking whather the user wants to delete a player
        QMessageBox delete_player;
        //Set title of the window
        delete_player.setWindowTitle("Are you sure?");
        //Set the text of the message box
        delete_player.setText(QString("Are you sure you would like to delete %1 from the game?").arg(player_name));
        //Set the okay and cancel button
        delete_player.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
        //If the user selects ok
        if (delete_player.exec() == QMessageBox::Ok)
        {
            //Get the file containing the player data
            QFile player_file(QString("%1/pvz_players.csv").arg(QDir::homePath()));
            //Check if the file can be opened and created
            if (player_file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                //Create a text stream to output data from the list
                QTextStream out(&player_file);
                out.setCodec("UTF-8");
                //Write data to the file
                for (int i = 0; i < playerData.size(); i++)
                {
                    out << playerData.at(i) << '\n';
                }
                player_file.close();
                updateMenu();
            }
            else
            {
                QMessageBox name_not_found;
                name_not_found.warning(0,"Warning","The new data could not be saved since the file could not be opened/created");
            }
        }
    }
    else
    {
        QMessageBox name_not_found;
        name_not_found.warning(0,"Warning","The name you have entered is invalid");
    }
}

void MainWindow::on_newButton_clicked()
{
    QString player_name = ui->nameLineEdit->text();
    QRegExp regex("^(([A-Z])|([a-z])|([0-9])|([\u00C0-\u1FFF\u2C00-\uD7FF])|([ ]))*$");
    if (player_name.isEmpty()||
            player_name.length() > 10||
            !regex.exactMatch(player_name))
    {
        QMessageBox incorrect_name;
        incorrect_name.warning(0,"Warning","The name you have entered is invalid");
    }
    else
    {
        //Create a new message box asking whether the user wants to add another player
        QMessageBox new_player_confirm;
        //Set the title of the window
        new_player_confirm.setWindowTitle("Are you Sure?");
        //Set the text of the message box
        new_player_confirm.setText(QString("Are you sure you would like to add %1 to the game?").arg(player_name));
        //Set the okay button
        new_player_confirm.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
        //If the user selects okay
        if (new_player_confirm.exec() == QMessageBox::Ok)
        {
            //Get the current time in miliseconds
            unsigned int current_timestamp = QDateTime::currentMSecsSinceEpoch();
            //Prepend the user data
            playerData.prepend(QString("%1:%2:1").arg(current_timestamp).arg(player_name));
            //Create the csv file containing the user data
            QFile player_file(QString("%1/pvz_players.csv").arg(QDir::homePath()));
            //Check if the file can be opened and created
            if (player_file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                //Create a text stream to output data from the list
                QTextStream out(&player_file);
                out.setCodec("UTF-8");
                //Write data to the file
                for (int i = 0; i < playerData.size(); i++)
                {
                    out << playerData.at(i) << '\n';
                }
                player_file.close();
                //Remove whitespaces
                int index = 0;
                while (index < playerData.size())
                {
                    if (playerData.at(index).isEmpty() || playerData.at(index).isNull())
                    {
                        playerData.removeAt(index);
                        index = 0;
                    }
                    else
                    {
                        index++;
                    }
                }
                currentUser = playerData.at(0);
                //Set the current level according to the current user
                for (int i = 0; i < levelData.size(); i++)
                {
                    if (currentUser.split(':').at(2) == levelData.at(i).split(':').at(0))
                    {
                        currentLevel = levelData.at(i);
                    }
                }
                updateMenu();
            }
            else
            {
                qDebug() << "couldn't create output file";
            }
        }
    }
}

void MainWindow::on_startButton_clicked()
{
    //Hide the menu so the user can start to play
    hideMenu();
    //Enable the menu button
    menuButton->setEnabled(true);
    //Generate the current level
    levelGenerator();
}

void MainWindow::on_quitButton_clicked()
{
    QMessageBox quit_message;
    quit_message.setWindowTitle("Are you sure?");
    quit_message.setText("Are you sure you want to leave this level?");
    quit_message.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
    if (quit_message.exec() == QMessageBox::Ok)
    {
        zombieCounter = 0;
        int counter = 0;
        //Delete plant objects
        while (counter < plantObjects.size())
        {
            plantObjects.at(counter)->setLife(0);
            counter++;
        }
        counter = 0;
        //Delete bullets
        while (counter < bulletObjects.size())
        {
            bulletObjects.at(counter)->setLife(0);
            counter++;
        }
        counter = 0;
        //Delete other objects
        while (counter < lawnMowerObjects.size())
        {
            lawnMowerObjects.at(counter)->setLife(0);
            counter++;
        }
        currentSunAmount = 50;
        delete gameTimer;
        gameTimer = 0;
        delete zombieTimer;
        zombieTimer = 0;
        delete refreshTimer;
        refreshTimer = 0;
        delete plantHover;
        ui->resumeButton->hide();
        ui->startButton->show();
        ui->startButton->setEnabled(true);
        ui->newButton->setEnabled(true);
        ui->deleteButton->setEnabled(true);
        ui->restartButton->setEnabled(false);
        ui->quitButton->setEnabled(false);
        plantHover = 0;
    }
}

void MainWindow::on_resumeButton_clicked()
{
    hideMenu();
}

void MainWindow::peaShooterButtonClicked()
{
    //Subtract the cost of the pea shooter
    currentSunAmount -= PeaShooter::cost;
    //Create a new plant timer to make the picture of the pea shooter follow the mouse
    plantTimer = new QTimer();
    //Connect the timer to the function
    connect(plantTimer,SIGNAL(timeout()),this,SLOT(peaShooterPlacement()));
    //Set the last clicked coordinates to (0,0)
    mousePressCoordinates = QPointF(0,0);
    //Start the plant timer
    plantTimer->start(20);
}

void MainWindow::peaShooterPlacement()
{
    //Set the mouse background image to the pea shooter image
    plantHover->setPixmap(QPixmap(":/images/pea-shooter.png"));
    plantHover->raise();
    //Set the image to follow the mouse
    plantHover->setGeometry(mouseCoordinates.x()-21,mouseCoordinates.y()-20,42,41);
    //If the user clicks anywhere on the graphics view
    if (mousePressCoordinates.x() != 0 || mousePressCoordinates.y() != 0)
    {
        //Create a new pea shooter object
        GameObject *pea_shooter = new PeaShooter();
        //Start Timer to disable button for seeding
        peaShooterButtonTimer->start(pea_shooter->getSeeding() * 1000);
        //Generate the pea shooter
        plantGenerator(pea_shooter);
    }
}

void MainWindow::sunflowerButtonClicked()
{
    //Subtract the cost of the sunflower
    currentSunAmount -= SunFlower::cost;
    //Create a new plant timer to make the picture of the sunflower follow the mouse
    plantTimer = new QTimer();
    //Connect the timer to the function
    connect(plantTimer,SIGNAL(timeout()),this,SLOT(sunflowerPlacement()));
    //Set the last clicked coordinates to (0,0)
    mousePressCoordinates = QPointF(0,0);
    //Start the plant timer
    plantTimer->start(20);
}

void MainWindow::sunflowerPlacement()
{
    //Set the mouse background image to the sunflower image
    plantHover->setPixmap(QPixmap(":/images/sunflower.png"));
    //Set the image to follow the mouse
    plantHover->setGeometry(mouseCoordinates.x()-21,mouseCoordinates.y()-21,42,42);
    //If user clicks anywhere on the graphics view
    if (mousePressCoordinates.x() != 0 || mousePressCoordinates.y() != 0)
    {
        //Create a new sunflower object
        GameObject *sunflower = new SunFlower();
        //Start Timer to disable button for seeding
        sunflowerButtonTimer->start(sunflower->getSeeding() * 1000);
        //Generate the sunflower
        plantGenerator(sunflower);
    }
}

void MainWindow::cherryBombButtonClicked()
{
    //Subtract the cost of the cherry bomb
    currentSunAmount -= CherryBomb::cost;
    //Create a new plant timer to make the picture of the cherry bomb follow the mouse
    plantTimer = new QTimer();
    //Connect the timer to the function
    connect(plantTimer,SIGNAL(timeout()),this,SLOT(cherryBombPlacement()));
    //Set the last clicked coordinates to (0,0)
    mousePressCoordinates = QPointF(0,0);
    //Start the plant timer
    plantTimer->start(20);
}

void MainWindow::cherryBombPlacement()
{
    //Set the mouse background image to the cherry bomb image
    plantHover->setPixmap(QPixmap(":/images/cherry-bomb.png"));
    //Set the image to follow the mouse
    plantHover->setGeometry(mouseCoordinates.x()-21,mouseCoordinates.y()-18,42,37);
    //If user clicks anywhere on the graphics view
    if (mousePressCoordinates.x() != 0 || mousePressCoordinates.y() != 0)
    {
        //Create a new cherry bomb object
        GameObject *cherry_bomb = new CherryBomb();
        //Start Timer to disable button for seeding
        cherryBombButtonTimer->start(cherry_bomb->getSeeding() * 1000);
        //Generate the cherry bomb
        plantGenerator(cherry_bomb);
    }
}

void MainWindow::wallnutButtonClicked()
{
    //Subtract the cost of the wallnut
    currentSunAmount -= WallNut::cost;
    //Create a new plant timer to make the picture of the wallnut follow the mouse
    plantTimer = new QTimer();
    //Connect the timer to the function
    connect(plantTimer,SIGNAL(timeout()),this,SLOT(wallnutPlacement()));
    //Set the last clicked coordinates to (0,0)
    mousePressCoordinates = QPointF(0,0);
    //Start the plant timer
    plantTimer->start(20);
}

void MainWindow::wallnutPlacement()
{
    //Set the mouse background image to the wallnut image
    plantHover->setPixmap(QPixmap(":/images/wallnut.png"));
    //Set the image to follow the mouse
    plantHover->setGeometry(mouseCoordinates.x()-21,mouseCoordinates.y()-21,42,42);
    //If user clicks anywhere on the graphics view
    if (mousePressCoordinates.x() != 0 || mousePressCoordinates.y() != 0)
    {
        //Create a new wallnut object
        GameObject *wallnut = new WallNut();
        //Start timer to disable button for seeding
        wallnutButtonTimer->start(wallnut->getSeeding() * 1000);
        //Generate the wallnut
        plantGenerator(wallnut);
    }
}

void MainWindow::potatoMineButtonClicked()
{
    //Subtract the cost of the potato mine
    currentSunAmount -= PotatoMine::cost;
    //Create a new plant timer to make the picture of the potato mine follow the mouse
    plantTimer = new QTimer();
    //Connect the timer to the function
    connect(plantTimer,SIGNAL(timeout()),this,SLOT(potatoMinePlacement()));
    //Set the last clicked coordinates to (0,0)
    mousePressCoordinates = QPointF(0,0);
    //Start the plant timer
    plantTimer->start(20);
}

void MainWindow::potatoMinePlacement()
{
    //Set the mouse background image to the potato mine image
    plantHover->setPixmap(QPixmap(":/images/potato-mine.png"));
    //Set the image to follow the mouse
    plantHover->setGeometry(mouseCoordinates.x()-21,mouseCoordinates.y()-15,42,31);
    //If user clicks anywhere on the graphics view
    if (mousePressCoordinates.x() != 0 || mousePressCoordinates.y() != 0)
    {
        //Create a new potato mine object
        GameObject *potato_mine = new PotatoMine();
        //Start timer to disable button for seeding
        potatoMineButtonTimer->start(potato_mine->getSeeding() * 1000);
        //Generate the potato mine
        plantGenerator(potato_mine);
    }
}

void MainWindow::snowPeaButtonClicked()
{
    //Subtract the cost of the snow pea
    currentSunAmount -= SnowPea::cost;
    //Create a new plant timer to make the picture of the snow pea follow the mouse
    plantTimer = new QTimer();
    //Connect the timer to the function
    connect(plantTimer,SIGNAL(timeout()),this,SLOT(snowPeaPlacement()));
    //Set the last clicked coordinates to (0,0)
    mousePressCoordinates = QPointF(0,0);
    //Start the plant timer
    plantTimer->start(20);
}

void MainWindow::snowPeaPlacement()
{
    //Set the mouse background image to the snow pea image
    plantHover->setPixmap(QPixmap(":/images/snow-pea.png"));
    //Set the image to follow the mouse
    plantHover->setGeometry(mouseCoordinates.x()-19,mouseCoordinates.y()-21,39,42);
    //If the user clicks anywhere on the graphics view
    if (mousePressCoordinates.x() != 0 || mousePressCoordinates.y() != 0)
    {
        //Create a new snow pea object
        GameObject *snow_pea = new SnowPea();
        //Start timer to disable button for seeding
        snowPeaButtonTimer->start(snow_pea->getSeeding() * 1000);
        //Generate the snow pea
        plantGenerator(snow_pea);
    }
}

void MainWindow::chomperButtonClicked()
{
    //Subtract the cost of the chomper
    currentSunAmount -= Chomper::cost;
    //Create a new plant timer to make the picture of the chomper follow the mouse
    plantTimer = new QTimer();
    //Connect the timer to the function
    connect(plantTimer,SIGNAL(timeout()),this,SLOT(chomperPlacement()));
    //Set the last clicked coordinates to (0,0)
    mousePressCoordinates = QPointF(0,0);
    //Start the plant timer
    plantTimer->start(20);
}

void MainWindow::chomperPlacement()
{
    //Disable all plant buttons
    disablePlantButtons();
    //Set the mouse background image to the chomper image
    plantHover->setPixmap(QPixmap(":/images/chomper.png"));
    //Set the image to follow the mouse
    plantHover->setGeometry(mouseCoordinates.x()-17,mouseCoordinates.y()-21,35,42);
    //If the user clicks anywhere on the grahics view
    if (mousePressCoordinates.x() != 0 || mousePressCoordinates.y() != 0)
    {
        //Create a new chomper object
        GameObject *chomper = new Chomper();
        //Start timer to disable button for seeding
        chomperButtonTimer->start(chomper->getSeeding() * 1000);
        //Generate the chomper
        plantGenerator(chomper);
    }
}

void MainWindow::repeaterButtonClicked()
{
    //Subtract the cost of the repeater
    currentSunAmount -= Repeater::cost;
    //Create a new plant timer to make the picture of the repeater follow the mouse
    plantTimer = new QTimer();
    //Connect the timer to the function
    connect(plantTimer,SIGNAL(timeout()),this,SLOT(repeaterPlacement()));
    //Set the last clicked coordinates to (0,0)
    mousePressCoordinates = QPointF(0,0);
    //Start the plant timer
    plantTimer->start(20);
}

void MainWindow::repeaterPlacement()
{
    //Set the mouse background image to the repeater
    plantHover->setPixmap(QPixmap(":/images/repeater.png"));
    //Make sure the hover is always on top
    //Set the image to follow the mouse
    plantHover->setGeometry(mouseCoordinates.x()-21,mouseCoordinates.y()-20,42,40);
    //If the user clicks anywhere on the map
    if (mousePressCoordinates.x() != 0 || mousePressCoordinates.y() != 0)
    {
        //Create a new repeater object
        GameObject *repeater = new Repeater();
        //Start timer to disable button for seeding
        repeaterButtonTimer->start(repeater->getSeeding() * 1000);
        //Generate the repeater
        plantGenerator(repeater);
    }
}

void MainWindow::menuButtonClicked()
{
    showPauseMenu();
}

void MainWindow::plantGenerator(GameObject *plant)
{
    //Stop making the image follow the mouse
    plantTimer->stop();
    //Get the coordinates where the clicked
    const int mouse_press_x = mousePressCoordinates.x();
    const int mouse_press_y = mousePressCoordinates.y();
    //Check if the user clicked the lawn area
    if (mouse_press_x > 72 && mouse_press_x < 720 && mouse_press_y > 82 && mouse_press_y < 442)
    {
        //Check if use clicks in the available rows
        if (mouse_press_y > availableRowsBottomY && mouse_press_y < availableRowsTopY)
        {
            //Check if the given plant is a repeater
            if (plant->getObjectType() == "Repeater")
            {
                //Check if the place clicked contains a pea shooter
                for (int i = 0; i < plantObjects.size(); i++)
                {
                    if (plantObjects.at(i)->getObjectType() == "PeaShooter" &&
                            mouse_press_x >= plantObjects.at(i)->x() &&
                            mouse_press_x <= (plantObjects.at(i)->x() + 72) &&
                            mouse_press_y >= plantObjects.at(i)->y() &&
                            mouse_press_y <= (plantObjects.at(i)->y() + 71))
                    {
                        //Delete the existing pea shooter
                        plantObjects.at(i)->setLife(0);
                        //Get the nearest column x value of the mouse click coordinate
                        int x = mouse_press_x - ((mouse_press_x - 72) % 72);
                        //Get the nearest row y value of the mouse click coordinate
                        int y = mouse_press_y - ((mouse_press_y - 82) % 72);
                        //Create a new body to set parameters of the game object
                        plant = new Repeater();
                        Body body(x,y,0,0);
                        //Set the body
                        plant->setBody(body);
                        //Add the plant to the scene
                        gameScene->addItem(plant);
                        //Add the plant game object to the list
                        plantObjects.append(plant);
                        //Initialize the plant object
                        plant->setupGameObject();
                        //Reset the hover tool
                        plantHover->setGeometry(-70,-70,0,0);
                        return;
                    }
                }
                plantTimer->start();
                delete plant;
                plant = 0;
                return;
            }
            else
            {
                //Check if the user clicked on an already existing plant
                for (int i = 0; i < plantObjects.size(); i++)
                {
                    if (mouse_press_x >= plantObjects.at(i)->x() &&
                            mouse_press_x <= (plantObjects.at(i)->x() + 71) &&
                            mouse_press_y >= plantObjects.at(i)->y() &&
                            mouse_press_y <= (plantObjects.at(i)->y() + 71))
                    {
                        plantTimer->start();
                        delete plant;
                        plant = 0;
                        return;
                    }
                }
                //Check if the user clicked on a zombie
                for (int i = 0; i < zombieObjects.size(); i++)
                {
                    if (mouse_press_x >= zombieObjects.at(i)->x() &&
                            mouse_press_x <= (zombieObjects.at(i)->x() + 71) &&
                            mouse_press_y >= zombieObjects.at(i)->y() &&
                            mouse_press_y <= (zombieObjects.at(i)->y() + 71))
                    {
                        plantTimer->start();
                        delete plant;
                        plant = 0;
                        return;
                    }
                }
            }
            //Get the nearest column x value of the mouse click coordinate
            int x = mouse_press_x - ((mouse_press_x - 72) % 72);
            //Get the nearest row y value of the mouse click coordinate
            int y = mouse_press_y - ((mouse_press_y - 82) % 72);
            //Create a new body to set parameters of the game object
            Body body(x,y,0,0);
            //Set the body
            plant->setBody(body);
            //Add the plant to the scene
            gameScene->addItem(plant);
            //Add the plant game object to the list
            plantObjects.append(plant);
            //Initialize the plant object
            plant->setupGameObject();
        }
        else
        {
            plantTimer->start();
            delete plant;
            plant = 0;
            return;
        }
    }
    else
    {
        currentSunAmount += plant->getCost();
        delete plant;
        plant = 0;
    }
    plantHover->setGeometry(-70,-70,0,0);
}

void MainWindow::levelGenerator()
{
    //Set up variables for current level
    levelNumber = currentLevel.split(':').at(0);                    //The level the user will start playing on
    zombieSequence = currentLevel.split(':').at(1).split(',');      //Order in which each type of zombie will appear
    availableRows = currentLevel.split(':').at(2);                  //Rows available to place plants
    startTime = currentLevel.split(':').at(3).toInt();              //Time till the first zombie appears
    startingInterval = currentLevel.split(':').at(4).toInt();       //Time till the next zombie appears after start
    intervalDecrement = currentLevel.split(':').at(5).toFloat();    //Time subtracted from the starting interval after each zombie
    //Set up available rows
    zombieStartRows.clear();
    if (availableRows == "1")
    {
        setDirtRow(72,82);
        setDirtRow(72,154);
        setGrassRow(72,226,1);
        setDirtRow(72,298);
        setDirtRow(72,370);
        zombieStartRows.append(226);
        availableRowsBottomY = 226;
        availableRowsTopY = 298;
        setLawnMower(72,226);
    }
    else if (availableRows == "3")
    {
        setDirtRow(72,82);
        setGrassRow(72,154,1);
        setGrassRow(72,226,2);
        setGrassRow(72,298,1);
        setDirtRow(72,370);
        zombieStartRows.append(154);
        zombieStartRows.append(226);
        zombieStartRows.append(298);
        availableRowsBottomY = 154;
        availableRowsTopY = 370;
        setLawnMower(72,154);
        setLawnMower(72,226);
        setLawnMower(72,298);
    }
    else
    {
        setGrassRow(72,82,1);
        setGrassRow(72,154,2);
        setGrassRow(72,226,1);
        setGrassRow(72,298,2);
        setGrassRow(72,370,1);
        zombieStartRows.append(82);
        zombieStartRows.append(154);
        zombieStartRows.append(226);
        zombieStartRows.append(298);
        zombieStartRows.append(370);
        availableRowsBottomY = 82;
        availableRowsTopY = 442;
        //setLawnMower(72,82);
        //setLawnMower(72,154);
        //setLawnMower(72,226);
        //setLawnMower(72,298);
        //setLawnMower(72,370);
    }

    //Refresh game items
    refreshTimer = new QTimer();
    connect(refreshTimer,SIGNAL(timeout()),this,SLOT(refreshGameObjects()));
    refreshTimer->start(20);

    //Game timer
    gameTimer = new QTimer(this);
    connect(gameTimer, SIGNAL(timeout()), gameScene, SLOT(advance()));
    gameTimer->start(20);

    //Zombie timer
    zombieTimer = new QTimer();
    connect(zombieTimer, SIGNAL(timeout()), this, SLOT(zombieGenerator()));
    zombieTimer->start(startTime * 1000);

    //Produce sun on the map
    GameObject *sun = new Sun("computer-generated");
    lawnMowerObjects.append(sun);

    //Add the plant hover label to the game scene
    plantHover = new QLabel();
    plantHover->setAttribute(Qt::WA_TranslucentBackground,true);
    plantHover->setGeometry(-5,-5,0,0);
    gameScene->addWidget(plantHover);
}

void MainWindow::refreshGameObjects()
{
    for (int i = 0; i < zombieObjects.size(); i++)
    {
        if (zombieObjects.at(i)->scenePos().x() < 72)
        {
            QMessageBox death;
            death.setText("Zombies have eaten your brain");
            death.setStandardButtons(QMessageBox::Ok);
            if (death.exec() == QMessageBox::Ok)
            {
                zombieCounter = 0;
                int counter = 0;
                //Delete plant objects
                while (counter < plantObjects.size())
                {
                    plantObjects.at(counter)->setLife(0);
                    counter++;
                }
                counter = 0;
                //Delete bullets
                while (counter < bulletObjects.size())
                {
                    bulletObjects.at(counter)->setLife(0);
                    counter++;
                }
                counter = 0;
                //Delete other objects
                while (counter < lawnMowerObjects.size())
                {
                    lawnMowerObjects.at(counter)->setLife(0);
                    counter++;
                }
                currentSunAmount = 50;
                delete gameTimer;
                gameTimer = 0;
                delete zombieTimer;
                zombieTimer = 0;
                delete refreshTimer;
                refreshTimer = 0;
                delete plantHover;
                plantHover = 0;
                levelGenerator();
            }
        }
    }
    //Check whether the user has enough money for plants
    enablePlantButtons();
    //Refresh the amount of sun the user has
    if (sunValue->text().toInt() != currentSunAmount)
    {
        sunValue->setText(QString("%1").arg(currentSunAmount));
    }
    //Go to next level
    if (zombieCounter == zombieSequence.size() && zombieObjects.isEmpty())
    {
        if (currentLevel.split(':').at(0).toInt() >= 8)
        {
            qDebug() << "Finished level";
            QMessageBox win_box;
            win_box.setWindowTitle("Congradulations");
            win_box.setText("Congradulations! You have won the game");
            win_box.setStandardButtons(QMessageBox::Ok);
            if (win_box.exec() == QMessageBox::Ok)
            {
                for (int i = 0; i < levelData.size(); i++)
                {
                    if (levelData.at(i).split(':').at(0) == "1")
                    {
                        currentLevel = levelData.at(i);
                        ui->levelNumberLabel->setText("1");
                        zombieCounter = 0;
                        int counter = 0;
                        //Delete plant objects
                        while (counter < plantObjects.size())
                        {
                            plantObjects.at(counter)->setLife(0);
                            counter++;
                        }
                        counter = 0;
                        //Delete bullets
                        while (counter < bulletObjects.size())
                        {
                            bulletObjects.at(counter)->setLife(0);
                            counter++;
                        }
                        counter = 0;
                        //Delete other objects
                        while (counter < lawnMowerObjects.size())
                        {
                            lawnMowerObjects.at(counter)->setLife(0);
                            counter++;
                        }
                        currentSunAmount = 50;
                        delete gameTimer;
                        gameTimer = 0;
                        delete zombieTimer;
                        zombieTimer = 0;
                        delete refreshTimer;
                        refreshTimer = 0;
                        delete plantHover;
                        plantHover = 0;
                        showMainMenu();
                        ui->newButton->setEnabled(true);
                        ui->deleteButton->setEnabled(true);
                        ui->startButton->setEnabled(true);
                        ui->restartButton->setEnabled(false);
                        ui->quitButton->setEnabled(false);
                        break;
                    }

                }
            }
        }
        else
        {
            for (int i = 0; i < levelData.size(); i++)
            {
                if (levelData.at(i).split(':').at(0).toInt() == (currentLevel.split(':').at(0).toInt() + 1))
                {
                    currentLevel = levelData.at(i);
                    zombieCounter = 0;
                    int counter = 0;
                    //Delete plant objects
                    while (counter < plantObjects.size())
                    {
                        plantObjects.at(counter)->setLife(0);
                        counter++;
                    }
                    counter = 0;
                    //Delete bullets
                    while (counter < bulletObjects.size())
                    {
                        bulletObjects.at(counter)->setLife(0);
                        counter++;
                    }
                    counter = 0;
                    //Delete other objects
                    while (counter < lawnMowerObjects.size())
                    {
                        lawnMowerObjects.at(counter)->setLife(0);
                        counter++;
                    }
                    currentSunAmount = 50;
                    delete gameTimer;
                    gameTimer = 0;
                    delete zombieTimer;
                    zombieTimer = 0;
                    delete refreshTimer;
                    refreshTimer = 0;
                    delete plantHover;
                    plantHover = 0;
                    levelGenerator();
                    ui->levelNumberLabel->setText(levelData.at(i).split(':').at(0));
                    break;
                }
            }
        }
    }
}

void MainWindow::enablePlantButtons()
{
    if (PeaShooter::cost <= currentSunAmount && !peaShooterButtonTimer->isActive() && !plantTimer->isActive())
    {
        peaShooterButton->setEnabled(true);
    }
    else
    {
        peaShooterButton->setEnabled(false);
    }
    if (SunFlower::cost <= currentSunAmount && !sunflowerButtonTimer->isActive() && !plantTimer->isActive())
    {
        sunflowerButton->setEnabled(true);
    }
    else
    {
        sunflowerButton->setEnabled(false);
    }
    if (CherryBomb::cost <= currentSunAmount && !cherryBombButtonTimer->isActive() && !plantTimer->isActive())
    {
        cherryBombButton->setEnabled(true);
    }
    else
    {
        cherryBombButton->setEnabled(false);
    }
    if (WallNut::cost <= currentSunAmount && !wallnutButtonTimer->isActive() && !plantTimer->isActive())
    {
        wallnutButton->setEnabled(true);
    }
    else
    {
        wallnutButton->setEnabled(false);
    }
    if (PotatoMine::cost <= currentSunAmount && !potatoMineButtonTimer->isActive() && !plantTimer->isActive())
    {
        potatoMineButton->setEnabled(true);
    }
    else
    {
        potatoMineButton->setEnabled(false);
    }
    if (SnowPea::cost <= currentSunAmount && !snowPeaButtonTimer->isActive() && !plantTimer->isActive())
    {
        snowPeaButton->setEnabled(true);
    }
    else
    {
        snowPeaButton->setEnabled(false);
    }
    if (Chomper::cost <= currentSunAmount && !chomperButtonTimer->isActive() && !plantTimer->isActive())
    {
        chomperButton->setEnabled(true);
    }
    else
    {
        chomperButton->setEnabled(false);
    }
    if (Repeater::cost <= currentSunAmount && !repeaterButtonTimer->isActive() && !plantTimer->isActive())
    {
        repeaterButton->setEnabled(true);
    }
    else
    {
        repeaterButton->setEnabled(false);
    }
}

void MainWindow::showMainMenu()
{
    ui->menuListWidget->show();
    ui->deleteButton->show();
    ui->menuLabel->show();
    ui->newButton->show();
    ui->quitButton->show();
    ui->restartButton->show();
    ui->startButton->show();
    ui->levelLabel->show();
    ui->levelNumberLabel->show();
    ui->nameLabel->show();
    ui->userLabel->show();
    ui->userComboBox->show();
    ui->resumeButton->hide();
}

void MainWindow::showPauseMenu()
{
    ui->menuListWidget->show();
    ui->deleteButton->show();
    ui->menuLabel->show();
    ui->newButton->show();
    ui->quitButton->show();
    ui->restartButton->show();
    ui->startButton->hide();
    ui->levelLabel->show();
    ui->levelNumberLabel->show();
    ui->nameLabel->show();
    ui->userLabel->show();
    ui->userComboBox->show();
    ui->resumeButton->show();
    ui->nameLineEdit->show();
    ui->newButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    ui->quitButton->setEnabled(true);
    ui->restartButton->setEnabled(true);
}

void MainWindow::hideMenu()
{
    ui->menuListWidget->hide();
    ui->deleteButton->hide();
    ui->menuLabel->hide();
    ui->newButton->hide();
    ui->quitButton->hide();
    ui->restartButton->hide();
    ui->startButton->hide();
    ui->levelLabel->hide();
    ui->levelNumberLabel->hide();
    ui->nameLabel->hide();
    ui->userLabel->hide();
    ui->userComboBox->hide();
    ui->resumeButton->hide();
    ui->nameLineEdit->hide();
}

void MainWindow::enableGameplayButtons()
{
    peaShooterButton->setEnabled(true);
    sunflowerButton->setEnabled(true);
    cherryBombButton->setEnabled(true);
    wallnutButton->setEnabled(true);
    potatoMineButton->setEnabled(true);
    snowPeaButton->setEnabled(true);
    chomperButton->setEnabled(true);
    repeaterButton->setEnabled(true);
    menuButton->setEnabled(true);
}

void MainWindow::disableGameplayButtons()
{
    peaShooterButton->setEnabled(false);
    sunflowerButton->setEnabled(false);
    cherryBombButton->setEnabled(false);
    wallnutButton->setEnabled(false);
    potatoMineButton->setEnabled(false);
    snowPeaButton->setEnabled(false);
    chomperButton->setEnabled(false);
    repeaterButton->setEnabled(false);
    menuButton->setEnabled(false);
}

void MainWindow::disablePlantButtons()
{
    peaShooterButton->setEnabled(false);
    sunflowerButton->setEnabled(false);
    cherryBombButton->setEnabled(false);
    wallnutButton->setEnabled(false);
    potatoMineButton->setEnabled(false);
    snowPeaButton->setEnabled(false);
    chomperButton->setEnabled(false);
    repeaterButton->setEnabled(false);
}

void MainWindow::createGameplayUi()
{
    //Set up the pea shooter button
    peaShooterButton = new QPushButton();
    peaShooterButton->setGeometry(72,0,59,82);
    peaShooterButton->setIcon(QIcon(":/images/pea-shooter-button.jpg"));
    peaShooterButton->setIconSize(QSize(59,82));
    peaShooterButton->setToolTip("Pea Shooter Cost: 100");
    //Set up the sunflower button
    sunflowerButton = new QPushButton();
    sunflowerButton->setGeometry(134,0,59,82);
    sunflowerButton->setIcon(QIcon(":/images/sunflower-button.jpg"));
    sunflowerButton->setIconSize(QSize(59,82));
    sunflowerButton->setToolTip("Sunflower Cost: 50");
    //Set up the cherry bomb button
    cherryBombButton = new QPushButton();
    cherryBombButton->setGeometry(196,0,59,82);
    cherryBombButton->setIcon(QIcon(":/images/cherry-bomb-button.jpg"));
    cherryBombButton->setIconSize(QSize(59,82));
    cherryBombButton->setToolTip("Cherry Bomb Cost: 150");
    //Set up the wallnut button
    wallnutButton = new QPushButton();
    wallnutButton->setGeometry(258,0,59,82);
    wallnutButton->setIcon(QIcon(":/images/wallnut-button.jpg"));
    wallnutButton->setIconSize(QSize(59,82));
    wallnutButton->setToolTip("Wallnut Cost: 50");
    //Set up the potato mine button
    potatoMineButton = new QPushButton();
    potatoMineButton->setGeometry(320,0,59,82);
    potatoMineButton->setIcon(QIcon(":/images/potato-mine-button.jpg"));
    potatoMineButton->setIconSize(QSize(59,82));
    potatoMineButton->setToolTip("Potato Mine Cost: 25");
    //Setup the snow pea button
    snowPeaButton = new QPushButton();
    snowPeaButton->setGeometry(382,0,59,82);
    snowPeaButton->setIcon(QIcon(":/images/snow-pea-button.jpg"));
    snowPeaButton->setIconSize(QSize(59,82));
    snowPeaButton->setToolTip("Snow Pea Cost: 175");
    //Setup the chomper button
    chomperButton = new QPushButton();
    chomperButton->setGeometry(444,0,59,82);
    chomperButton->setIcon(QIcon(":/images/chomper-button.jpg"));
    chomperButton->setIconSize(QSize(59,82));
    chomperButton->setToolTip("Chomper Cost: 150");
    //Set up the repeater button
    repeaterButton = new QPushButton();
    repeaterButton->setGeometry(506,0,59,82);
    repeaterButton->setIcon(QIcon(":/images/repeater-button.jpg"));
    repeaterButton->setIconSize(QSize(59,82));
    repeaterButton->setToolTip("Repeater Cost: 200");
    //Set up the menu button
    menuButton = new QPushButton();
    menuButton->setGeometry(688,0,60,23);
    menuButton->setText("Menu");

    //Set up button slots
    connect(peaShooterButton,SIGNAL(clicked()),this,SLOT(peaShooterButtonClicked()));
    connect(sunflowerButton,SIGNAL(clicked()),this,SLOT(sunflowerButtonClicked()));
    connect(cherryBombButton,SIGNAL(clicked()),this,SLOT(cherryBombButtonClicked()));
    connect(wallnutButton,SIGNAL(clicked()),this,SLOT(wallnutButtonClicked()));
    connect(potatoMineButton,SIGNAL(clicked()),this,SLOT(potatoMineButtonClicked()));
    connect(snowPeaButton,SIGNAL(clicked()),this,SLOT(snowPeaButtonClicked()));
    connect(chomperButton,SIGNAL(clicked()),this,SLOT(chomperButtonClicked()));
    connect(repeaterButton,SIGNAL(clicked()),this,SLOT(repeaterButtonClicked()));
    connect(menuButton,SIGNAL(clicked()),this,SLOT(menuButtonClicked()));

    //Set up UI labels
    sunValue = new QLabel();
    sunValue->setAttribute(Qt::WA_TranslucentBackground);
    sunValue->setGeometry(10,65,52,15);
    sunValue->setAlignment(Qt::AlignCenter);
    sunValue->setText("0");

    //Set up UI Pictures
    sunImage = new QLabel();
    sunImage->setAttribute(Qt::WA_TranslucentBackground);
    sunImage->setGeometry(20,20,35,35);
    sunImage->setPixmap(QPixmap(":/images/sun.png"));
    plantTimer = new QTimer();
    plantTimer->stop();

    //Disable gameplay buttons
    disableGameplayButtons();

    //Start timers for each button
    peaShooterButtonTimer = new QTimer();
    peaShooterButtonTimer->setSingleShot(true);
    peaShooterButtonTimer->stop();
    sunflowerButtonTimer = new QTimer();
    sunflowerButtonTimer->setSingleShot(true);
    sunflowerButtonTimer->stop();
    cherryBombButtonTimer = new QTimer();
    cherryBombButtonTimer->setSingleShot(true);
    cherryBombButtonTimer->stop();
    wallnutButtonTimer = new QTimer();
    wallnutButtonTimer->setSingleShot(true);
    wallnutButtonTimer->stop();
    potatoMineButtonTimer = new QTimer();
    potatoMineButtonTimer->setSingleShot(true);
    potatoMineButtonTimer->stop();
    snowPeaButtonTimer = new QTimer();
    snowPeaButtonTimer->setSingleShot(true);
    snowPeaButtonTimer->stop();
    chomperButtonTimer = new QTimer();
    chomperButtonTimer->setSingleShot(true);
    chomperButtonTimer->stop();
    repeaterButtonTimer = new QTimer();
    repeaterButtonTimer->setSingleShot(true);
    repeaterButtonTimer->stop();
}

void MainWindow::setupGameScene()
{
    //Add ui elements to scene
    gameScene->addWidget(peaShooterButton);
    gameScene->addWidget(sunflowerButton);
    gameScene->addWidget(cherryBombButton);
    gameScene->addWidget(wallnutButton);
    gameScene->addWidget(potatoMineButton);
    gameScene->addWidget(snowPeaButton);
    gameScene->addWidget(chomperButton);
    gameScene->addWidget(repeaterButton);
    gameScene->addWidget(menuButton);
    gameScene->addWidget(sunValue);
    gameScene->addWidget(sunImage);

    //Add scene to the main UI
    ui->gameView->setScene(gameScene);
    gameScene->setSceneRect(0, 0, GAME_WIDTH, GAME_HEIGHT);

    //Outlines
    gameScene->addRect(0,82,72,360,QPen(Qt::gray));     //house
}

void MainWindow::setDirtRow(int x, int y)
{
    QGraphicsPixmapItem *dirt_row = new QGraphicsPixmapItem();
    dirt_row->setPixmap(QPixmap(":/images/dirt-row.png"));
    dirt_row->setPos(x,y);
    dirt_row->setZValue(-5);
    gameScene->addItem(dirt_row);
}

void MainWindow::setGrassRow(int x, int y, int type)
{
    QGraphicsPixmapItem *grass_row = new QGraphicsPixmapItem();
    if (type == 1)
    {
        grass_row->setPixmap(QPixmap(":/images/grass-row.png"));
    }
    else
    {
        grass_row->setPixmap(QPixmap(":/images/grass-row-alt.png"));
    }
    grass_row->setZValue(-4);
    grass_row->setPos(x-2,y-2);
    gameScene->addItem(grass_row);
}

void MainWindow::setLawnMower(int x, int y)
{
    //Create a new lawn mower object
    GameObject *lawn_mower = new LawnMower();
    //Create a new body to set the parameters of the lawn mower
    Body body(x-50,y+9.5,0,0);
    //Set the body to the lawn mower object
    lawn_mower->setBody(body);
    //Set Z index of lawn mower
    lawn_mower->setZValue(400);
    //Add the lawn mower to the game scene
    gameScene->addItem(lawn_mower);
    //Add the lawn mower object to the lawn mower list
    lawnMowerObjects.append(lawn_mower);
}

//Destructor
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_restartButton_clicked()
{
    QMessageBox quit_message;
    quit_message.setWindowTitle("Are you sure?");
    quit_message.setText("Are you sure you want to restart this level?");
    quit_message.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
    if (quit_message.exec() == QMessageBox::Ok)
    {
        zombieCounter = 0;
        int counter = 0;
        //Delete plant objects
        while (counter < plantObjects.size())
        {
            plantObjects.at(counter)->setLife(0);
            counter++;
        }
        counter = 0;
        //Delete bullets
        while (counter < bulletObjects.size())
        {
            bulletObjects.at(counter)->setLife(0);
            counter++;
        }
        counter = 0;
        //Delete other objects
        while (counter < lawnMowerObjects.size())
        {
            lawnMowerObjects.at(counter)->setLife(0);
            counter++;
        }
        currentSunAmount = 50;
        delete gameTimer;
        gameTimer = 0;
        delete zombieTimer;
        zombieTimer = 0;
        delete refreshTimer;
        refreshTimer = 0;
        delete plantHover;
        plantHover = 0;
        hideMenu();
        levelGenerator();
    }
}
