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

#ifndef AUTOMATONTREEWIDGET_H
#define AUTOMATONTREEWIDGET_H

#include <QTreeWidget>

class Project;
class DCAutomaton;

class AutomatonTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    enum ItemType
    {
        Automaton = 1,
        State,
        Transition,
        Event
    };

    explicit AutomatonTreeWidget(QWidget *parent = 0);

    void setProject(Project *newProject);

signals:
    void openAutomaton(DCAutomaton *automaton);

public slots:
    void updateView();

    void openAutomaton( QTreeWidgetItem * item, int column );
private:
    Project *m_project;
};

#endif // AUTOMATONTREEWIDGET_H
