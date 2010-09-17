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

#include "projectserializer.h"

#include "project.h"
#include "dcserver.h"
#include "dctrain.h"
#include "dcsensor.h"
#include "dcactuator.h"

#include <QIODevice>
ProjectSerializer::ProjectSerializer(QObject *parent)
    : QObject(parent)
    , m_project(0)
{
}

bool ProjectSerializer::saveProject(Project* project, QIODevice *device)
{
    writer.setDevice(device);
    writer.setAutoFormatting(true);

    writer.writeStartDocument();
    writer.writeDTD("<!DOCTYPE descontrol>");
    writer.writeStartElement("descontrol");
    writer.writeAttribute("version", "1.0");

    // store project information
    writer.writeStartElement("project");
    writer.writeTextElement("name", project->name());
    writer.writeTextElement("description", project->description());
    writer.writeEndElement();

    //store server info
    writer.writeStartElement("srcpserver");
    writer.writeAttribute("version", "0.8.4");
    writer.writeTextElement("host", project->server()->ip());
    writer.writeTextElement("port", project->server()->port());
    writer.writeEndElement();

    // store sensors
    writer.writeStartElement("sensors");
    foreach(DCSensor *sensor, project->sensors())
    {
        writer.writeStartElement("sensor");
        writer.writeAttribute("name", sensor->name());
        writer.writeTextElement("bus", QString("%1").arg(sensor->busID()));
        writer.writeTextElement("addr", QString("%1").arg(sensor->address()));
        writer.writeEndElement();
    }
    writer.writeEndElement();

    // store actuators
    writer.writeStartElement("actuators");
    foreach(DCActuator *actuator, project->actuators())
    {
        writer.writeStartElement("actuator");
        writer.writeAttribute("name", actuator->name());
        writer.writeTextElement("bus", QString("%1").arg(actuator->busID()));
        writer.writeTextElement("addr", QString("%1").arg(actuator->address()));
        writer.writeTextElement("port", QString("%1").arg(actuator->port()));
        writer.writeTextElement("protocol", QString("%1").arg(actuator->protocol()));
        writer.writeEndElement();
    }
    writer.writeEndElement();

    // store trains
    writer.writeStartElement("trains");
    foreach(DCTrain *train, project->trains())
    {
        writer.writeStartElement("train");
        writer.writeAttribute("name", train->name());
        writer.writeTextElement("bus", QString("%1").arg(train->busID()));
        writer.writeTextElement("addr", QString("%1").arg(train->address()));
        writer.writeTextElement("protocol", QString("%1").arg(train->protocol()));
        writer.writeTextElement("decodersteps", QString("%1").arg(train->decoderSteps()));
        writer.writeTextElement("decoderfunctions", QString("%1").arg(train->decoderFunctions()));
        writer.writeEndElement();
    }
    writer.writeEndElement();

    writer.writeEndDocument();

    return true;
}

Project* ProjectSerializer::loadProject(QIODevice *device)
{
    reader.setDevice(device);

    if (reader.readNextStartElement()) {
        if (reader.name() == "descontrol" && reader.attributes().value("version") == "1.0")
            readFile();
        else
            reader.raiseError(QObject::tr("The file is not an DES-Control version 1.0 file."));
    }

    return m_project;
}

void ProjectSerializer::readFile()
{
    //seems it was a valied file, create empty project and fill it
    m_project = new Project();

    while (reader.readNextStartElement()) {
        if (reader.name() == "project")
            readProject();
        else if (reader.name() == "srcpserver")
            readServer();
        else if (reader.name() == "sensors")
            readSensors();
        else if (reader.name() == "actuators")
            readActuators();
        else if (reader.name() == "trains")
            readTrains();
        else
            reader.skipCurrentElement();
    }
}

void ProjectSerializer::readProject()
{
    Q_ASSERT(reader.isStartElement() && reader.name() == "project");

    while (reader.readNextStartElement()) {
        if (reader.name() == "name")
            m_project->setName(reader.readElementText());
        else if (reader.name() == "description")
            m_project->setDescription(reader.readElementText());
        else
            reader.skipCurrentElement();
    }
}

void ProjectSerializer::readServer()
{
    Q_ASSERT(reader.isStartElement() && reader.name() == "srcpserver");

    DCServer *server = new DCServer();

    while (reader.readNextStartElement()) {
        if (reader.name() == "host")
            server->setHost(reader.readElementText());
        else if (reader.name() == "port")
            server->setPort(reader.readElementText());
        else
            reader.skipCurrentElement();
    }

    m_project->setServer(server);
}

void ProjectSerializer::readSensors()
{
    Q_ASSERT(reader.isStartElement() && reader.name() == "sensors");

    // all sensors
    while (reader.readNextStartElement()) {

        if(reader.name() == "sensor")
        {
            // single sensor
            DCSensor *sensor = new DCSensor();
            sensor->setName(reader.attributes().value("name").toString());

            while (reader.readNextStartElement()) {
                if (reader.name() == "bus")
                    sensor->setBusID(reader.readElementText().toInt());
                else if (reader.name() == "addr")
                    sensor->setAddress(reader.readElementText().toInt());
                else
                    reader.skipCurrentElement();
            }

            m_project->addSensor(sensor);
        }
        else
            reader.skipCurrentElement();
    }

}

void ProjectSerializer::readActuators()
{
    Q_ASSERT(reader.isStartElement() && reader.name() == "actuators");

    // all actuators
    while (reader.readNextStartElement()) {

        if(reader.name() == "actuator")
        {
            // single actuator
            DCActuator *actuator = new DCActuator();
            actuator->setName(reader.attributes().value("name").toString());

            while (reader.readNextStartElement()) {
                if (reader.name() == "bus")
                    actuator->setBusID(reader.readElementText().toInt());
                else if (reader.name() == "addr")
                    actuator->setAddress(reader.readElementText().toInt());
                else if (reader.name() == "port")
                    actuator->setPort(reader.readElementText().toInt());
                else if (reader.name() == "protocol")
                    actuator->setProtocol((DCActuator::GAProtocol)reader.readElementText().toInt());
                else
                    reader.skipCurrentElement();
            }

            m_project->addActuator(actuator);
        }
        else
            reader.skipCurrentElement();
    }
}

void ProjectSerializer::readTrains()
{
    Q_ASSERT(reader.isStartElement() && reader.name() == "trains");

    // all trains
    while (reader.readNextStartElement()) {

        if(reader.name() == "train")
        {
            // single train
            DCTrain *train = new DCTrain();
            train->setName(reader.attributes().value("name").toString());

            while (reader.readNextStartElement()) {
                if (reader.name() == "bus")
                    train->setBusID(reader.readElementText().toInt());
                else if (reader.name() == "addr")
                    train->setAddress(reader.readElementText().toInt());
                else if (reader.name() == "protocol")
                    train->setProtocol((DCTrain::GLProtocol)reader.readElementText().toInt());
                else if (reader.name() == "decodersteps")
                    train->setDecoderSteps(reader.readElementText().toInt());
                else if (reader.name() == "decoderfunctions")
                    train->setDecoderFunctions(reader.readElementText().toInt());
                else
                    reader.skipCurrentElement();
            }

            m_project->addTrain(train);
        }
        else
            reader.skipCurrentElement();
    }
}
