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

#include "srcp/serversettings.h"
#include "srcp/trainsettings.h"
#include "srcp/actuatorsettings.h"
#include "srcp/sensorsettings.h"

#include <QAbstractButton>
#include <QDebug>

SettingsDialog::SettingsDialog(HWSettings * hwsettings, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
    , m_hwsettings(hwsettings)
    , m_serverPage(new ServerSettings(hwsettings))
    , m_trainPage(new TrainSettings(hwsettings))
    , m_actuatorPage(new ActuatorSettings(hwsettings))
{
    ui->setupUi(this);

    ui->stackedWidget->addWidget(m_serverPage);
    ui->stackedWidget->addWidget(m_actuatorPage);
    ui->stackedWidget->addWidget(m_trainPage);

    createIcons();

    ui->listWidget->setCurrentRow(0);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::createIcons()
{
    QListWidgetItem *serverButton = new QListWidgetItem(ui->listWidget);
    serverButton->setIcon(QIcon(":/icons/sensor.png"));
    serverButton->setText(tr("Server"));
    serverButton->setTextAlignment(Qt::AlignHCenter);
    serverButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *actuatorButton = new QListWidgetItem(ui->listWidget);
    actuatorButton->setIcon(QIcon(":/icons/actuator.png"));
    actuatorButton->setText(tr("Actuators"));
    actuatorButton->setTextAlignment(Qt::AlignHCenter);
    actuatorButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *trainButton = new QListWidgetItem(ui->listWidget);
    trainButton->setIcon(QIcon(":/icons/train.png"));
    trainButton->setText(tr("Trains"));
    trainButton->setTextAlignment(Qt::AlignHCenter);
    trainButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

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

void SettingsDialog::clicked ( QAbstractButton * button )
{
    if(ui->buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole ||
       ui->buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole)
    {
        m_serverPage->saveChanges();
        m_trainPage->saveChanges();
        m_actuatorPage->saveChanges();
    }
}

void SettingsDialog::showPage(SettingsDialog::pageType type)
{
    ui->stackedWidget->setCurrentIndex((int)type);
    ui->listWidget->setCurrentRow((int)type);
}





