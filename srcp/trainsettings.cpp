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

#include "trainsettings.h"
#include "ui_trainsettings.h"

#include "project.h"
#include "dctrain.h"

TrainSettings::TrainSettings(Project * project)
    : QWidget(0)
    , ui(new Ui::TrainSettings)
    , m_project(project)
{
    ui->setupUi(this);

    foreach(DCTrain *train, m_project->trains())
    {
        ui->listWidgetTrains->addItem(train->name());
    }

    connect(ui->listWidgetTrains, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(showNewItemInfo(QListWidgetItem*,QListWidgetItem*)));

}

TrainSettings::~TrainSettings()
{
    delete ui;
}

void TrainSettings::showNewItemInfo ( QListWidgetItem * current, QListWidgetItem * previous )
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
    //TODO check if there are unsaved chanegs in the old item

    //we assume no sorting in the listview thus => row of item = position in the project QList

    int currentRow = ui->listWidgetTrains->currentRow();
    if(m_project->trains().size() > currentRow && currentRow >= 0)
    {
        showItem(m_project->trains().at(currentRow));
    }
    else
    {
        ui->lineEditName->setText(QString());
        ui->spinBoxAddress->setValue(-1);
        ui->comboBoxProtocol->setCurrentIndex(0);

        ui->spinBoxDecoderFunctions->setValue(-1);
        ui->spinBoxSpeedSteps->setValue(-1);
    }
}

void TrainSettings::showItem(DCTrain *train)
{
    ui->lineEditName->setText(train->name());
    ui->spinBoxAddress->setValue(train->address());
    ui->comboBoxProtocol->setCurrentIndex(train->protocol());

    ui->spinBoxDecoderFunctions->setValue(train->decoderFunctions());
    ui->spinBoxSpeedSteps->setValue(train->decoderSteps());
}

void TrainSettings::newTrain()
{
    DCTrain *train = new DCTrain();
    int itemNumber = m_project->trains().size()+1;
    train->setName(tr("NewTrain%1").arg(itemNumber));

    m_project->addTrain(train);
    ui->listWidgetTrains->addItem(train->name());
    ui->listWidgetTrains->setCurrentRow(itemNumber-1);
    showItem(train);
}

void TrainSettings::deleteCurrent()
{
    int currentRow = ui->listWidgetTrains->currentRow();

    if(currentRow != -1)
    {
        m_project->removeTrain(m_project->trains().at(currentRow));
        ui->listWidgetTrains->takeItem(currentRow);
    }
}

void TrainSettings::saveChanges()
{
    int currentRow = ui->listWidgetTrains->currentRow();

    if(currentRow < 0)
        return;

    DCTrain *train = m_project->trains().at(currentRow);

    train->setName(ui->lineEditName->text());
    train->setAddress(ui->spinBoxAddress->value());
    train->setProtocol((DCTrain::GLProtocol)ui->comboBoxProtocol->currentIndex());
    train->setDecoderFunctions(ui->spinBoxDecoderFunctions->value());
    train->setDecoderSteps(ui->spinBoxSpeedSteps->value());

    QListWidgetItem *cur = ui->listWidgetTrains->currentItem();
    cur->setText(ui->lineEditName->text());
}
