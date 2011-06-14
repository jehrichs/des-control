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

#include "hwsettings.h"
#include "settingsdialog.h"
#include "project.h"
#include "projectwidget.h"
#include "des/automatonwidget.h"
#include "automatontreewidget.h"
#include "projectserializer.h"

#include "srcp/serverdebugconsole.h"
#include "hhdebugdialog.h"
#include "srcp/dcserver.h"
#include "srcp/dcsensor.h"
#include "srcp/dcactuator.h"
#include "srcp/dctrain.h"

#include "des/dcautomaton.h"
#include "des/automatonview.h"
#include "des/dccontroller.h"
#include "des/eventstatus.h"
#include "hwconnections.h"

#include "import/importautomaton.h"

#include <QCloseEvent>
#include <QtGui/QSplitter>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_project(0)
    , m_controller(new DCController())
    , m_splitter(0)
    , m_projectWidget(0)
    , m_automatonWidget(0)
    , m_autonamtonMode(0)
    , m_controllerState(0)
    , m_unsavedChanges(false)
{
    ui->setupUi(this);

    createActions();
    createToolBars();

    showEmptyView();

    readSettings();

    m_es = new EventStatus();

    connect(m_controller, SIGNAL(historyEntry(QString)), this, SLOT(printSessionHistory(QString)));

    QThread *testThread = new QThread;
    m_controller->moveToThread(testThread);
    testThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_projectWidget;
    delete m_automatonWidget;
    delete m_project;
    delete m_autonamtonMode;
    delete m_es;
}

void MainWindow::writeSettings()
{
    QSettings settings;

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("mainToolBar", ui->mainToolBar->isVisible());
    settings.setValue("controllerToolBar", ui->controllerToolBar->isVisible());
    settings.setValue("statusbar", ui->statusBar->isVisible());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings;

    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(800, 600)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());

    ui->mainToolBar->setVisible(settings.value("mainToolBar", true).toBool());
    ui->statusBar->setVisible(settings.value("statusbar", true).toBool());
    ui->actToggleStatusBar->setChecked(settings.value("statusbar", true).toBool());

    settings.endGroup();

    m_hw = new HWSettings;

    DCServer *server = new DCServer();
    server->setHost("localhost");
    server->setPort("4303");

    m_hw->setServer(server);

    connect (server, SIGNAL(connectionClosed()), this, SLOT(disconnectedFromServer()));
    connect (server, SIGNAL(connectionEstablished()), this, SLOT(connectedToServer()));

    DCTrain *t1 = new DCTrain();
    t1->setName(QString("Train 1"));
    t1->setProtocol(DCTrain::MM2);
    t1->setAddress(10);
    t1->setDecoderSteps(14);
    t1->setDecoderFunctions(4);
    m_hw->addTrain(t1);
    DCTrain *t2 = new DCTrain();
    t2->setName(QString("Train 2"));
    t2->setProtocol(DCTrain::MM2);
    t2->setAddress(20);
    t2->setDecoderSteps(14);
    t2->setDecoderFunctions(4);
    m_hw->addTrain(t2);

    connect ( ui->actStartTrains, SIGNAL(triggered()), m_hw, SLOT(startTrains()));
    connect ( ui->actStopTrains, SIGNAL(triggered()), m_hw, SLOT(stopTrains()));

    DCActuator *a1 = new DCActuator();
    a1->setName(QString("Switch 1"));
    a1->setProtocol(DCActuator::MAERKLIN);
    a1->setAddress(1);
    m_hw->addActuator(a1);
    DCActuator *a2 = new DCActuator();
    a2->setName(QString("Switch 2"));
    a2->setProtocol(DCActuator::MAERKLIN);
    a2->setAddress(2);
    m_hw->addActuator(a2);
    DCActuator *a3 = new DCActuator();
    a3->setName(QString("Switch 3"));
    a3->setProtocol(DCActuator::MAERKLIN);
    a3->setAddress(3);
    m_hw->addActuator(a3);
    DCActuator *a4 = new DCActuator();
    a4->setName(QString("Switch 4"));
    a4->setProtocol(DCActuator::MAERKLIN);
    a4->setAddress(4);
    m_hw->addActuator(a4);

    DCSensor *s1 = new DCSensor();
    s1->setName(QString("Sensor 1 inner"));
    s1->setAddress(1);
    m_hw->addSensor(s1);
    DCSensor *s2 = new DCSensor();
    s2->setName(QString("Sensor 1 outer"));
    s2->setAddress(2);
    m_hw->addSensor(s2);
    DCSensor *s3 = new DCSensor();
    s3->setName(QString("Sensor 2 inner"));
    s3->setAddress(3);
    m_hw->addSensor(s3);
    DCSensor *s4 = new DCSensor();
    s4->setName(QString("Sensor 2 outer"));
    s4->setAddress(4);
    m_hw->addSensor(s4);

    m_controller->setHWSettings(m_hw);
}

