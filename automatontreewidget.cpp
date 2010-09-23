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

#include "automatontreewidget.h"

#include "project.h"
#include "des/dcautomaton.h"

#include <QTreeWidgetItem>
#include <QDebug>

AutomatonTreeWidget::AutomatonTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
    , m_project(0)
{
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this, SLOT(openAutomaton(QTreeWidgetItem*,int)));
}

void AutomatonTreeWidget::setProject(Project *newProject)
{
    m_project = newProject;
    connect (m_project, SIGNAL(updateAutomata()), this, SLOT(updateView()));

    updateView();
}

void AutomatonTreeWidget::updateView()
{
    clear();

    int i =0;
    foreach(DCAutomaton *automaton, m_project->automata())
    {
        QTreeWidgetItem *automatonItem = new QTreeWidgetItem(Automaton);
        automatonItem->setData(0,Qt::UserRole,i);
        automatonItem->setText(0,automaton->name());
        insertTopLevelItem(0,automatonItem);
        i++;

        //TODO add states / transitions / evets as children
    }
}

void AutomatonTreeWidget::openAutomaton( QTreeWidgetItem * item, int column )
{
    if(item->type() == Automaton)
    {
        DCAutomaton *automaton = m_project->automata().value(item->data(0,Qt::UserRole).toInt());

        if(automaton)
        {
            emit openAutomaton(automaton);
        }
    }
}
