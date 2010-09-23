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

#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QList>

class DCServer;
class DCTrain;
class DCActuator;
class DCSensor;
class DCAutomaton;
class QIODevice;

class Project : public QObject
{
    Q_OBJECT
public:
    Project();

    void setName(const QString& name);
    QString name() const;

    void setDescription(const QString& description);
    QString description() const;

    void setFileName(const QString& name);
    QString fileName() const;

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

    void addAutomaton(DCAutomaton* newAutomaton);
    void removeAutomaton(DCAutomaton* automaton);
    QList<DCAutomaton*> automata() const;

    void initializeDevices();

signals:
    void projectChanged();
    void updateTrains();
    void updateSensors();
    void updateActuators();
    void updateAutomata();

private:
    QString m_name;
    QString m_description;

    QString m_filename;

    DCServer *m_server;
    QList<DCTrain*> m_trains;
    QList<DCActuator*> m_actuators;
    QList<DCSensor*> m_sensors;
    QList<DCAutomaton*> m_automata;
};

#endif // PROJECT_H
