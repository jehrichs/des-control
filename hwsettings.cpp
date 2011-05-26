/*
 * Copyright 2011 Jörg Ehrichs <joerg.ehichs@gmx.de>
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

#include "hwsettings.h"

#include "srcp/dcserver.h"
#include "srcp/dctrain.h"
#include "srcp/dcactuator.h"
#include "srcp/dcsensor.h"

HWSettings::HWSettings()
    : QObject(0),
      m_server(0)
{
}

void HWSettings::setServer(DCServer *newServer)
{
    delete m_server;        // clean up and delete old server instance
    m_server = newServer;
}

DCServer* HWSettings::server() const
{
    return m_server;
}

void HWSettings::addTrain(DCTrain* newTrain)
{
    connect(newTrain, SIGNAL(sendSRCPString(QString)), m_server, SLOT(sendSRCP(QString)));
    connect(newTrain, SIGNAL(itemChanged()), this, SIGNAL(updateTrains()));
    m_trains.append(newTrain);

    emit updateTrains();
}

QList<DCTrain*> HWSettings::trains() const
{
    return m_trains;
}

void HWSettings::removeTrain(DCTrain* train)
{
    disconnect(train, SIGNAL(sendSRCPString(QString)), m_server, SLOT(sendSRCP(QString)));
    m_trains.removeAll(train);
    delete train;

    emit updateTrains();
}

void HWSettings::removeTrain(const QString & trainName)
{
    foreach(DCTrain* train, m_trains)
    {
        if(train->name() == trainName)
        {
            removeTrain(train);
            return;
        }
    }
}

void HWSettings::stopTrains()
{
    foreach(DCTrain* train, m_trains)
    {
        train->setSpeed(0);
        train->sendValue();
    }
}

void HWSettings::startTrains()
{
    foreach(DCTrain* train, m_trains)
    {
        train->setSpeed(100);
        train->setmaxSpeed(200);
        train->setDriveMode(DCTrain::FORWARD);
        train->sendValue();
    }
}

void HWSettings::addActuator(DCActuator* newActuator)
{
    connect(newActuator, SIGNAL(sendSRCPString(QString)), m_server, SLOT(sendSRCP(QString)));
    connect(newActuator, SIGNAL(itemChanged()), this, SIGNAL(updateActuators()));
    m_actuators.append(newActuator);

    emit updateActuators();
}

QList<DCActuator*> HWSettings::actuators() const
{
    return m_actuators;
}

void HWSettings::removeActuator(DCActuator* actuator)
{
    disconnect(actuator, SIGNAL(sendSRCPString(QString)), m_server, SLOT(sendSRCP(QString)));
    m_actuators.removeAll(actuator);
    delete actuator;

    emit updateActuators();
}

void HWSettings::removeActuator(const QString & actuatorName)
{
    foreach(DCActuator* actuator, m_actuators)
    {
        if(actuator->name() == actuatorName)
        {
            removeActuator(actuator);
            return;
        }
    }
}

void HWSettings::addSensor(DCSensor* newSensor)
{
    connect(newSensor, SIGNAL(sendSRCPString(QString)), m_server, SLOT(sendSRCP(QString)));
    connect(newSensor, SIGNAL(itemChanged()), this, SIGNAL(updateSensors()));
    m_sensors.append(newSensor);

    emit updateSensors();
}

QList<DCSensor*> HWSettings::sensors() const
{
    return m_sensors;
}

void HWSettings::removeSensor(DCSensor* sensor)
{
    disconnect(sensor, SIGNAL(sendSRCPString(QString)), m_server, SLOT(sendSRCP(QString)));
    m_sensors.removeAll(sensor);
    delete sensor;

    emit updateSensors();
}

void HWSettings::removeSensor(const QString & sensorName)
{
    foreach(DCSensor* sensor, m_sensors)
    {
        if(sensor->name() == sensorName)
        {
            removeSensor(sensor);
            return;
        }
    }
}

void HWSettings::initializeDevices()
{
    foreach(DCTrain *train, m_trains)
    {
        train->initialize();
    }
    foreach(DCActuator *actuator, m_actuators)
    {
        actuator->initialize();
    }
}
