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

#include "dctrain.h"
#include <QStringList>

#include <QDebug>

DCTrain::DCTrain()
    : DCModelItem()
{
}

void DCTrain::setProtocol(GLProtocol protocol)
{
    m_protocol = protocol;
}

DCTrain::GLProtocol DCTrain::protocol() const
{
    return m_protocol;
}

void DCTrain::setDecoderSteps(int steps)
{
    if(steps != m_decoderSteps)
    {
        m_decoderSteps = steps;
        emit itemChanged();
    }
}

int DCTrain::decoderSteps() const
{
    return m_decoderSteps;
}

void DCTrain::setDriveMode(Drivemode driveMode)
{
    m_driveMode = driveMode;
}

DCTrain::Drivemode DCTrain::driveMode() const
{
    return driveMode();
}

void DCTrain::setDecoderFunctions(int numberOfFunctions)
{
    if(numberOfFunctions != m_decoderFunctions)
    {
        m_decoderFunctions = numberOfFunctions;

        // initialize the list
        for (int i = 0; i < numberOfFunctions; i++)
        {
            m_functions.append(false);
        }
        emit itemChanged();
    }
}

int DCTrain::decoderFunctions() const
{
    return m_decoderFunctions;
}

void DCTrain::setSpeed(int speed)
{
    m_speed = speed;
}

int DCTrain::speed() const
{
    return m_speed;
}

void DCTrain::setmaxSpeed(int maxspeed)
{
    m_maxSpeed = maxspeed;
}

int DCTrain::maxspeed() const
{
    return m_maxSpeed;
}

void DCTrain::setFunction(int function, bool value)
{
    if(function > m_decoderFunctions)
        return;

    m_functions.replace(function, value);
}

bool DCTrain::function(int number) const
{
    return m_functions.at(number);
}

bool DCTrain::initialize()
{
    if(address() != -1 && busID() != -1)
    {
        QString srcpString;

        switch(m_protocol)
        {
        case ANALOG:
            srcpString = QString("INIT %1 GL %2 A").arg(busID()).arg(address());
            break;

        case MM1:
            srcpString = QString("INIT %1 GL %2 M 1 %3 %4").arg(busID()).arg(address()).arg(m_decoderSteps).arg(m_decoderFunctions);
            break;

        case MM2:
            srcpString = QString("INIT %1 GL %2 M 2 %3 %4").arg(busID()).arg(address()).arg(m_decoderSteps).arg(m_decoderFunctions);
            break;

        case DCC1:
            srcpString = QString("INIT %1 GL %2 N 1 %3 %4").arg(busID()).arg(address()).arg(m_decoderSteps).arg(m_decoderFunctions);
            break;

        case DCC2:
            srcpString = QString("INIT %1 GL %2 N 1 %3 %4").arg(busID()).arg(address()).arg(m_decoderSteps).arg(m_decoderFunctions);
            break;

        case SERVER:
            srcpString = QString("INIT %1 GL %2 P").arg(busID()).arg(address());
            break;

        }

        emit sendSRCPString(srcpString);

        return true;
    }

    return false;
}

void DCTrain::sendValue()
{
    // SET <bus> GL <addr> <drivemode> <V> <V_max> <f1> . . <fn>
    QString srcpString = QString("SET %1 GL %2 %3 %4 %5").arg(busID()).arg(address()).arg((int) m_driveMode).arg(m_speed).arg(m_maxSpeed);

    foreach(bool fn, m_functions)
    {
        srcpString.append(QString(" %1").arg(fn));
    }

    emit sendSRCPString(srcpString);
}

void DCTrain::updateValues(const QString & srcpString)
{
    // 100 INFO <bus> GL <addr> <drivemode> <V> <V_max> <f1> . . <fn>
    // only <drivemode> and everything afterwards will be send to this slot, rest is stripped out by the server class

    QStringList valueList = srcpString.split(' ');
    bool changedValues;

    m_driveMode  = (Drivemode) valueList.at(0).toInt();
    m_speed =valueList.at(1).toInt();
    m_maxSpeed =valueList.at(2).toInt();

    for (int i = 3; i < valueList.size(); i++)
    {
        m_functions.replace(i-3, (bool)valueList.at(i).toInt());
    }

    emit stateChanged();
}
