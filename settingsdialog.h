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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
    class SettingsDialog;
}

class QListWidgetItem;
class QAbstractButton;
class HWSettings;
class ServerSettings;
class TrainSettings;
class ActuatorSettings;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    enum pageType { General = 0, Server, Actuator, Sensor, Train };

    explicit SettingsDialog(HWSettings * hwsettings, QWidget *parent = 0);
    ~SettingsDialog();

public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
    void clicked ( QAbstractButton * button );


    void showPage(SettingsDialog::pageType type);

private:
    void createIcons();
    Ui::SettingsDialog *ui;

    HWSettings * m_hwsettings;
    ServerSettings   *m_serverPage;
    TrainSettings    *m_trainPage;
    ActuatorSettings *m_actuatorPage;
};

#endif // SETTINGSDIALOG_H
