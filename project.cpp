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

#include "project.h"

#include "srcp/dcserver.h"
#include "srcp/dctrain.h"
#include "srcp/dcactuator.h"
#include "srcp/dcsensor.h"
#include "des/dcautomaton.h"

#include <QIODevice>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

Project::Project()
    : QObject(0)
    , m_name(tr("default project"))
    , m_description(tr("default DES-Control project"))
    , m_filename(QString())
    , m_server(0)
{
}

void Project::setName(const QString& name)
{
    m_name = name;
    emit projectChanged();
}

QString Project::name() const
{
    return m_name;
}

void Project::setDescription(const QString& description)
{
    m_description = description;

    emit projectChanged();
}

QString Project::description() const
{
    return m_description;
}

void Project::setFileName(const QString& name)
{
    m_filename = name;
}

QString Project::fileName() const
{
    return m_filename;
}

void Project::setServer(DCServer *newServer)
{
    delete m_server;        // clean up and delete old server instance
    m_server = newServer;

    emit projectChanged();
}

DCServer* Project::server() const
{
    return m_server;
}

void Project::addTrain(DCTrain* newTrain)
{
    connect(newTrain, SIGNAL(sendSRCPString(QString)), m_server, SLOT(sendSRCP(QString)));
    connect(newTrain, SIGNAL(itemChanged()), this, SIGNAL(projectChanged()));
    connect(newTrain, SIGNAL(itemChanged()), this, SIGNAL(updateTrains()));
    m_trains.append(newTrain);
    emit projectChanged();
    emit updateTrains();
}

QList<DCTrain*> Project::trains() const
{
    return m_trains;
}

void Project::removeTrain(DCTrain* train)
{
    disconnect(train, SIGNAL(sendSRCPString(QString)), m_server, SLOT(sendSRCP(QString)));
    m_trains.removeAll(train);
    delete train;

    emit projectChanged();
    emit updateTrains();
}

void Project::removeTrain(const QString & trainName)
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

void Project::addActuator(DCActuator* newActuator)
{
    connect(newActuator, SIGNAL(sendSRCPString(QString)), m_server, SLOT(sendSRCP(QString)));
    connect(newActuator, SIGNAL(itemChanged()), this, SIGNAL(projectChanged()));
    connect(newActuator, SIGNAL(itemChanged()), this, SIGNAL(updateActuators()));
    m_actuators.append(newActuator);
    emit projectChanged();
    emit updateActuators();
}

QList<DCActuator*> Project::actuators() const
{
    return m_actuators;
}

void Project::removeActuator(DCActuator* actuator)
{
    disconnect(actuator, SIGNAL(sendSRCPString(QString)), m_server, SLOT(sendSRCP(QString)));
    m_actuators.removeAll(actuator);
    delete actuator;

    emit projectChanged();
    emit updateActuators();
}

void Project::removeActuator(const QString & actuatorName)
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

void Project::addSensor(DCSensor* newSensor)
{
    connect(newSensor, SIGNAL(sendSRCPString(QString)), m_server, SLOT(sendSRCP(QString)));
    connect(newSensor, SIGNAL(itemChanged()), this, SIGNAL(projectChanged()));
    connect(newSensor, SIGNAL(itemChanged()), this, SIGNAL(updateSensors()));
    m_sensors.append(newSensor);
    emit projectChanged();
    emit updateSensors();
}

QList<DCSensor*> Project::sensors() const
{
    return m_sensors;
}

void Project::removeSensor(DCSensor* sensor)
{
    disconnect(sensor, SIGNAL(sendSRCPString(QString)), m_server, SLOT(sendSRCP(QString)));
    m_sensors.removeAll(sensor);
    delete sensor;

    emit projectChanged();
    emit updateSensors();
}

void Project::removeSensor(const QString & sensorName)
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

void Project::addAutomaton(DCAutomaton* newAutomaton)
{
    m_automata.append(newAutomaton);
    emit projectChanged();
    emit updateAutomata();
}

void Project::removeAutomaton(DCAutomaton* automaton)
{
    m_automata.removeAll(automaton);
    delete automaton;

    emit projectChanged();
    emit updateAutomata();
}

QList<DCAutomaton*> Project::automata() const
{
    return m_automata;
}

void Project::initializeDevices()
{
    foreach(DCTrain *train, m_trains)
    {
        train->initialize();
    }
    foreach(DCActuator *actuator, m_actuators)
    {
        actuator->initialize();
    }
    //DEBUG !! SENSOR ARE NOT INITIALIZED YET!!!!!!!
    foreach(const DCSensor *sensor, m_sensors)
    {
        // skipped in local mode as it creates errors for now
        //sensor->initialize();
    }
}
