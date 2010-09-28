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

#include "sensorsettings.h"
#include "ui_sensorsettings.h"

#include "project.h"
#include "dcsensor.h"

SensorSettings::SensorSettings(Project * project)
    : QWidget(0)
    , ui(new Ui::SensorSettings)
    , m_project(project)
{
    ui->setupUi(this);

    foreach(DCSensor *sensor, m_project->sensors())
    {
        ui->listWidgetSensors->addItem(sensor->name());
    }

    connect(ui->listWidgetSensors, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(showNewItemInfo(QListWidgetItem*,QListWidgetItem*)));

}

SensorSettings::~SensorSettings()
{
    delete ui;
}

void SensorSettings::showNewItemInfo ( QListWidgetItem * current, QListWidgetItem * previous )
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
    //TODO check if there are unsaved chanegs in the old item

    //we assume no sorting in the listview thus => row of item = position in the project QList

    int currentRow = ui->listWidgetSensors->currentRow();
    if(m_project->sensors().size() > currentRow && currentRow >= 0)
    {
        showItem(m_project->sensors().at(currentRow));
    }
    else
    {
        ui->lineEditName->setText(QString());
        ui->spinBoxAddress->setValue(-1);
    }
}

void SensorSettings::showItem(DCSensor *sensor)
{
    ui->lineEditName->setText(sensor->name());
    ui->spinBoxAddress->setValue(sensor->address());
}

void SensorSettings::newSensor()
{
    DCSensor *sensor = new DCSensor();
    int itemNumber = m_project->sensors().size()+1;
    sensor->setName(tr("NewSensor%1").arg(itemNumber));

    m_project->addSensor(sensor);
    ui->listWidgetSensors->addItem(sensor->name());
    ui->listWidgetSensors->setCurrentRow(itemNumber-1);
    showItem(sensor);
}

void SensorSettings::deleteCurrent()
{
    int currentRow = ui->listWidgetSensors->currentRow();

    if(currentRow != -1)
    {
        m_project->removeSensor(m_project->sensors().at(currentRow));
        ui->listWidgetSensors->takeItem(currentRow);
    }
}

void SensorSettings::saveChanges()
{
    int currentRow = ui->listWidgetSensors->currentRow();

    if(currentRow < 0)
        return;

    DCSensor *sensor = m_project->sensors().at(currentRow);

    sensor->setName(ui->lineEditName->text());
    sensor->setAddress(ui->spinBoxAddress->value());

    QListWidgetItem *cur = ui->listWidgetSensors->currentItem();
    cur->setText(ui->lineEditName->text());
}
