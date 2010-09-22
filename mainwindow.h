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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "des/dccontroller.h"

namespace Ui {
    class MainWindow;
}

class Project;
class ProjectTreeView;
class QSplitter;
class DCAutomaton;
class AutomatonView;
class QToolBar;
class QActionGroup;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public Q_SLOTS:
    void showSettings();
    void showAbout();
    void showDebugConsole();

    void newProject();
    void openProject();
    void saveProject();
    void saveProjectAs();
    void closeProject();

    void importAutomaton();

    void connectServer();
    void connectedToServer();
    void disconnectServer();

    void newAutomata();
    void deleteAutomata();
    void showAutomaton(DCAutomaton* automaton);

    void createController(DCController::ControlMode mode);

private:
    void showProjectView();
    void showEmptyView();
    void createToolbar();

    Ui::MainWindow *ui;
    Project *m_project;
    ProjectTreeView *m_projectView;
    QSplitter *m_splitter;
    AutomatonView *m_automatView;

    QToolBar *m_automatonToolBar;
    QActionGroup* m_autonamtonEdit;
    DCController *m_controler;
};

#endif // MAINWINDOW_H
