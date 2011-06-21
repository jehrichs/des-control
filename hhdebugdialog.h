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

#ifndef HHDEBUGDIALOG_H
#define HHDEBUGDIALOG_H

#include <QDialog>

namespace Ui {
    class HHDebugDialog;
}

class QTimer;
class HWSettings;

class HHDebugDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HHDebugDialog(HWSettings *hw);
    ~HHDebugDialog();

public slots:
    void train1Start100();
    void train1Start50();
    void train1Start25();
    void train1Stop();
    void train2Start100();
    void train2Start50();
    void train2Start25();
    void train2Stop();
    void switch1Left();
    void switch1Right();
    void switch2Left();
    void switch2Right();
    void switch3Left();
    void switch3Right();
    void switch4Left();
    void switch4Right();
    void updateSensors();
    void changeSensorState();
    void signal1iRed();
    void signal1iGreen();
    void signal1oRed();
    void signal1oGreen();
    void signal2iRed();
    void signal2iGreen();
    void signal2oRed();
    void signal2oGreen();

private:
    Ui::HHDebugDialog *ui;
    HWSettings *m_hw;
    QTimer *m_cycleTimer;
};

#endif // HHDEBUGDIALOG_H
