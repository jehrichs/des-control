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

#include "actuatorsettings.h"
#include "ui_actuatorsettings.h"

#include "project.h"
#include "dcactuator.h"

#include <QDebug>

ActuatorSettings::ActuatorSettings(Project * project)
    : QWidget(0)
    , ui(new Ui::ActuatorSettings)
    , m_project(project)
{
    ui->setupUi(this);

    foreach(DCActuator *actuator, m_project->actuators())
    {
        ui->listWidgetActuators->addItem(actuator->name());
    }

    connect(ui->listWidgetActuators, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(showNewItemInfo(QListWidgetItem*,QListWidgetItem*)));

}

ActuatorSettings::~ActuatorSettings()
{
    delete ui;
}

void ActuatorSettings::showNewItemInfo ( QListWidgetItem * current, QListWidgetItem * previous )
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
    //TODO check if there are unsaved chanegs in the old item

    //we assume no sorting in the listview thus => row of item = position in the project QList

    int currentRow = ui->listWidgetActuators->currentRow();
    if(m_project->actuators().size() > currentRow && currentRow >= 0)
    {
        showItem(m_project->actuators().at(currentRow));
    }
    else
    {
        ui->lineEditName->setText(QString());
        ui->spinBoxAddress->setValue(-1);
        ui->spinBoxPort->setValue(-1);

        ui->comboBoxProtocol->setCurrentIndex(0);
    }
}

void ActuatorSettings::showItem(DCActuator *actuator)
{
    ui->lineEditName->setText(actuator->name());
    ui->spinBoxAddress->setValue(actuator->address());
    ui->spinBoxPort->setValue(actuator->port());

    ui->comboBoxProtocol->setCurrentIndex(actuator->protocol());
}

void ActuatorSettings::newActuator()
{
    DCActuator *act = new DCActuator();
    int itemNumber = m_project->actuators().size()+1;
    act->setName(tr("NewActuator%1").arg(itemNumber));

    m_project->addActuator(act);
    ui->listWidgetActuators->addItem(act->name());
    ui->listWidgetActuators->setCurrentRow(itemNumber-1);
    showItem(act);
}

void ActuatorSettings::deleteCurrent()
{
    int currentRow = ui->listWidgetActuators->currentRow();

    if(currentRow != -1)
    {
        m_project->removeActuator(m_project->actuators().at(currentRow));
        ui->listWidgetActuators->takeItem(currentRow);
    }
}

void ActuatorSettings::saveChanges()
{
    int currentRow = ui->listWidgetActuators->currentRow();

    if(currentRow < 0)
        return;

    DCActuator *act = m_project->actuators().at(currentRow);

    act->setName(ui->lineEditName->text());
    act->setAddress(ui->spinBoxAddress->value());
    act->setProtocol((DCActuator::GAProtocol)ui->comboBoxProtocol->currentIndex());
    act->setPort(ui->spinBoxPort->value());

    QListWidgetItem *cur = ui->listWidgetActuators->currentItem();
    cur->setText(ui->lineEditName->text());

}
