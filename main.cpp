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

#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(application);

    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Uni Bremen IAE");
    QCoreApplication::setOrganizationDomain("uni-bremen.de");
    QCoreApplication::setApplicationName("DES-Control");

    MainWindow mainWin;
    mainWin.show();

    return app.exec();
}
