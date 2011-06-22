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

#include <QFile>
#include <QMessageBox>
#include <QDebug>

ImportAutomaton::ImportAutomaton(AutomatonFile fileType, const QString & filename) :
    QThread(0)
{
    m_useShortnames = true;
    m_fileType = fileType;
    m_filename = filename;
}

ImportAutomaton::~ImportAutomaton()
{
}

void ImportAutomaton::run()
{
    loadAutomaton();
}

void ImportAutomaton::loadAutomaton()
{
    QFile file(m_filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(0, tr("DES Automaton Importer"),
                             tr("Cannot open file %1:\n%2.")
                             .arg(m_filename)
                             .arg(file.errorString()));
        return;
    }

    switch(m_fileType)
    {
    case DESUMA:
        loadDesumaFile(&file);
        break;
    case SUPREMICA:
        loadSupremicaFile(&file);
        break;
    }

    emit importfinished(m_automaton);
}

DCAutomaton* ImportAutomaton::getAutomaton()
{
    return m_automaton;
}

void ImportAutomaton::setAutomaton(DCAutomaton* newAutomaton)
{
    m_automaton = newAutomaton;
}

void ImportAutomaton::loadDesumaFile(QIODevice *device)
{
    Q_UNUSED(device);
    qDebug() << "DESUMA implementation still missing :P";
}

void ImportAutomaton::loadSupremicaFile(QIODevice *device)
{
    m_reader.setDevice(device);

    while (m_reader.readNextStartElement())
    {
        if (m_reader.name() == "Automata")
        {
            while (m_reader.readNextStartElement())
            {
                if (m_reader.name() == "Automaton")
                {
                    //DCAutomaton* automaton = new DCAutomaton(DCAutomaton::Visual);
                    m_automaton->setName(m_reader.attributes().value("name").toString());

                    QString type = m_reader.attributes().value("type").toString();
                    if(type == "Supervisor")
                        m_automaton->setAutomatonType(DCAutomaton::Supervisor);
                    else if(type == "Plant")
                        m_automaton->setAutomatonType(DCAutomaton::Plant);
                    else if(type == "Specification")
                        m_automaton->setAutomatonType(DCAutomaton::Specification);
                    else if(type == "Property")
                        m_automaton->setAutomatonType(DCAutomaton::Property);
                    else
                        qDebug() << "Error unknown automaton type ::" << type;

                    while (m_reader.readNextStartElement())
                    {
                        if (m_reader.name() == "Events")
                        {
                            emit importvalue(10);
                            addSupEvent(m_automaton);
                        }
                        else if (m_reader.name() == "States")
                        {
                            emit importvalue(33);
                            addSupState(m_automaton);

                        }
                        else if (m_reader.name() == "Transitions")
                        {
                            emit importvalue(66);
                            addSupTransition(m_automaton);
                            emit importvalue(100);
                        }
                        else
                            m_reader.skipCurrentElement();
                    }
                }
                else
                    m_reader.skipCurrentElement();
            }
        }
        else
            m_reader.skipCurrentElement();
    }
}

void ImportAutomaton::addSupEvent(DCAutomaton* automaton)
{
    m_reader.readNext();
    int eventcounter = 0;
    while(m_reader.name() != "Events")
    {
        if(m_reader.readNext() == QXmlStreamReader::StartElement)
        {
            DCEvent* newevent = new DCEvent();

            newevent->setName(m_reader.attributes().value("label").toString());
            newevent->setId(m_reader.attributes().value("id").toString().toInt());

            if(m_reader.attributes().value("controllable").toString() == "false") {
                newevent->setControlable(false);
            }
            else {
                newevent->setControlable(true);
            }

            automaton->addEvent(newevent);
            eventcounter++;

            emit importstatus(QString("Import Event %1").arg(eventcounter));
        }
    }

}

void ImportAutomaton::addSupState(DCAutomaton* automaton)
{
    m_reader.readNext();

    int placeNumber = 1;
    while(m_reader.name() != "States")
    {
        if(m_reader.readNext() == QXmlStreamReader::StartElement)
        {
            DCState* newState = new DCState();

            if(m_useShortnames) {
                newState->setName(QString("S%1").arg(placeNumber));
            }
            else {
                newState->setName(m_reader.attributes().value("name").toString());
            }

            newState->setLongName(m_reader.attributes().value("name").toString());
            newState->setId(m_reader.attributes().value("id").toString().toInt());

            if(m_reader.attributes().value("initial").toString() == "true")
                newState->setInitial(true);

            if(m_reader.attributes().value("accepting").toString() == "true")
                newState->setMarked(true);

            automaton->addState(newState);

            placeNumber++;

            if(placeNumber > 100 && automaton->getVisualMode() == DCAutomaton::Visual) {
                automaton->setVisualMode(DCAutomaton::Nonvisual);
            }

            emit importstatus(QString("Import State %1").arg(placeNumber));
        }
    }
}

void ImportAutomaton::addSupTransition(DCAutomaton* automaton)
{
    m_reader.readNext();
    int transitioncounter = 0;
    while(m_reader.name() != "Transitions")
    {
        if(m_reader.readNext() == QXmlStreamReader::StartElement)
        {
            DCTransition* newTransition = new DCTransition();

            newTransition->setStates(automaton->getStateFromId(m_reader.attributes().value("source").toString().toInt()),
                                     automaton->getStateFromId(m_reader.attributes().value("dest").toString().toInt()));

            newTransition->setEvent(automaton->getEventFromId(m_reader.attributes().value("event").toString().toInt()));

            automaton->addTransition(newTransition);
            transitioncounter++;
            emit importstatus(QString("Import Transition %1").arg(transitioncounter));
        }
    }
}

