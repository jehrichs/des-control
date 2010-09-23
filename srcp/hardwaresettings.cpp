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

#include "hardwaresettings.h"
#include "ui_hardwaresettings.h"

#include "trainsettings.h"
#include "actuatorsettings.h"
#include "sensorsettings.h"

HardwareSettings::HardwareSettings(Project *project, QWidget *parent)
    :  QDialog(parent)
    , ui(new Ui::HardwareSettings)
    , m_project(project)
    , m_trainPage(new TrainSettings(project))
    , m_actuatorPage(new ActuatorSettings(project))
    , m_sensorPage(new SensorSettings(project))
{
    ui->setupUi(this);

    ui->stackedWidget->addWidget(m_actuatorPage);
    ui->stackedWidget->addWidget(m_sensorPage);
    ui->stackedWidget->addWidget(m_trainPage);

    createIcons();

    ui->listWidget->setCurrentRow(0);
}

HardwareSettings::~HardwareSettings()
{
    delete ui;
    delete m_trainPage;
    delete m_actuatorPage;
    delete m_sensorPage;
}

void HardwareSettings::createIcons()
{
    m_trainButton = new QListWidgetItem(ui->listWidget);
    m_trainButton->setIcon(QIcon(":/icons/train.png"));
    m_trainButton->setText(tr("Trains"));
    m_trainButton->setTextAlignment(Qt::AlignHCenter);
    m_trainButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    m_actuatorButton = new QListWidgetItem(ui->listWidget);
    m_actuatorButton->setIcon(QIcon(":/icons/actuator.png"));
    m_actuatorButton->setText(tr("Actuators"));
    m_actuatorButton->setTextAlignment(Qt::AlignHCenter);
    m_actuatorButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    m_sensorButton = new QListWidgetItem(ui->listWidget);
    m_sensorButton->setIcon(QIcon(":/icons/sensor.png"));
    m_sensorButton->setText(tr("Sensors"));
    m_sensorButton->setTextAlignment(Qt::AlignHCenter);
    m_sensorButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(ui->listWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}

void HardwareSettings::showPage(int type)
{
    switch(type)
    {
    case 1:
        changePage(m_trainButton, 0);
        break;
    case 2:
        changePage(m_actuatorButton, 0);
        break;
    case 3:
        changePage(m_sensorButton, 0);
        break;
    case 4:
        changePage(m_trainButton, 0);
        break;
    case 5:
        changePage(m_actuatorButton, 0);
        break;
    case 6:
        changePage(m_sensorButton, 0);
        break;
    default:
        m_trainButton->setSelected(true);
        return;
    }
}

void HardwareSettings::selectItem()
{

}

void HardwareSettings::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    ui->stackedWidget->setCurrentIndex(ui->listWidget->row(current));
}
