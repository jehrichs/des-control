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

#ifndef DCTRAIN_H
#define DCTRAIN_H

#include "dcmodelitem.h"

#include <QList>

class DCTrain : public DCModelItem
{
    Q_OBJECT

public:
    enum GLProtocol
    {
        ANALOG,
        MM1,
        MM2,
        DCC1,
        DCC2,
        SERVER
    };

    enum Drivemode
    {
        BACKWARD,
        FORWARD,
        EMERGENCYSTOP
    };

    explicit DCTrain();

    void setProtocol(GLProtocol protocol);
    DCTrain::GLProtocol protocol() const;

    void setDecoderSteps(int steps);
    int decoderSteps() const;

    void setDriveMode(Drivemode driveMode);
    DCTrain::Drivemode driveMode() const;

    void setDecoderFunctions(int numberOfFunctions);
    int decoderFunctions() const;

    void setSpeed(int speed);
    int speed() const;

    void setmaxSpeed(int maxspeed);
    int maxspeed() const;

    void setFunction(int function, bool value);
    bool function(int number) const;

    bool initialize();
    void sendValue();

public slots:
    void updateValues(const QString & srcpString);

private:
    GLProtocol m_protocol;
    int m_decoderSteps;
    int m_decoderFunctions;
    int m_speed;
    int m_maxSpeed;
    Drivemode m_driveMode;

    QList<bool> m_functions;
};

#endif // DCTRAIN_H
