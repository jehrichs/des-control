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

#ifndef HWCONNECTIONS_H
#define HWCONNECTIONS_H

#include <QDialog>

namespace Ui {
    class HWConnections;
}

class HWSettings;
class DCAutomaton;
class QComboBox;

class HWConnections : public QDialog
{
    Q_OBJECT

public:
    explicit HWConnections(HWSettings *hw, DCAutomaton *automaton);
    ~HWConnections();

public slots:
    void applyChanges();

private:
    void setupDialog();
    Ui::HWConnections *ui;

    HWSettings  *m_hw;
    DCAutomaton *m_automaton;

    QList<QComboBox *> m_cbList;
};

#endif // HWCONNECTIONS_H
