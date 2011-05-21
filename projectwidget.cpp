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

#include "projectwidget.h"
#include "ui_projectwidget.h"

ProjectWidget::ProjectWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProjectWidget)
    , m_project(0)
{
    ui->setupUi(this);
}

ProjectWidget::~ProjectWidget()
{
    delete ui;
}

void ProjectWidget::setProject(Project *newProject)
{
    m_project = newProject;

    ui->treeAutomata->setProject(m_project);
}

AutomatonTreeWidget* ProjectWidget::automatonList() const
{
    return ui->treeAutomata;
}
