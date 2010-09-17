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

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "serversettings.h"
#include "trainsettings.h"
#include "actuatorsettings.h"
#include "sensorsettings.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
    , m_serverPage(new ServerSettings)
    , m_trainPage(0)
    , m_actuatorPage(0)
    , m_sensorPage(0)
{
    ui->setupUi(this);

    ui->stackedWidget->addWidget(new QWidget);
    ui->stackedWidget->addWidget(m_serverPage);
    ui->stackedWidget->addWidget(m_trainPage);
    ui->stackedWidget->addWidget(m_actuatorPage);
    ui->stackedWidget->addWidget(m_sensorPage);

    createIcons();

    ui->listWidget->setCurrentRow(0);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::createIcons()
{
    QListWidgetItem *generalButton = new QListWidgetItem(ui->listWidget);
    generalButton->setIcon(QIcon(":/icons/sensor.png"));
    generalButton->setText(tr("General"));
    generalButton->setTextAlignment(Qt::AlignHCenter);
    generalButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *serverButton = new QListWidgetItem(ui->listWidget);
    serverButton->setIcon(QIcon(":/icons/sensor.png"));
    serverButton->setText(tr("Server"));
    serverButton->setTextAlignment(Qt::AlignHCenter);
    serverButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *trainButton = new QListWidgetItem(ui->listWidget);
    trainButton->setIcon(QIcon(":/icons/train.png"));
    trainButton->setText(tr("Trains"));
    trainButton->setTextAlignment(Qt::AlignHCenter);
    trainButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *actuatorButton = new QListWidgetItem(ui->listWidget);
    actuatorButton->setIcon(QIcon(":/icons/actuator.png"));
    actuatorButton->setText(tr("Actuators"));
    actuatorButton->setTextAlignment(Qt::AlignHCenter);
    actuatorButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *sensorButton = new QListWidgetItem(ui->listWidget);
    sensorButton->setIcon(QIcon(":/icons/sensor.png"));
    sensorButton->setText(tr("Sensors"));
    sensorButton->setTextAlignment(Qt::AlignHCenter);
    sensorButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(ui->listWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}
void SettingsDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    ui->stackedWidget->setCurrentIndex(ui->listWidget->row(current));
}
