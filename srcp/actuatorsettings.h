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

#ifndef ACTUATORSETTINGS_H
#define ACTUATORSETTINGS_H

#include <QWidget>

namespace Ui {
    class ActuatorSettings;
}

class HWSettings;
class QListWidgetItem;
class DCActuator;

class ActuatorSettings : public QWidget
{
    Q_OBJECT

public:
    explicit ActuatorSettings(HWSettings * hwsettings);
    ~ActuatorSettings();

public slots:
    void saveChanges();

private slots:
    void showNewItemInfo ( QListWidgetItem * current, QListWidgetItem * previous );
    void newActuator();
    void deleteCurrent();

private:
    void showItem(DCActuator *actuator);

    Ui::ActuatorSettings *ui;
    HWSettings * m_hwsettings;
};

#endif // ACTUATORSETTINGS_H
