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

#include "des/dcautomaton.h"

#include <QIODevice>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

Project::Project()
    : QObject(0)
    , m_name(tr("default project"))
    , m_description(tr("default DES-Control project"))
    , m_filename(QString())
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
