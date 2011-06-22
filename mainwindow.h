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
#include "des/dcautomaton.h"

namespace Ui {
    class MainWindow;
}

class HWSettings;
class Project;
class DCController;
class QSplitter;
class ProjectWidget;
class AutomatonWidget;
class QActionGroup;
class QCloseEvent;
class EventStatus;
class ImportAutomaton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void writeSettings();
    void readSettings();

public Q_SLOTS:
    void newProject();
    void openProject();
    void saveProject();
    void saveProjectAs();
    bool closeProject();

    void importAutomaton();
    void importFinished(DCAutomaton* ia);
    void deleteAutomaton();

    void editHWConnection();

    void editAutomaton();
    void runAutomaton();
    void simulateAutomaton();
    void showEventStatus();

    void connectToServer();
    void connectedToServer();
    void disconnectFromServer();
    void disconnectedFromServer();
    void showHWDebugConsole();

    void toggleStatusBar();
    void showProjectSettings();
    void showHandbook();
    void aboutDES();

    void unsavedChanges();

    void noAutomatonVisible();
    void someAutomataVisible();
    void switchOpendAutomaton(DCAutomaton::SceneMode currentMode);

    void printSessionHistory(const QString & msg);

protected:
    void closeEvent(QCloseEvent *event);

private:
    void createActions();
    void createToolBars();

    void showProjectView();
    void showEmptyView();

    Ui::MainWindow *ui;
    HWSettings *m_hw;
    Project *m_project;
    DCController * m_controller;
    QSplitter *m_splitter;
    ProjectWidget *m_projectWidget;
    AutomatonWidget *m_automatonWidget;
    ImportAutomaton *m_importer;

    QActionGroup* m_autonamtonMode;
    QActionGroup* m_controllerState;

    bool m_unsavedChanges;

    EventStatus *m_es;
};

#endif // MAINWINDOW_H
