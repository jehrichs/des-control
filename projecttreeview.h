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

#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

#include <QTreeWidget>

class Project;
class QTreeWidgetItem;

class ProjectTreeView : public QTreeWidget
{
    Q_OBJECT

    enum ItemType
    {
        HardwareHeader = 1,
        AutomataHeader,
        TrainHeader,
        ActuartorHeader,
        SensorHeader,
        Train,
        Actuator,
        Sensor
    };

public:
    explicit ProjectTreeView(QWidget *parent = 0);

    void setProject(Project *newProject);

public slots:
    void updateTreeView();
    void updateTrains();
    void updateSensors();
    void updateActuators();
    void updateAutomata();
    void updateAutomata(int i);

protected slots:
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void openHardwareList();

private:
    void createActions();

    Project *m_project;

    QTreeWidgetItem *hardwareTrainHeader;
    QTreeWidgetItem *hardwareActuatorHeader;
    QTreeWidgetItem *hardwareSensorHeader;

    QAction* m_addTrain;
    QAction* m_addSensor;
    QAction* m_addActuator;


};

#endif // PROJECTTREEVIEW_H
