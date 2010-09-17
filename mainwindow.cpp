/*
 * Copyright 2010 Jörg Ehrichs <joerg.ehichs@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "settingsdialog.h"
#include "serverdebugconsole.h"

#include "dcserver.h"
#include "project.h"
#include "projecttreeview.h"

#include "dcsensor.h"
#include "dcactuator.h"
#include "dctrain.h"

#include <QtGui/QSplitter>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        m_projectView(0)
{
    ui->setupUi(this);

    m_projectView = new ProjectTreeView();

    QSplitter *splitter = new QSplitter(parent);
    splitter->addWidget(m_projectView);
    splitter->addWidget(ui->graphicsView);
    setCentralWidget(splitter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showSettings()
{
    SettingsDialog setting;
    setting.exec();
}

void MainWindow::showAbout()
{
}

void MainWindow::showDebugConsole()
{
    ServerDebugConsole *console = new ServerDebugConsole();
    console->show();

    //server->disconnectSRCP();
}

void MainWindow::newProject()
{
    m_project = new Project(this);
    m_project->setName("Test Project");

    DCServer *server = new DCServer();
    server->setHost("127.0.0.1");
    server->setPort("4303");

    m_project->setServer(server);

    connect (server, SIGNAL(connectionClosed()), this, SLOT(disconnectServer()));
    connect (server, SIGNAL(connectionEstablished()), this, SLOT(connectedToServer()));

    ui->menu_Server->setEnabled(true);
    ui->menu_Power->setEnabled(true);

    m_projectView->setProject(m_project);


    //create some fake devices
    /*
    DCSensor *sens = new DCSensor;
    connect(sens, SIGNAL(initDevice(QString)), m_project->server(), SLOT(sendSRCP(QString)));
    sens->setBusID(1);
    sens->setAddress(1);
    sens->initialize();*/

    DCActuator *actuator = new DCActuator();
    actuator->setBusID(1);
    actuator->setAddress(1);
    actuator->setPort(1);
    actuator->setProtocol(DCActuator::MAERKLIN);
    m_project->addActuator(actuator);

    DCActuator *actuator2 = new DCActuator();
    actuator2->setBusID(1);
    actuator2->setAddress(2);
    actuator2->setPort(1);
    actuator2->setProtocol(DCActuator::MAERKLIN);
    m_project->addActuator(actuator2);

    DCTrain *train = new DCTrain();
    train->setBusID(1);
    train->setAddress(1);
    train->setProtocol(DCTrain::DCC1);
    train->setDecoderSteps(128);
    train->setDecoderFunctions(4);
    m_project->addTrain(train);

    DCTrain *train2 = new DCTrain();
    train2->setBusID(1);
    train2->setAddress(2);
    train2->setProtocol(DCTrain::MM2);
    train2->setDecoderSteps(27);
    train2->setDecoderFunctions(2);
    m_project->addTrain(train2);
}

void MainWindow::saveProject()
{
    DCTrain *t = m_project->trains().first();

    t->setSpeed(24);
    t->setmaxSpeed(100);
    t->setDriveMode(DCTrain::FORWARD);

    t->setFunction(1,true);
    t->sendValue();

    DCActuator *actuator = new DCActuator();
    actuator->setBusID(1);
    actuator->setAddress(3);
    actuator->setProtocol(DCActuator::MAERKLIN);
    m_project->addActuator(actuator);
}

void MainWindow::connectServer()
{
    m_project->server()->connectSRCP();
}

void MainWindow::connectedToServer()
{
    ui->actionConnect_to_Server->setEnabled(false);
    ui->actionDisconnect->setEnabled(true);
    ui->actionShow_debug_console->setEnabled(true);
    ui->actionServer_information->setEnabled(true);
    ui->actionSave->setEnabled(true);

    m_project->initializeDevices();
}

void MainWindow::disconnectServer()
{
    m_project->server()->disconnectSRCP();

    ui->actionConnect_to_Server->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionShow_debug_console->setEnabled(false);
    ui->actionServer_information->setEnabled(false);
}
