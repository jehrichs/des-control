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

#include "hardwaretreewidget.h"

#include "project.h"
#include "srcp/dctrain.h"
#include "srcp/dcsensor.h"
#include "srcp/dcactuator.h"

#include "srcp/hardwaresettings.h"

#include <QTreeWidgetItem>

HardwareTreeWidget::HardwareTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
    , m_project(0)
{
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this, SLOT(openConfig(QTreeWidgetItem*,int)));
}

void HardwareTreeWidget::setProject(Project *newProject)
{
    m_project = newProject;
    connect (m_project, SIGNAL(updateTrains()), this, SLOT(updateView()));
    connect (m_project, SIGNAL(updateSensors()), this, SLOT(updateView()));
    connect (m_project, SIGNAL(updateActuators()), this, SLOT(updateView()));

    updateView();
}

void HardwareTreeWidget::updateView()
{
    clear();

    //trains
    hardwareTrainHeader = new QTreeWidgetItem(TrainHeader);
    hardwareTrainHeader->setText(0,tr("Trains"));
    insertTopLevelItem(0,hardwareTrainHeader);

    updateTrains();

    //actuators
    hardwareActuatorHeader = new QTreeWidgetItem(ActuartorHeader);
    hardwareActuatorHeader->setText(0,tr("Actuators"));
    insertTopLevelItem(0,hardwareActuatorHeader);

    updateActuators();

    //sensors
    hardwareSensorHeader = new QTreeWidgetItem(SensorHeader);
    hardwareSensorHeader->setText(0,tr("Sensors"));
    insertTopLevelItem(0,hardwareSensorHeader);

    updateSensors();
}

void HardwareTreeWidget::updateTrains()
{
    hardwareTrainHeader->takeChildren();

    foreach(DCTrain *train, m_project->trains())
    {
        QTreeWidgetItem *trainItem = new QTreeWidgetItem(Train);
        trainItem->setText(0,train->name());
        hardwareTrainHeader->insertChild(0,trainItem);
    }
}

void HardwareTreeWidget::updateSensors()
{
    hardwareSensorHeader->takeChildren();

    foreach(DCSensor *sensor, m_project->sensors())
    {
        QTreeWidgetItem *sensorItem = new QTreeWidgetItem(Actuator);
        sensorItem->setText(0,sensor->name());
        hardwareSensorHeader->insertChild(0,sensorItem);
    }
}

void HardwareTreeWidget::updateActuators()
{
    hardwareActuatorHeader->takeChildren();

    foreach(DCActuator *actuator, m_project->actuators())
    {
        QTreeWidgetItem *actuatorItem = new QTreeWidgetItem(Sensor);
        actuatorItem->setText(0,actuator->name());
        hardwareActuatorHeader->insertChild(0,actuatorItem);
    }
}

void HardwareTreeWidget::openConfig( QTreeWidgetItem * item, int column )
{
    HardwareSettings hwSettings(m_project);

    hwSettings.showPage(item->type());
    hwSettings.selectItem();

    hwSettings.exec();

}
