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

#include "hwconnections.h"
#include "ui_hwconnections.h"

#include "hwsettings.h"
#include "des/dcautomaton.h"
#include "des/dcevent.h"
#include "srcp/dcsensor.h"
#include "srcp/dcactuator.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>

#include <QDebug>

HWConnections::HWConnections(HWSettings *hw, DCAutomaton *automaton) :
    QDialog(0)
  , ui(new Ui::HWConnections)
  , m_hw(hw)
  , m_automaton(automaton)
{
    ui->setupUi(this);

    setupDialog();

    connect(this, SIGNAL(accepted()), this, SLOT(applyChanges()));
}

HWConnections::~HWConnections()
{
    delete ui;
}

void HWConnections::setupDialog()
{
    QVBoxLayout *baselayout = new QVBoxLayout();

    int eNum=0;
    foreach(DCEvent *e, m_automaton->getEventList())
    {
        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget(new QLabel(e->name()));
        layout->addWidget(new QLabel("<--->"));

        if(e->controlable()) {
            QComboBox * cb = new QComboBox();
            cb->setProperty("event", eNum);
            cb->setProperty("type", "controlled");

            foreach(DCActuator *a, m_hw->actuators()) {
                if(a->getType() == DCActuator::GA_SWITCH) {
                    cb->addItem(QString("%1 :: %2").arg(a->name()).arg("outer"), DCEvent::SWITCH_LEFT );
                    cb->addItem(QString("%1 :: %2").arg(a->name()).arg("inner"), DCEvent::SWITCH_RIGHT );
                }
                else {
                    cb->addItem(QString("%1 :: %2").arg(a->name()).arg("green"), DCEvent::SWITCH_LEFT );
                    cb->addItem(QString("%1 :: %2").arg(a->name()).arg("red"), DCEvent::SWITCH_RIGHT );
                }
            }

            if(e->getActuator()) {
                QString lookupString;
                switch(e->getActuatorMode()) {
                    case(DCEvent::SWITCH_LEFT):
                    if(e->getActuator()->getType() == DCActuator::GA_SWITCH) {
                        lookupString =  "outer";
                    }
                    else {
                        lookupString =  "green";
                    }
                    break;
                case(DCEvent::SWITCH_RIGHT):
                    if(e->getActuator()->getType() == DCActuator::GA_SWITCH) {
                        lookupString =  "inner";
                    }
                    else {
                        lookupString =  "red";
                    }
                break;

                }

                int index = cb->findText(QString("%1 :: %2").arg(e->getActuator()->name()).arg(lookupString));
                cb->setCurrentIndex(index);
            }

            layout->addWidget(cb);
            m_cbList.append(cb);
        }
        else {
            QComboBox * cb = new QComboBox();
            cb->setProperty("event", eNum);
            cb->setProperty("type", "uncontrolled");

            foreach(DCSensor *s, m_hw->sensors()) {
                cb->addItem(QString("%1 :: %2").arg(s->name()).arg("On"), DCEvent::HIGH );
                cb->addItem(QString("%1 :: %2").arg(s->name()).arg("Off"), DCEvent::LOW );
            }

            if(e->getSensor()) {
                QString name;
                if(e->getFBState() == DCEvent::HIGH) {
                    name = QString("%1 :: %2").arg(e->getSensor()->name()).arg("On");
                }
                else {
                    name = QString("%1 :: %2").arg(e->getSensor()->name()).arg("Off");
                }

                int index = cb->findText(name);
                cb->setCurrentIndex(index);
            }

            layout->addWidget(cb);
            m_cbList.append(cb);
        }

        baselayout->addItem(layout);
        eNum++;
    }

    ui->groupBox->setLayout(baselayout);
}

void HWConnections::applyChanges()
{
    foreach(QComboBox *cb, m_cbList) {

        DCEvent *event = m_automaton->getEventList().at(cb->property("event").toInt());

        if(cb->property("type") == "uncontrolled") {
            int selectedIndex = cb->currentIndex();
            DCEvent::FBState state = (DCEvent::FBState)cb->itemData(selectedIndex).toInt();
            QString eventName = cb->itemText(selectedIndex).split("::").first();

            foreach(DCSensor *s, m_hw->sensors()) {
                if(eventName.contains(s->name())) {
                    event->setSensor(s, state);
                    break;
                }
            }
        }
        else {
            int selectedIndex = cb->currentIndex();
            DCEvent::GAAction state = (DCEvent::GAAction)cb->itemData(selectedIndex).toInt();
            QString eventName = cb->itemText(selectedIndex).split("::").first();

            foreach(DCActuator *a, m_hw->actuators()) {
                if(eventName.contains(a->name())) {
                    event->setActuator(a, state);
                    break;
                }
            }
        }
    }
}
