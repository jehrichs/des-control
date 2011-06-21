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

#ifndef DCACTUATOR_H
#define DCACTUATOR_H

#include "dcmodelitem.h"

class DCActuator : public DCModelItem
{
    Q_OBJECT

public:
    enum GAProtocol
    {
        MAERKLIN,
        NRMA,
        SELECTRIX,
        SERVER
    };

    enum GAType {
        GA_SWITCH,
        GA_SIGNAL
    };

    explicit DCActuator();

    void setProtocol(GAProtocol protocol);
    DCActuator::GAProtocol protocol() const;

    void setPort(int port);
    int port() const;

    void setType(GAType type);
    GAType getType();

    void setValue(const QString & value);
    QString value() const;

    void switchLeft();
    void switchRight();

    bool initialize();
    void sendValue();

public slots:
    void updateValues(const QString & srcpString);

private:
    GAProtocol m_protocol;
    GAType m_type;
    int m_port;

    QString m_value;

};

#endif // DCACTUATOR_H
