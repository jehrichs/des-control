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

#ifndef TRAINSETTINGS_H
#define TRAINSETTINGS_H

#include <QWidget>

namespace Ui {
    class TrainSettings;
}

class Project;
class QListWidgetItem;
class DCTrain;

class TrainSettings : public QWidget
{
    Q_OBJECT

public:
    explicit TrainSettings(Project * project);
    ~TrainSettings();

public slots:
    void saveChanges();

private slots:
    void showNewItemInfo ( QListWidgetItem * current, QListWidgetItem * previous );
    void newTrain();
    void deleteCurrent();

private:
    void showItem(DCTrain *train);

    Ui::TrainSettings *ui;
    Project* m_project;
};

#endif // TRAINSETTINGS_H
