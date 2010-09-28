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

#ifndef HARDWARETREEWIDGET_H
#define HARDWARETREEWIDGET_H

#include <QTreeWidget>

class Project;
class QTreeWidgetItem;

class HardwareTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    enum ItemType
    {
        ActuartorHeader = 1,
        SensorHeader,
        TrainHeader,
        Train,
        Actuator,
        Sensor
    };

    explicit HardwareTreeWidget(QWidget *parent = 0);

    void setProject(Project *newProject);

public slots:
    void updateView();
    void updateTrains();
    void updateSensors();
    void updateActuators();

    void openConfig( QTreeWidgetItem * item, int column );

private:
    Project *m_project;

    QTreeWidgetItem *hardwareTrainHeader;
    QTreeWidgetItem *hardwareActuatorHeader;
    QTreeWidgetItem *hardwareSensorHeader;
};

#endif // HARDWARETREEWIDGET_H
