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
#include "projectserializer.h"

#include "dcsensor.h"
#include "dcactuator.h"
#include "dctrain.h"

#include <QtGui/QSplitter>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        m_projectView(0),
        m_splitter(0)
{
    ui->setupUi(this);

    showEmptyView();
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

    showProjectView();
}

void MainWindow::openProject()
{
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
        statusBar()->showMessage(tr("File opened"), 2000);
        m_project->setFileName(fileName);

        connect (m_project->server(), SIGNAL(connectionClosed()), this, SLOT(disconnectServer()));
        connect (m_project->server(), SIGNAL(connectionEstablished()), this, SLOT(connectedToServer()));

        showProjectView();

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
            statusBar()->showMessage(tr("File saved"), 2000);
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
        statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::closeProject()
{

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

void MainWindow::showProjectView()
{
    if(!m_project)
        return;

    delete m_projectView;
    m_projectView = new ProjectTreeView();
    m_projectView->setProject(m_project);

    ui->menu_Server->setEnabled(true);
    ui->menu_Power->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->actionSave_As->setEnabled(true);

    m_splitter = new QSplitter();
    m_splitter->addWidget(m_projectView);
    //m_splitter->addWidget(ui->graphicsView);
    setCentralWidget(m_splitter);
}

void MainWindow::showEmptyView()
{
    QWidget *intro = new QWidget();
    setCentralWidget(intro);
    delete m_splitter;
    delete m_projectView;
}
