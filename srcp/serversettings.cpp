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

#include "serversettings.h"
#include "ui_serversettings.h"

#include "hwsettings.h"
#include "srcp/dcserver.h"

ServerSettings::ServerSettings(HWSettings * hwsettings, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServerSettings)
    , m_hwsettings(hwsettings)
{
    ui->setupUi(this);

    ui->lineEditIP->setText(m_hwsettings->server()->ip());
    ui->lineEditPort->setText(m_hwsettings->server()->port());
}

ServerSettings::~ServerSettings()
{
    delete ui;
}

void ServerSettings::saveChanges()
{
    m_hwsettings->server()->setHost(ui->lineEditIP->text());
    m_hwsettings->server()->setPort(ui->lineEditPort->text());
}
