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

#include "projecttreeview.h"

#include "project.h"
#include "dctrain.h"
#include "dcactuator.h"
#include "dcsensor.h"
#include "dcautomaton.h"

#include "actuatorsettings.h"
#include "sensorsettings.h"
#include "trainsettings.h"

#include <QTreeWidgetItem>
#include <QMenu>
#include <QContextMenuEvent>
#include <QAction>
#include <QList>
#include <QDialog>
#include <QLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>

#include <QDebug>

ProjectTreeView::ProjectTreeView(QWidget *parent)
    : QTreeWidget(parent)
    , m_project(0)
{
    createActions();
}

void ProjectTreeView::createActions()
{
    m_addTrain = new QAction(tr("Add New Train"), this);
    m_addTrain->setStatusTip(tr("Add a new Train to the project"));
    m_addTrain->setData(Train);
    connect(m_addTrain, SIGNAL(triggered()), this, SLOT(openHardwareList()));
    m_addSensor = new QAction(tr("Add New Sensor"), this);
    m_addSensor->setStatusTip(tr("Add a new Sensor to the project"));
    m_addSensor->setData(Sensor);
    connect(m_addSensor, SIGNAL(triggered()), this, SLOT(openHardwareList()));
    m_addActuator = new QAction(tr("Add New Actuator"), this);
    m_addActuator->setStatusTip(tr("Add a new Actuator to the project"));
    m_addActuator->setData(Actuator);
    connect(m_addActuator, SIGNAL(triggered()), this, SLOT(openHardwareList()));
}

void ProjectTreeView::setProject(Project *newProject)
{
    m_project = newProject;

    QTreeWidgetItem *header = headerItem();
    header->setText(0,m_project->name());

    //connect (m_project, SIGNAL(projectChanged()), this, SLOT(updateTreeView()));
    connect (m_project, SIGNAL(updateTrains()), this, SLOT(updateTrains()));
    connect (m_project, SIGNAL(updateSensors()), this, SLOT(updateSensors()));
    connect (m_project, SIGNAL(updateActuators()), this, SLOT(updateActuators()));
    connect (m_project, SIGNAL(updateAutomata()), this, SLOT(updateAutomata()));

    updateTreeView();
}

void ProjectTreeView::updateTreeView()
{
    clear();

    //show hardware
    QTreeWidgetItem *hardwareHeader = new QTreeWidgetItem(HardwareHeader);
    hardwareHeader->setText(0,"Hardware");
    insertTopLevelItem(0,hardwareHeader);

    //trains
    hardwareTrainHeader = new QTreeWidgetItem(TrainHeader);
    hardwareTrainHeader->setText(0,"Trains");
    hardwareHeader->insertChild(0,hardwareTrainHeader);

    //actuators
    hardwareActuatorHeader = new QTreeWidgetItem(ActuartorHeader);
    hardwareActuatorHeader->setText(0,"Actuator");
    hardwareHeader->insertChild(0,hardwareActuatorHeader);

    //sensors
    hardwareSensorHeader = new QTreeWidgetItem(SensorHeader);
    hardwareSensorHeader->setText(0,"Sensor");
    hardwareHeader->insertChild(0,hardwareSensorHeader);

    //show automata
    automataHeader = new QTreeWidgetItem(AutomataHeader);
    automataHeader->setText(0,"Automata");
    insertTopLevelItem(0,automataHeader);

    updateTrains();
    updateSensors();
    updateActuators();
    updateAutomata();
}

void ProjectTreeView::updateTrains()
{
    hardwareTrainHeader->takeChildren();

    foreach(DCTrain *train, m_project->trains())
    {
        QTreeWidgetItem *trainItem = new QTreeWidgetItem(Train);
        trainItem->setText(0,train->name());
        hardwareTrainHeader->insertChild(0,trainItem);
    }
}

void ProjectTreeView::updateSensors()
{
    hardwareSensorHeader->takeChildren();

    foreach(DCSensor *sensor, m_project->sensors())
    {
        QTreeWidgetItem *sensorItem = new QTreeWidgetItem(Sensor);
        sensorItem->setText(0,sensor->name());
        hardwareSensorHeader->insertChild(0,sensorItem);
    }
}

void ProjectTreeView::updateActuators()
{
    hardwareActuatorHeader->takeChildren();

    foreach(DCActuator *actuator, m_project->actuators())
    {
        QTreeWidgetItem *actuatorItem = new QTreeWidgetItem(Actuator);
        actuatorItem->setText(0,actuator->name());
        hardwareActuatorHeader->insertChild(0,actuatorItem);
    }
}

void ProjectTreeView::updateAutomata()
{
    hardwareActuatorHeader->takeChildren();

    foreach(DCAutomaton *automaton, m_project->automata())
    {
        QTreeWidgetItem *automatonItem = new QTreeWidgetItem(Plant);
        automatonItem->setText(0,automaton->name());
        automataHeader->insertChild(0,automatonItem);
    }
}

void ProjectTreeView::openHardwareList()
{
    QAction *action = dynamic_cast<QAction*>(sender());


    QDialog *settingsDialog = new QDialog(this);
    settingsDialog->setLayout(new QVBoxLayout());

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttonBox, SIGNAL(rejected()), settingsDialog, SLOT(accept()));

    switch(action->data().toInt())
    {
    case Train:
        {
            TrainSettings *ts = new TrainSettings(m_project);
            settingsDialog->layout()->addWidget(ts);
            settingsDialog->layout()->addWidget(buttonBox);
            settingsDialog->exec();

            delete ts;
        }
        break;

    case Sensor:
        {
            SensorSettings *ss = new SensorSettings(m_project);
            settingsDialog->layout()->addWidget(ss);
            settingsDialog->layout()->addWidget(buttonBox);
            settingsDialog->exec();

            delete ss;
        }
        break;

    case Actuator:
        {
            ActuatorSettings *as = new ActuatorSettings(m_project);
            settingsDialog->layout()->addWidget(as);
            settingsDialog->layout()->addWidget(buttonBox);
            settingsDialog->exec();

            delete as;
        }
        break;

        //    default:
        //        qDebug() << "wrong hardware dialoge requested";
        //        return;
    }

    delete buttonBox;
    delete settingsDialog;
}

void ProjectTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    QTreeWidgetItem *clickedItem = itemAt(event->pos());

    if(!clickedItem)
        return;

    QList<QAction *> actions;
    QAction *seperator = new QAction(this);
    seperator->setSeparator(true);

    switch(clickedItem->type())
    {
    case HardwareHeader:
        //skip
        break;
    case AutomataHeader:
        break;
    case TrainHeader:
        actions.append(m_addTrain);
        break;
    case Train:
        actions.append(m_addTrain);
        actions.append(seperator);
        actions.append(new QAction("Edit Train", this));
        actions.append(new QAction("Remove Train", this));
        break;

    case ActuartorHeader:
        actions.append(m_addActuator);
        break;
    case Actuator:
        actions.append(m_addActuator);
        actions.append(seperator);
        actions.append(new QAction("Edit Actuator", this));
        actions.append(new QAction("Remove Actuator", this));
        break;
    case SensorHeader:
        actions.append(m_addSensor);
        break;
    case Sensor:
        actions.append(m_addSensor);
        actions.append(seperator);
        actions.append(new QAction("Edit Sensor", this));
        actions.append(new QAction("Remove Sensor", this));
        break;

    }

    if (actions.count() > 0)
        QMenu::exec(actions, event->globalPos());
}
