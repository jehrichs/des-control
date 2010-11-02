#ifndef EVENTSTATUS_H
#define EVENTSTATUS_H

#include <QDialog>

#include <QLabel>
#include <QPushButton>

#include "dcevent.h"

namespace Ui {
    class EventStatus;
}
class DCAutomaton;

class EventStatus : public QDialog
{
    Q_OBJECT

public:
    explicit EventStatus(QWidget *parent = 0);
    ~EventStatus();

    void setAutomaton(DCAutomaton* automaton);


private slots:
    void updateStatus();

private:

    struct eventrow
    {
        DCEvent* event;
        QLabel *name;
        QLabel *status;
        QPushButton *button;
    };

    void setupDialog();

    Ui::EventStatus *ui;
    DCAutomaton* m_automaton;

    QList <eventrow> eventList;
};

#endif // EVENTSTATUS_H
