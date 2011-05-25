/*
 * Copyright 2011 Jörg Ehrichs <joerg.ehichs@gmx.de>
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

#ifndef HWSETTINGS_H
#define HWSETTINGS_H

#include <QObject>
#include <QList>

class DCServer;
class DCTrain;
class DCActuator;
class DCSensor;

class HWSettings : public QObject
{
    Q_OBJECT
public:
    HWSettings();

    void setServer(DCServer *newServer);
    DCServer* server() const;

    void addTrain(DCTrain* newTrain);
    QList<DCTrain*> trains() const;
    void removeTrain(DCTrain* train);
    void removeTrain(const QString & trainName);

    void addActuator(DCActuator* newActuator);
    QList<DCActuator*> actuators() const;
    void removeActuator(DCActuator* train);
    void removeActuator(const QString & actuatorName);

    void addSensor(DCSensor* newSensor);
    QList<DCSensor*> sensors() const;
    void removeSensor(DCSensor* sensor);
    void removeSensor(const QString & sensorName);

    void initializeDevices();

public slots:
    void stopTrains();
    void startTrains();

signals:
    void updateTrains();
    void updateSensors();
    void updateActuators();

    private:
    DCServer *m_server;
    QList<DCTrain*> m_trains;
    QList<DCActuator*> m_actuators;
    QList<DCSensor*> m_sensors;
};

#endif // HWSETTINGS_H
