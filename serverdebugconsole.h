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

#ifndef SERVERDEBUGCONSOLE_H
#define SERVERDEBUGCONSOLE_H

#include <QDialog>

namespace Ui {
    class ServerDebugConsole;
}

class ServerDebugConsole : public QDialog
{
    Q_OBJECT

public:
    explicit ServerDebugConsole(QWidget *parent = 0);
    ~ServerDebugConsole();

private:
    Ui::ServerDebugConsole *ui;
};

#endif // SERVERDEBUGCONSOLE_H
