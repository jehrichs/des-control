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

#ifndef PROJECTSERIALIZER_H
#define PROJECTSERIALIZER_H

#include <QObject>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class Project;
class QIODevice;

class ProjectSerializer : public QObject
{
    Q_OBJECT
public:
    explicit ProjectSerializer(QObject *parent = 0);

    bool saveProject(Project* project, QIODevice *device);

    Project* loadProject(QIODevice *device);

private:
    void readFile();
    void readProject();

    QXmlStreamWriter writer;
    QXmlStreamReader reader;
    Project *m_project;

};

#endif // PROJECTSERIALIZER_H
