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

#include "importautomaton.h"

#include "../des/dcautomaton.h"
#include "../des/dcstate.h"
#include "../des/dctransition.h"
#include "../des/dcevent.h"

#include <QDebug>

ImportAutomaton::ImportAutomaton(QObject *parent) :
        QObject(parent)
{

}

ImportAutomaton::~ImportAutomaton()
{
}

QList<DCAutomaton*> ImportAutomaton::loadAutomaton(AutomatonFile fileType, QIODevice *device)
{
    switch(fileType)
    {
    case DESUMA:
        return loadDesumaFile(device);
        break;
    case SUPREMICA:
        return loadSupremicaFile(device);
        break;
    }

    return QList<DCAutomaton*>();
}

QList<DCAutomaton*> ImportAutomaton::loadDesumaFile(QIODevice *device)
{
    Q_UNUSED(device);
    qDebug() << "DESUMA implementation still missing :P";
    return QList<DCAutomaton*>();
}

QList<DCAutomaton*> ImportAutomaton::loadSupremicaFile(QIODevice *device)
{
    reader.setDevice(device);
    m_automatonList.clear();

    while (reader.readNextStartElement())
    {
        if (reader.name() == "Automata")
        {
            while (reader.readNextStartElement())
            {
                if (reader.name() == "Automaton")
                {
                    DCAutomaton* automaton = new DCAutomaton();
                    automaton->setName(reader.attributes().value("name").toString());

                    QString type = reader.attributes().value("type").toString();
                    if(type == "Supervisor")
                        automaton->setAutomatonType(DCAutomaton::Supervisor);
                    else if(type == "Plant")
                        automaton->setAutomatonType(DCAutomaton::Plant);
                    else if(type == "Specification")
                        automaton->setAutomatonType(DCAutomaton::Specification);
                    else if(type == "Property")
                        automaton->setAutomatonType(DCAutomaton::Property);
                    else
                        qDebug() << "Error unknown automaton type ::" << type;

                    while (reader.readNextStartElement())
                    {
                        if (reader.name() == "Events")
                        {
                            addSupEvent(automaton);
                        }
                        else if (reader.name() == "States")
                        {
                            addSupState(automaton);
                        }
                        else if (reader.name() == "Transitions")
                        {
                            addSupTransition(automaton);
                        }
                        else
                            reader.skipCurrentElement();
                    }

                    m_automatonList.append(automaton);
                }
                else
                    reader.skipCurrentElement();
            }
        }
        else
            reader.skipCurrentElement();
    }

    return m_automatonList;
}

void ImportAutomaton::addSupEvent(DCAutomaton* automaton)
{
    reader.readNext();
    while(reader.name() != "Events")
    {
        if(reader.readNext() == QXmlStreamReader::StartElement)
        {
            DCEvent* newevent = new DCEvent();

            newevent->setName(reader.attributes().value("label").toString());
            newevent->setId(reader.attributes().value("id").toString().toInt());

            if(reader.attributes().value("controlable").toString() == "true")
                newevent->setControlable(true);

            automaton->addEvent(newevent);
        }
    }

}

void ImportAutomaton::addSupState(DCAutomaton* automaton)
{
    reader.readNext();
    while(reader.name() != "States")
    {
        if(reader.readNext() == QXmlStreamReader::StartElement)
        {
            DCState* newState = new DCState();

            newState->setName(reader.attributes().value("name").toString());
            newState->setId(reader.attributes().value("id").toString().toInt());

            if(reader.attributes().value("initial").toString() == "true")
                newState->setInitial(true);

            if(reader.attributes().value("accepting").toString() == "true")
                newState->setMarked(true);

            automaton->addState(newState);
        }
    }
}

void ImportAutomaton::addSupTransition(DCAutomaton* automaton)
{
    reader.readNext();
    while(reader.name() != "Transitions")
    {
        if(reader.readNext() == QXmlStreamReader::StartElement)
        {
            DCTransition* newTransition = new DCTransition();

            newTransition->setStates(automaton->getStateFromId(reader.attributes().value("source").toString().toInt()),
                                     automaton->getStateFromId(reader.attributes().value("dest").toString().toInt()));

            newTransition->setEvent(automaton->getEventFromId(reader.attributes().value("event").toString().toInt()));

            automaton->addTransition(newTransition);
        }
    }
}

