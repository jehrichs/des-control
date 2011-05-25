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

#include "dcevent.h"
#include "../srcp/dcsensor.h"
#include "../srcp/dcactuator.h"

#include <QDebug>

DCEvent::DCEvent()
    : m_id(-1)
    , m_controlable(false)
    , m_active(false)
    , m_actuator(0)
{

}

void DCEvent::setId(int id)
{
    m_id = id;
}

int DCEvent::id() const
{
    return m_id;
}

void DCEvent::setName(const QString & name)
{
    m_name = name;
}

QString DCEvent::name() const
{
    return m_name;
}

void DCEvent::setControlable(bool controlable)
{
    m_controlable = controlable;
}

bool DCEvent::controlable() const
{
    return m_controlable;
}

bool DCEvent::isActive()
{
    return m_active;
}

void DCEvent::setActive(bool active)
{
    m_active = active;
    emit statusChanged();
}

void DCEvent::setSensor(DCSensor *sensor, FBState state)
{
    m_sensor = sensor;

    if(state == HIGH) {
        connect(m_sensor, SIGNAL(sensorHigh()), this, SLOT(updateStatus()));
    }
    else {
        connect(m_sensor, SIGNAL(sensorLow()), this, SLOT(updateStatus()));
    }
}

DCSensor * DCEvent::getSensor()
{
    return m_sensor;
}

void DCEvent::setActuator(DCActuator *actuator, GAAction actuatorMode)
{
    m_actuator = actuator;
    m_actuatorMode = actuatorMode;
}

void DCEvent::activateActuator()
{
    if(!m_actuator) {
        return;
    }

    if(m_actuatorMode == SWITCH_LEFT) {
        m_actuator->switchLeft();
    }
    else {
        m_actuator->switchRight();
    }
}

void DCEvent::toggleStatus()
{
    m_active = m_active ? false : true;

    emit statusChanged();
}

void DCEvent::updateStatus()
{
    if(m_sensor->value() == 1) {
        m_active =  true;
    }
    else {
        m_active =  false;
    }

    emit statusChanged();
}
