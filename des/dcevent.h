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

#ifndef DCEVENT_H
#define DCEVENT_H

#include <QObject>

class DCSensor;
class DCActuator;

class DCEvent : public QObject
{
    Q_OBJECT
public:

    enum GAAction {
        SWITCH_LEFT,
        SWITCH_RIGHT
    };

    enum FBState {
        HIGH,
        LOW
    };

    DCEvent();

    void setId(int id);
    int id() const;

    void setName(const QString & name);
    QString name() const;

    void setControlable(bool controlable);

    bool controlable() const;

    bool isActive();
    void setActive(bool active);

    void setSensor(DCSensor *sensor, FBState state);
    FBState getFBState();
    DCSensor * getSensor();

    void setActuator(DCActuator *actuator, GAAction actuatorMode);
    DCActuator * getActuator();
    GAAction getActuatorMode();
    void activateActuator();

public slots:
    void toggleStatus();
    void updateStatus();

signals:
    void statusChanged();

private:
    int m_id;
    QString m_name;

    bool m_controlable;

    bool m_active;
    FBState m_state;
    DCSensor *m_sensor;
    DCActuator *m_actuator;
    GAAction m_actuatorMode;


};

#endif // DCEVENT_H
