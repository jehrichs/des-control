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

#ifndef SENSORSETTINGS_H
#define SENSORSETTINGS_H

#include <QWidget>

namespace Ui {
    class SensorSettings;
}

class Project;
class QListWidgetItem;
class DCSensor;

class SensorSettings : public QWidget
{
    Q_OBJECT

public:
    explicit SensorSettings(Project * project);
    ~SensorSettings();

private slots:
    void showNewItemInfo ( QListWidgetItem * current, QListWidgetItem * previous );
    void newSensor();
    void deleteCurrent();
    void saveChanges();

private:
    void showItem(DCSensor *sensor);
    Ui::SensorSettings *ui;
    Project* m_project;
};

#endif // SENSORSETTINGS_H
