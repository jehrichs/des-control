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

#include "automatonwidget.h"
#include "ui_automatonwidget.h"

#include "des/automatonview.h"
#include "des/dcautomaton.h"

#include <QString>
#include <QDebug>

AutomatonWidget::AutomatonWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::AutomatonWidget)
{
    ui->setupUi(this);

    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)),
            this, SLOT(closeTab(int)));
}

AutomatonWidget::~AutomatonWidget()
{
    delete ui;
}

DCAutomaton * AutomatonWidget::currentAutomaton() const
{
    if(ui->tabEmpty->isHidden())
    {
        AutomatonView *currentView = dynamic_cast<AutomatonView *>(ui->tabWidget->currentWidget());

        if(currentView)
        {
            DCAutomaton *automaton = dynamic_cast<DCAutomaton *>(currentView->scene());
            return automaton;
        }

    }
    return 0;
}

void AutomatonWidget::openAutomaton(DCAutomaton* automaton)
{
    int index = ui->tabWidget->indexOf(ui->tabEmpty);
    if(index != -1)
        ui->tabWidget->removeTab(index);

    AutomatonView *newView = new AutomatonView();
    newView->setScene(automaton);
    QString tabname = automaton->name();
    tabname.truncate(15);
    ui->tabWidget->addTab(newView, tabname);
    ui->tabWidget->setCurrentWidget(newView);
}

void AutomatonWidget::removeAutomaton(DCAutomaton* automaton)
{

}

void AutomatonWidget::closeTab(int index)
{
    ui->tabWidget->removeTab(index);

    if(ui->tabWidget->count() == 0)
    {
        ui->tabWidget->addTab(ui->tabEmpty, tr("empty automaton"));
    }
}
