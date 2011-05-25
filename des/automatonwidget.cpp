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

#include <QMessageBox>
#include <QString>
#include <QDebug>

AutomatonWidget::AutomatonWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AutomatonWidget)
{
    ui->setupUi(this);

    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)),
            this, SLOT(closeTab(int)));

    connect(ui->tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(openAutomatonChanged(int)));

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
            if(automaton)
                return automaton;
        }

    }
    return 0;
}

void AutomatonWidget::openAutomaton(DCAutomaton* automaton)
{
    int index = ui->tabWidget->indexOf(ui->tabEmpty);
    if(index != -1)
    {
        ui->tabWidget->removeTab(index);
        emit firstAutomatonOpend();
    }

    AutomatonView *newView = new AutomatonView();
    newView->setScene(automaton);
    QString tabname = automaton->name();
    tabname.truncate(15);
    ui->tabWidget->addTab(newView, tabname);
    ui->tabWidget->setCurrentWidget(newView);
}

void AutomatonWidget::closeTab(int index)
{
    if(currentAutomaton())
    {
        if(currentAutomaton()->sceneMode() == DCAutomaton::Run)
        {
            QMessageBox::warning(this, tr("DES Controller"),
                                 tr("Stop the controller first before closing the automaton") );

            return;
        }
    }
    ui->tabWidget->removeTab(index);

    if(ui->tabWidget->count() == 0)
    {
        ui->tabWidget->addTab(ui->tabEmpty, tr("empty automaton"));
        emit lastAutomatonClosed();
    }
}

void AutomatonWidget::closeTab(DCAutomaton* automaton)
{
    int index = ui->tabWidget->indexOf(automaton->views().first());

    closeTab(index);
}

void AutomatonWidget::openAutomatonChanged(int index)
{
    AutomatonView *currentView = dynamic_cast<AutomatonView *>(ui->tabWidget->widget(index));

    if(currentView)
    {
        DCAutomaton *automaton = dynamic_cast<DCAutomaton *>(currentView->scene());

        if(automaton)
            emit switchOpendAutomaton(automaton->sceneMode());

    }

}
