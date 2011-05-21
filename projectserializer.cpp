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
#include "srcp/dcserver.h"
#include "srcp/dctrain.h"
#include "srcp/dcsensor.h"
#include "srcp/dcactuator.h"

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
