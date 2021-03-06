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

#ifndef DCCONTROLLER_H
#define DCCONTROLLER_H

#include <QObject>

class QTimer;
class DCAutomaton;
class DCState;
class HWSettings;

class DCController : public QObject
{
    Q_OBJECT
public:
    enum ControlMode { Off, Simulation, Live };

    explicit DCController(QObject *parent = 0);
    ~DCController();

    void setMode(ControlMode mode);
    DCController::ControlMode mode() const;

    void setAutomaton(DCAutomaton* automaton);
    DCAutomaton* automaton() const;

    void setHWSettings(HWSettings *hw);

    bool isRunnung();
    bool isPaused();

signals:
    void curState(DCState *curstate);
    void historyEntry(const QString &entry);

public slots:
    void startController();
    void stopController();
    void pauseController(bool paused);

private slots:
    void updateDES();

private:
    ControlMode m_mode;
    QTimer *m_cycleTimer;
    DCAutomaton* m_automaton;
    HWSettings *m_hwSettings;
    DCState* m_initialState;
    DCState* m_currentState;

    bool m_running;
    bool m_paused;
    int m_tmp;
};

#endif // DCCONTROLLER_H
