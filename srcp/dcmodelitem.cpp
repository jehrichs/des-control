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

#include "dcmodelitem.h"

DCModelItem::DCModelItem()
    : QObject()
    , m_busID(-1)
    , m_address(-1)
    , m_name(QString())
{
}


void DCModelItem::setBusID(int busID)
{
    if(m_busID != busID)
    {
        m_busID = busID;
        emit itemChanged();
    }
}

int DCModelItem::busID() const
{
    return m_busID;
}

void DCModelItem::setAddress(int address)
{
    if(address != m_address)
    {
        m_address = address;
        emit itemChanged();
    }
}

int DCModelItem::address() const
{
    return m_address;
}

void DCModelItem::setName(const QString & name)
{
    if(name != m_name)
    {
        m_name = name;
        emit itemChanged();
    }
}

QString DCModelItem::name()
{
    if(m_name.isEmpty())
    {
        return QString("Unnamed :: Bus: %1 Addr: %2").arg(m_busID).arg(m_address);
    }

    return m_name;
}

void DCModelItem::updateValues(const QString & srcpString)
{
    Q_UNUSED(srcpString);
}
