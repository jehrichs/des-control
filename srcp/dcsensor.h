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

#ifndef DCSENSOR_H
#define DCSENSOR_H

#include "dcmodelitem.h"

class DCSensor : public DCModelItem
{
    Q_OBJECT
public:
    explicit DCSensor();

    int value() const;

    bool initialize();

public slots:
    void updateValues(const QString & srcpString);

private:
    int m_value;
};

#endif // DCSENSOR_H
