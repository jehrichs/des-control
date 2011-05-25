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
#include "hwsettings.h"
#include "../srcp/dcsensor.h"

#include <QTimer>
#include <QProcess>
#include <QDebug>

DCController::DCController(QObject *parent)
    : QObject(parent)
    , m_mode(Simulation)
    , m_cycleTimer(new QTimer(this))
    , m_automaton(0)
    , m_initialState(0)
    , m_currentState(0)
    , m_running(false)
    , m_paused(false)
{
    connect(m_cycleTimer, SIGNAL(timeout()), this, SLOT(updateDES()));
}

DCController::~DCController()
{
    delete m_cycleTimer;
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

    if(automaton)
    {
        m_initialState = m_automaton->getInitialState();
        m_automaton->setSceneMode(DCAutomaton::Run);
    }
}

DCAutomaton* DCController::automaton() const
{
    return m_automaton;
}

void DCController::setHWSettings(HWSettings *hw)
{
    m_hwSettings = hw;
}

void DCController::startController()
{
    m_currentState = m_initialState;
    m_currentState->setActive(true);

    m_cycleTimer->start(100);
    m_running = true;
    emit curState(m_currentState);
}

void DCController::stopController()
{
    m_cycleTimer->stop();

    if(m_currentState)
    {
        m_currentState->setActive(false);
        m_currentState = 0;
    }
    m_paused = false;
    m_running = false;
    emit curState(m_currentState);
}

void DCController::pauseController(bool paused)
{
    if(paused)
        m_cycleTimer->stop();
    else
        m_cycleTimer->start(100);

    m_paused = paused;
}

bool DCController::isRunnung()
{
    return m_running;
}

bool DCController::isPaused()
{
    return m_paused;
}

void DCController::updateDES()
{
    // start every cycle by reading the inputs
    if(m_mode == Live) {
        QString  cmd = QString("readsensors");
        QString inputstring;

        QProcess setConf;
        setConf.start( cmd );

        if( !setConf.waitForStarted() ) {
            qDebug() << "can't start process readsensors";
            return;
        }

        if( !setConf.waitForFinished() ) {
            qDebug() << "can't wait for process readsensors";
            return;
        }

        QByteArray output = setConf.readAll();

        if( !output.isEmpty() ) {
            //qDebug() << cmd;
            //qDebug() << output;
            inputstring = QString(output);

            QList<DCSensor*> sensorList = m_hwSettings->sensors();
            foreach(DCSensor* sensor, sensorList)
            {
                sensor->updateValues(inputstring);
            }
        }
        else {
            qDebug() << "could not read sensor input";
        }
    }

    //ok we have a current state
    // what events are we listening to?
    QList<DCTransition*> eventList = m_currentState->outgoingTransitions();

    // do we have a controlled event which we can activate?
    foreach(DCTransition* edge, eventList)
    {
        // if it is, follow current state to next state
        if(edge->event()->controlable())
        {
            if(m_mode == Live) {
                edge->event()->activateActuator();
            }

            m_currentState->setActive(false);
            m_currentState = edge->destinationState();
            m_currentState->setActive(true);
            break;
        }
    }

    //is one of the uncontrolled events active?
    foreach(DCTransition* edge, eventList)
    {
        // if it is, follow current state to next state
        // do not check 2 conrollable shortly after each other, we might not be able to switch so fast
        if(!edge->event()->controlable() && edge->event()->isActive())
        {
            m_currentState->setActive(false);
            m_currentState = edge->destinationState();
            qDebug() << edge->destinationState()->name();
            m_currentState->setActive(true);
        }
    }

    emit curState(m_currentState);
}
