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

#ifndef HARDWARESETTINGS_H
#define HARDWARESETTINGS_H

#include <QDialog>

namespace Ui {
    class HardwareSettings;
}

class Project;
class TrainSettings;
class ActuatorSettings;
class SensorSettings;
class QListWidgetItem;

class HardwareSettings : public QDialog
{
    Q_OBJECT

public:
    explicit HardwareSettings(Project *project, QWidget *parent = 0);
    ~HardwareSettings();

public slots:
    void showPage(int type);
    void selectItem();
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private:
    void createIcons();
    Ui::HardwareSettings *ui;

    Project * m_project;
    TrainSettings    *m_trainPage;
    ActuatorSettings *m_actuatorPage;
    SensorSettings   *m_sensorPage;

    QListWidgetItem *m_trainButton;
    QListWidgetItem *m_actuatorButton;
    QListWidgetItem *m_sensorButton;
};

#endif // HARDWARESETTINGS_H
