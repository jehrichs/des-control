#include "eventstatus.h"
#include "ui_eventstatus.h"

#include "dcautomaton.h"
#include "dcstate.h"
#include "dcevent.h"
#include "dctransition.h"

#include <QDebug>
#include <QLabel>
#include <QGridLayout>

EventStatus::EventStatus(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::EventStatus)
{
    ui->setupUi(this);
}

EventStatus::~EventStatus()
{
    delete ui;
}

void EventStatus::setAutomaton(DCAutomaton* automaton)
{
    m_automaton = automaton;

    setupDialog();
}

void EventStatus::setupDialog()
{
    QList<DCEvent*> events =  m_automaton->getEventList();

    int i= 1;
    foreach(DCEvent *event, events)
    {
        eventrow er;
        er.event = event;
        er.name = new QLabel(event->name());
        if(event->isActive())
            er.status = new QLabel("active");
        else
            er.status = new QLabel("inactive");

        er.button = new QPushButton("toggle");
        er.button->setProperty("event", i);

        connect(er.button, SIGNAL(clicked()), er.event, SLOT(toggleStatus()));
        connect(er.event, SIGNAL( statusChanged()) , this, SLOT(updateStatus()));

        ui->gridLayout->addWidget(er.name, i, 0);
        ui->gridLayout->addWidget(er.status, i, 1);

        ui->gridLayout->addWidget(er.button, i, 2);

        i++;

        eventList.append(er);
    }
}

void EventStatus::updateStatus()
{
    DCEvent* sender = dynamic_cast<DCEvent*>( this->sender() );

    foreach(eventrow er, eventList)
    {
        if(er.event == sender)
        {
            if(er.event->isActive())
                er.status->setText("active");
            else
                er.status->setText("inactive");
        }
    }
}
