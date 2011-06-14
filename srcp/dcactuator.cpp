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

#include "dcactuator.h"

#include <QDebug>

static const char* protocol2srcp[] = {"M","N","S","P"};

DCActuator::DCActuator()
    : DCModelItem()
    , m_protocol(MAERKLIN)
    , m_port(-1)
{
}

void DCActuator::setProtocol(GAProtocol protocol)
{
    if(protocol != m_protocol)
    {
        m_protocol = protocol;
        emit itemChanged();
    }
}

DCActuator::GAProtocol DCActuator::protocol() const
{
    return m_protocol;
}

void DCActuator::setPort(int port)
{
    if(port != m_port)
    {
        m_port = port;
        emit itemChanged();
    }
}

int DCActuator::port() const
{
    return m_port;
}

void DCActuator::setValue(const QString & value)
{
    m_value = value;
}

QString DCActuator::value() const
{
    return m_value;
}

void DCActuator::switchLeft()
{
    setPort(1);
    setValue(QString("1"));
    sendValue();
}

void DCActuator::switchRight()
{
    setPort(0);
    setValue(QString("1"));
    sendValue();
}

bool DCActuator::initialize()
{
    if(address() != -1)
    {
        QString srcpString = QString("INIT 1 GA %1 %2").arg(address()).arg(protocol2srcp[m_protocol]);

        emit sendSRCPString(srcpString);

        return true;
    }

    return false;
}

void DCActuator::sendValue()
{
    // SET <bus> GA <addr> <port> <value> <delay>
    // last -1 means no automatic deactivation
    // value > 0 deactivation after x milliseconds
    //QString srcpString = QString("SET 1 GA %1 %2 1 -1").arg(address()).arg(m_port).arg(m_value);
    QString srcpString = QString("SET 1 GA %1 %2 1 50").arg(address()).arg(m_port);
    QString srcpString2 = QString("SET 1 GA %1 %2 1 50").arg(address()).arg(m_port);

    emit sendSRCPString(srcpString);
    emit sendSRCPString(srcpString2);
}

void DCActuator::updateValues(const QString & srcpString)
{
    //100 INFO <bus> GA <addr> <port> <value>
    // only <value> will be send to this slot, rest is stripped out by the server class

    if( srcpString != m_value)
    {
        m_value = srcpString;
        emit stateChanged();
    }
}
