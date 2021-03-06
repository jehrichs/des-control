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

#ifndef AUTOMATONWIDGET_H
#define AUTOMATONWIDGET_H

#include <QWidget>

#include "dcautomaton.h"

namespace Ui {
    class AutomatonWidget;
}

class AutomatonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AutomatonWidget(QWidget *parent = 0);
    ~AutomatonWidget();

    DCAutomaton * currentAutomaton() const;

signals:
    void lastAutomatonClosed();
    void firstAutomatonOpend();
    void switchOpendAutomaton(DCAutomaton::SceneMode);

public slots:
    void openAutomaton(DCAutomaton* automaton);
    void closeTab(int index);
    void closeTab(DCAutomaton* automaton);
    void openAutomatonChanged(int index);

private:
    Ui::AutomatonWidget *ui;
};

#endif // AUTOMATONWIDGET_H
