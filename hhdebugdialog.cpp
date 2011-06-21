/*
 * Copyright 2011 Jörg Ehrichs <joerg.ehichs@gmx.de>
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

#include "hhdebugdialog.h"
#include "ui_hhdebugdialog.h"

#include "hwsettings.h"
#include "srcp/dctrain.h"
#include "srcp/dcactuator.h"
#include "srcp/dcsensor.h"

#include <QTimer>
#include <QProcess>
#include <QDebug>

HHDebugDialog::HHDebugDialog(HWSettings *hw) :
    QDialog(0)
  , ui(new Ui::HHDebugDialog)
  , m_hw(hw)
  , m_cycleTimer(new QTimer(this))
{
    ui->setupUi(this);

    QList<DCSensor*> sensors =  m_hw->sensors();

    foreach(DCSensor *s, sensors)
    {
        connect(s, SIGNAL(sensorChanged()), this, SLOT(changeSensorState()));
    }

    connect(m_cycleTimer, SIGNAL(timeout()), this, SLOT(updateSensors()));
    m_cycleTimer->start(1);
}

HHDebugDialog::~HHDebugDialog()
{
    delete ui;
    delete m_cycleTimer;
}

void HHDebugDialog::train1Start100()
{
    DCTrain *t = m_hw->trains().at(0);
    t->setSpeed(200);
    t->setmaxSpeed(200);
    t->setDriveMode(DCTrain::FORWARD);
    t->start();
}

void HHDebugDialog::train1Start50()
{
    DCTrain *t = m_hw->trains().at(0);
    t->setSpeed(100);
    t->setmaxSpeed(200);
    t->setDriveMode(DCTrain::FORWARD);
    t->start();
}

void HHDebugDialog::train1Start25()
{
    DCTrain *t = m_hw->trains().at(0);
    t->setSpeed(50);
    t->setmaxSpeed(200);
    t->setDriveMode(DCTrain::FORWARD);
    t->start();
}

void HHDebugDialog::train1Stop()
{
    DCTrain *t = m_hw->trains().at(0);
    t->stop();
}

void HHDebugDialog::train2Start100()
{
    DCTrain *t = m_hw->trains().at(1);
    t->setSpeed(200);
    t->setmaxSpeed(200);
    t->setDriveMode(DCTrain::FORWARD);
    t->start();
}

void HHDebugDialog::train2Start50()
{
    DCTrain *t = m_hw->trains().at(1);
    t->setSpeed(100);
    t->setmaxSpeed(200);
    t->setDriveMode(DCTrain::FORWARD);
    t->start();
}

void HHDebugDialog::train2Start25()
{
    DCTrain *t = m_hw->trains().at(1);
    t->setSpeed(50);
    t->setmaxSpeed(200);
    t->setDriveMode(DCTrain::FORWARD);
    t->start();
}

void HHDebugDialog::train2Stop()
{
    DCTrain *t = m_hw->trains().at(1);
    t->stop();
}

void HHDebugDialog::switch1Left()
{
    DCActuator *a = m_hw->actuators().at(0);
    a->switchLeft();
}

void HHDebugDialog::switch1Right()
{
    DCActuator *a = m_hw->actuators().at(0);
    a->switchRight();
}

void HHDebugDialog::switch2Left()
{
    DCActuator *a = m_hw->actuators().at(1);
    a->switchLeft();
}

void HHDebugDialog::switch2Right()
{
    DCActuator *a = m_hw->actuators().at(1);
    a->switchRight();
}

void HHDebugDialog::switch3Left()
{
    DCActuator *a = m_hw->actuators().at(2);
    a->switchLeft();
}

void HHDebugDialog::switch3Right()
{
    DCActuator *a = m_hw->actuators().at(2);
    a->switchRight();
}

void HHDebugDialog::switch4Left()
{
    DCActuator *a = m_hw->actuators().at(3);
    a->switchLeft();
}

void HHDebugDialog::switch4Right()
{
    DCActuator *a = m_hw->actuators().at(3);
    a->switchRight();
}
void HHDebugDialog::signal1iGreen()
{
    DCActuator *a = m_hw->actuators().at(4);
    a->switchLeft();
}

void HHDebugDialog::signal1iRed()
{
    DCActuator *a = m_hw->actuators().at(4);
    a->switchRight();
}

void HHDebugDialog::signal1oGreen()
{
    DCActuator *a = m_hw->actuators().at(5);
    a->switchLeft();
}

void HHDebugDialog::signal1oRed()
{
    DCActuator *a = m_hw->actuators().at(5);
    a->switchRight();
}

void HHDebugDialog::signal2iGreen()
{
    DCActuator *a = m_hw->actuators().at(6);
    a->switchLeft();
}

void HHDebugDialog::signal2iRed()
{
    DCActuator *a = m_hw->actuators().at(6);
    a->switchRight();
}

void HHDebugDialog::signal2oGreen()
{
    DCActuator *a = m_hw->actuators().at(7);
    a->switchLeft();
}

void HHDebugDialog::signal2oRed()
{
    DCActuator *a = m_hw->actuators().at(7);
    a->switchRight();
}

void HHDebugDialog::updateSensors()
{
    QString  cmd = QString("readsensors");
    QString inputstring;

    QProcess setConf;
    setConf.start( cmd );

    if( !setConf.waitForStarted() ) {
        qDebug() << "can't start process readsensors";
        return;
    }

    if( !setConf.waitForFinished() ) {
        qDebug() << "can't wait for process readsensors";
        return;
    }

    QByteArray output = setConf.readAll();

    if( !output.isEmpty() ) {
        inputstring = QString(output);

        QList<DCSensor*> sensorList = m_hw->sensors();
        foreach(DCSensor* sensor, sensorList)
        {
            sensor->updateValues(inputstring);
        }
    }
    else {
        qDebug() << "could not read sensor input";
    }
}

void HHDebugDialog::changeSensorState()
{
    QList<DCSensor*> sensors =  m_hw->sensors();

    foreach(DCSensor *s, sensors)
    {
        QString status;
        if(s->value() == 1) {
            status = QString("train detected");
        }
        else {
            status = QString("-------");
        }

        switch(s->address()) {
        case 1:
            ui->status_p11->setText(status);
            break;
        case 2:
            ui->status_p12->setText(status);
            break;
        case 3:
            ui->status_p21->setText(status);
            break;
        case 4:
            ui->status_p22->setText(status);
            break;
        }
    }
}
