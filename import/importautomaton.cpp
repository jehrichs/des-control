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
    qDebug() << "DESUMA implementation still missing :P";
    return QList<DCAutomaton*>();
}

QList<DCAutomaton*> ImportAutomaton::loadSupremicaFile(QIODevice *device)
{
    reader.setDevice(device);
    QList<DCAutomaton*> automatonList;

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
                }
                if (reader.name() == "States")
                {
                    while (reader.readNextStartElement())
                    {
                        if (reader.name() == "Event")
                        {
                            DCState* newState = new DCState();

                            newState->setName(reader.attributes().value("name").toString());
                            newState->setID(reader.attributes().value("id").toString().toInt());

                            if(reader.attributes().value("initial").toString() == "true")
                                newState->setInitial(true);
                            if(reader.attributes().value("accepting").toString() == "true")
                                newState->setMarked(true);
                        }
                        else
                            reader.skipCurrentElement();
                    }

                }
                if (reader.name() == "Transitions")
                {
                    while (reader.readNextStartElement())
                    {
                        if (reader.name() == "Transition")
                        {
                            DCEvent* newEvent = new DCEvent();

                            newEvent->setPlaceFrom(automaton->getStateFromId(reader.attributes().value("source").toString().toInt()));
                            newEvent->setPlaceTo(automaton->getStateFromId(reader.attributes().value("dest").toString().toInt()));

                            //                            if(reader.attributes().value("initial").toString() == "true")
                            //                                newState->setInitial(true);
                            //                            if(reader.attributes().value("accepting").toString() == "true")
                            //                                newState->setMarked(true);
                        }
                        else
                            reader.skipCurrentElement();
                    }
                }
            }

            automatonList.append(automaton);
        }
        else
            reader.skipCurrentElement();

    }

    return automatonList;
}


/*
<?xml version="1.0" encoding="ISO-8859-1"?>
<Automata name="Untitled" major="0" minor="9">
<Automaton name="S4" type="Supervisor">
        <Events>
                <Event id="0" label="a" controllable="false"/>
                <Event id="1" label="b" controllable="false"/>
                <Event id="2" label="c" controllable="false"/>
                <Event id="3" label="d"/>
                <Event id="4" label="e" controllable="false"/>
                <Event id="5" label="f"/>
                <Event id="6" label="g" controllable="false"/>
                <Event id="7" label="h" controllable="false"/>
        </Events>
</Automaton>
</Automata>
*/