void MainWindow::newProject()
{
    if(m_project)
    {
        if(!closeProject())
            return;
    }

    //TODO show project wizard
    m_project = new Project();
    m_project->setName("Test Project");
    connect (m_project, SIGNAL(projectChanged()), this, SLOT(unsavedChanges()));


    showProjectView();
    m_unsavedChanges = false;

    ui->actSaveProject->setEnabled(true);
    ui->actSaveProjectAs->setEnabled(true);
    ui->actCloseProject->setEnabled(true);
    ui->actImportAutomaton->setEnabled(true);
    ui->menuAutomata->setEnabled(true);
    ui->menuServer->setEnabled(true);
    ui->actShowSettings->setEnabled(true);
    ui->actDeleteAutomaton->setEnabled(true);

    m_projectWidget->setMode("unknown");
}

void MainWindow::openProject()
{
    if(m_project)
        if(!closeProject())
            return;

    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Open DES Project"),
                                         QDir::currentPath(),
                                         tr("DES Files (*.xml)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("DES Project Reader"),
                             tr("Cannot open file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    ProjectSerializer serializer;
    m_project = serializer.loadProject(&file);
    if(m_project)
    {
        statusBar()->showMessage(tr("Project opened"), 20000);
        m_project->setFileName(fileName);
        connect (m_project, SIGNAL(projectChanged()), this, SLOT(unsavedChanges()));

        showProjectView();
        m_unsavedChanges = false;

        ui->actSaveProject->setEnabled(true);
        ui->actSaveProjectAs->setEnabled(true);
        ui->actCloseProject->setEnabled(true);
        ui->actImportAutomaton->setEnabled(true);
        ui->menuAutomata->setEnabled(true);
        ui->menuServer->setEnabled(true);
        ui->actShowSettings->setEnabled(true);
        ui->actDeleteAutomaton->setEnabled(true);

        m_projectWidget->setMode("unknown");
    }
    else
    {
        delete m_project;
        m_project = 0;
    }
}

void MainWindow::saveProject()
{
    if(m_project->fileName().isEmpty())
        saveProjectAs();

    else
    {
        QFile file(m_project->fileName());
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("DES Project Writer"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(m_project->fileName())
                                 .arg(file.errorString()));
            return;
        }

        ProjectSerializer serializer;
        if(serializer.saveProject(m_project, &file))
            statusBar()->showMessage(tr("Project saved"), 20000);

        m_unsavedChanges = false;
    }
}

void MainWindow::saveProjectAs()
{
    QString fileName =
            QFileDialog::getSaveFileName(this, tr("Save DES Project"),
                                         QDir::currentPath(),
                                         tr("DES Files (*.xml)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("DES Project Writer"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    m_project->setFileName(fileName);

    ProjectSerializer serializer;
    if(serializer.saveProject(m_project, &file))
        statusBar()->showMessage(tr("Project saved"), 20000);

    m_unsavedChanges = false;
}

bool MainWindow::closeProject()
{
    if(m_unsavedChanges)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("The project has been modified."));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret) {
        case QMessageBox::Save:
            saveProject();
            break;
        case QMessageBox::Discard:
            // do nothing, don't sav and close project further down
            break;
        case QMessageBox::Cancel:
            // skip project closing
            return false;
            break;
        default:
            return false;
            break;
        }
    }

    showEmptyView();
    m_controller->stopController();

    if( m_hw->server() )
        m_hw->server()->disconnect();

    delete m_project;
    m_project = 0;

    ui->actSaveProject->setEnabled(false);
    ui->actSaveProjectAs->setEnabled(false);
    ui->actCloseProject->setEnabled(false);
    ui->actImportAutomaton->setEnabled(false);
    ui->menuAutomata->setEnabled(false);
    ui->menuServer->setEnabled(false);
    ui->actShowSettings->setEnabled(false);

    return true;
}

