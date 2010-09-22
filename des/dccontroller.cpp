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

#include "dccontroller.h"

#include "dcautomaton.h"
#include "dcstate.h"
#include "dcevent.h"
#include "dctransition.h"

#include <QTimer>
#include <QDebug>

DCController::DCController(QObject *parent)
    : QObject(parent)
    , m_mode(Simulation)
    , m_cycleTimer(new QTimer(this))
    , m_currentState(0)
{
    connect(m_cycleTimer, SIGNAL(timeout()), this, SLOT(updateDES()));
}

void DCController::setMode(ControlMode mode)
{
    m_mode = mode;
}

DCController::ControlMode DCController::mode() const
{
    return m_mode;
}

void DCController::setAutomaton(DCAutomaton* automaton)
{
    m_automaton = automaton;

    m_initialState = m_automaton->getInitialState();
    //m_currentState = m_initialState;
}

void DCController::startController()
{
    m_cycleTimer->start(100);
}

void DCController::stopController()
{
    m_cycleTimer->stop();
}

void DCController::pauseController(bool paused)
{
    if(paused)
        m_cycleTimer->stop();
    else
        m_cycleTimer->start(100);
}

void DCController::updateDES()
{
    //if no current state exist, we started, soset current = initial
    if(!m_currentState)
    {
        m_currentState = m_initialState;
        m_currentState->setActive(true);
    }

    //ok we have a current state
    // what events are we listening to?
    QList<DCTransition*> eventList = m_currentState->outgoingTransitions();

    //is one of the events active?
    foreach(DCTransition* edge, eventList)
    {
        // if it is, follow current state to next state
        if(edge->event()->isActive())
        {
            m_currentState->setActive(false);
            m_currentState = edge->destinationState();
            qDebug() << edge->destinationState()->name();
            m_currentState->setActive(true);
        }

        //qDebug() << "checked edge" << edge->event()->name();
    }

    // if we are not in a simulation, send hardware events for this state to the srcp server
    //qDebug() << "set state events for ::" << m_currentState->name() << m_currentState->isActive();
}
