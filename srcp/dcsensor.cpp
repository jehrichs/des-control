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

#include "dcsensor.h"

DCSensor::DCSensor()
    : DCModelItem()
{
}

QString DCSensor::value() const
{
    return m_value;
}

bool DCSensor::initialize()
{
    if(address() != -1 && busID() != -1)
    {
        QString srcpString = QString("INIT %1 FB %2").arg(busID()).arg(address());

        emit sendSRCPString(srcpString);

        return true;
    }

    return false;
}

void DCSensor::updateValues(const QString & srcpString)
{
    //100 INFO <bus> FB <addr> <value>
    // only <value> will be send to this slot, rest is stripped out by the server class

    if( srcpString != m_value)
    {
        m_value = srcpString;
        emit stateChanged();
    }
}