void MainWindow::importAutomaton()
{
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Import Automaton"),
                                         QDir::currentPath(),
                                         tr("Automaton Files (*.xml *.fsm)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("DES Automaton Importer"),
                             tr("Cannot open file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    ImportAutomaton importer;

    QList<DCAutomaton*> automatonList = importer.loadAutomaton(ImportAutomaton::SUPREMICA, &file);

    if(!automatonList.isEmpty())
    {
        statusBar()->showMessage(tr("Automaton imported"), 20000);

        foreach(DCAutomaton* automaton, automatonList)
        {
            m_project->addAutomaton(automaton);
        }

        automatonList.first()->doLayout();
        m_automatonWidget->openAutomaton(automatonList.first());

    }
    else
    {
        QMessageBox::critical(this, tr("Could not import Automaton"), tr("Importer failed") );
    }
}

void MainWindow::deleteAutomaton()
{
    DCAutomaton *delAutomaton = m_automatonWidget->currentAutomaton();

    if(delAutomaton) {
        m_automatonWidget->closeTab(delAutomaton);
        m_project->removeAutomaton(delAutomaton);
    }

}

void MainWindow::editHWConnection()
{
    DCAutomaton *automaton = m_automatonWidget->currentAutomaton();

    if(automaton) {
        HWConnections hwc(m_hw, automaton);
        hwc.exec();
    }
}

void MainWindow::editAutomaton()
{
    m_controller->stopController();

    if(m_automatonWidget->currentAutomaton())
    {
        m_automatonWidget->currentAutomaton()->setSceneMode(DCAutomaton::Edit);
        ui->actStartController->setEnabled(false);
        ui->actStopController->setEnabled(false);
        ui->actPauseController->setEnabled(false);

        m_projectWidget->setMode("edit mode");
    }
    else
        return;
}

void MainWindow::runAutomaton()
{

    m_controller->stopController();

    if(m_controller->automaton())
    {
        m_controller->automaton()->setSceneMode(DCAutomaton::Edit);
    }

    if(m_automatonWidget->currentAutomaton())
    {
        m_controller->setAutomaton(m_automatonWidget->currentAutomaton());

        m_controller->setMode(DCController::Live);
        ui->actStartController->setEnabled(true);
        ui->actStopController->setEnabled(true);
        ui->actPauseController->setEnabled(true);
        ui->actStopController->setChecked(true);

        m_projectWidget->setMode("Hardware mode");

        if(!ui->actDisconnectFromServer->isEnabled()) {
            connectToServer();
        }
    }
    else
    {
        QMessageBox::warning(this, tr("DES Controller"),
                             tr("No Automaton selected for the controller.\n\n Please open one Automaton first"));
    }
}

void MainWindow::simulateAutomaton()
{

    m_controller->stopController();

    if(m_controller->automaton())
    {
        m_controller->automaton()->setSceneMode(DCAutomaton::Edit);
    }

    if(m_automatonWidget->currentAutomaton())
    {
        m_automatonWidget->currentAutomaton()->setSceneMode(DCAutomaton::Edit);
        m_controller->setAutomaton(m_automatonWidget->currentAutomaton());

        m_controller->setMode(DCController::Simulation);
        ui->actStartController->setEnabled(true);
        ui->actStopController->setEnabled(true);
        ui->actPauseController->setEnabled(true);
        ui->actStopController->setChecked(true);

        m_projectWidget->setMode("Simulation mode");
    }
    else
    {
        QMessageBox::warning(this, tr("DES Controller"),
                             tr("No Automaton selected for the controller.\n\n Please open one Automaton first"));
    }
}

void MainWindow::showEventStatus()
{
    m_es->setAutomaton(m_automatonWidget->currentAutomaton());

    m_es->show();
}

void MainWindow::connectToServer()
{
    m_hw->server()->connectSRCP();
}

void MainWindow::connectedToServer()
{
    ui->actDisconnectFromServer->setEnabled(true);
    ui->actStartTrains->setEnabled(true);
    ui->actStopTrains->setEnabled(true);
    statusBar()->showMessage(tr("Connection to SRCP Server established"), 20000);

    m_hw->initializeDevices();
}

void MainWindow::disconnectFromServer()
{
    m_hw->server()->disconnectSRCP();
}

void MainWindow::disconnectedFromServer()
{
    ui->actDisconnectFromServer->setEnabled(false);
    ui->actStartTrains->setEnabled(false);
    ui->actStopTrains->setEnabled(false);
    statusBar()->showMessage(tr("Connection to SRCP Server lost"), 20000);
}

void MainWindow::showHWDebugConsole()
{
    HHDebugDialog dbg(m_hw);
    dbg.exec();
}

void MainWindow::toggleStatusBar()
{
    if(ui->statusBar->isVisible())
        ui->statusBar->hide();
    else
        ui->statusBar->show();
}

void MainWindow::showProjectSettings()
{
    SettingsDialog setting(m_hw);
    setting.exec();
}

void MainWindow::showHandbook()
{

}

void MainWindow::aboutDES()
{
    QMessageBox::about(this, tr("About DES Control"),
                       tr("Visualisation and controller implementation of DES Automaton\nHardware connection via srcp server to Märklin trains and switches.\n\nAuthor: Jörg Ehrichs"));
}

void MainWindow::showProjectView()
{
    if(!m_project)
        return;

    delete m_projectWidget;
    m_projectWidget = new ProjectWidget();
    m_projectWidget->setProject(m_project);

    connect(m_controller, SIGNAL(curState(DCState*)), m_projectWidget, SLOT(curState(DCState*)));

    QMenu * contextMenu = new QMenu();
    contextMenu->addAction(ui->actEditHWConnection);
    contextMenu->addAction(ui->actDeleteAutomaton);

    m_projectWidget->setContextMenu(contextMenu);

    delete m_splitter;
    m_splitter = new QSplitter();
    m_splitter->addWidget(m_projectWidget);

    m_automatonWidget = new AutomatonWidget();
    m_splitter->addWidget(m_automatonWidget);
    setCentralWidget(m_splitter);
    m_splitter->setStretchFactor(0,1);
    m_splitter->setStretchFactor(1,2);

    connect(m_projectWidget->automatonList(), SIGNAL(openAutomaton(DCAutomaton*)),
            m_automatonWidget, SLOT(openAutomaton(DCAutomaton*)));

    connect(m_automatonWidget, SIGNAL(firstAutomatonOpend()), this, SLOT(someAutomataVisible()));
    connect(m_automatonWidget, SIGNAL(lastAutomatonClosed()), this, SLOT(noAutomatonVisible()));
    connect(m_automatonWidget, SIGNAL(switchOpendAutomaton(DCAutomaton::SceneMode)), this, SLOT(switchOpendAutomaton(DCAutomaton::SceneMode)));
}

void MainWindow::showEmptyView()
{
    QWidget *intro = new QWidget();
    setCentralWidget(intro);
    delete m_projectWidget;
    m_projectWidget = 0;
    delete m_automatonWidget;
    m_automatonWidget = 0;
    delete m_splitter;
    m_splitter = 0;

    if(m_controller &&  m_projectWidget)
        disconnect(m_controller, SIGNAL(curState(DCState*)), m_projectWidget, SLOT(curState(DCState*)));
}

void MainWindow::createActions()
{
    m_autonamtonMode = new QActionGroup(this);
    m_autonamtonMode->setExclusive(true);
    m_autonamtonMode->addAction(ui->actEditAutomaton);
    m_autonamtonMode->addAction(ui->actRunSimulation);
    m_autonamtonMode->addAction(ui->actRunwithHardware);
    ui->actEditAutomaton->setChecked(true);
    ui->menuAutomata->setEnabled(false);
    ui->actDeleteAutomaton->setEnabled(false);
    ui->actImportAutomaton->setEnabled(false);
    ui->actEditHWConnection->setEnabled(false);

    m_controllerState= new QActionGroup(this);
    m_controllerState->setExclusive(true);
    m_controllerState->addAction(ui->actStartController);
    m_controllerState->addAction(ui->actStopController);
    m_controllerState->addAction(ui->actPauseController);
    ui->actStopController->setChecked(true);


    ui->menuToggleToolBars->addAction(ui->mainToolBar->toggleViewAction());
    ui->menuToggleToolBars->addAction(ui->controllerToolBar->toggleViewAction());

    connect(ui->actAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    connect(ui->actStartController, SIGNAL(triggered()), m_controller, SLOT(startController()));
    connect(ui->actStopController, SIGNAL(triggered()), m_controller, SLOT(stopController()));
    connect(ui->actPauseController, SIGNAL(triggered(bool)), m_controller, SLOT(pauseController(bool)));
}

void MainWindow::createToolBars()
{
    ui->mainToolBar->addAction(ui->actNewProject);
    ui->mainToolBar->addAction(ui->actSaveProject);
    ui->mainToolBar->addAction(ui->actCloseProject);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->actImportAutomaton);
    ui->mainToolBar->addAction(ui->actEditHWConnection);
    ui->mainToolBar->addAction(ui->actDeleteAutomaton);

    ui->controllerToolBar->addAction(ui->actEditAutomaton);
    ui->controllerToolBar->addAction(ui->actRunSimulation);
    ui->controllerToolBar->addAction(ui->actRunwithHardware);
    ui->controllerToolBar->addSeparator();
    ui->controllerToolBar->addAction(ui->actStartController);
    ui->controllerToolBar->addAction(ui->actStopController);
    ui->controllerToolBar->addAction(ui->actPauseController);
}

void MainWindow::unsavedChanges()
{
    m_unsavedChanges = true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(closeProject())
    {
        writeSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::noAutomatonVisible()
{
    m_controller->stopController();
    m_controller->setAutomaton(0);

    ui->menuController->setEnabled(false);
    ui->actEditAutomaton->setEnabled(false);
    ui->actRunwithHardware->setEnabled(false);
    ui->actRunSimulation->setEnabled(false);
    ui->actStartController->setEnabled(false);
    ui->actStopController->setEnabled(false);
    ui->actPauseController->setEnabled(false);
    ui->actEditHWConnection->setEnabled(false);
}

void MainWindow::someAutomataVisible()
{
    ui->menuController->setEnabled(true);
    ui->actEditAutomaton->setEnabled(true);
    ui->actRunwithHardware->setEnabled(true);
    ui->actRunSimulation->setEnabled(true);
    ui->actEditHWConnection->setEnabled(true);
}

void MainWindow::switchOpendAutomaton(DCAutomaton::SceneMode currentMode)
{
    switch(currentMode)
    {
    case DCAutomaton::Edit:
        ui->actEditAutomaton->setChecked(true);
        ui->actStopController->setChecked(true);
        ui->actStartController->setEnabled(false);
        ui->actStopController->setEnabled(false);
        ui->actPauseController->setEnabled(false);
        break;
    case DCAutomaton::Run:
        if(m_controller->mode() == DCController::Live)
            ui->actRunwithHardware->setChecked(true);
        else
            ui->actRunSimulation->setChecked(true);

        ui->actStartController->setEnabled(true);
        ui->actStopController->setEnabled(true);
        ui->actPauseController->setEnabled(true);

        if(m_controller->isRunnung())
            ui->actStartController->setChecked(true);
        else
            if(m_controller->isPaused())
                ui->actPauseController->setChecked(true);
            else
                ui->actStopController->setChecked(true);
        break;

    }
}

void MainWindow::printSessionHistory(const QString & msg)
{
    ui->textEdit->append(msg);
}

