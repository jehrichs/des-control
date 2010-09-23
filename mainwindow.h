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

namespace Ui {
    class MainWindow;
}

class Project;
class QSplitter;
class ProjectWidget;
class AutomatonWidget;
class QActionGroup;
class QCloseEvent;

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

    void importHardware();
    void importAutomaton();
    void exportHardware();

    void newAutomaton();
    void deleteAutomaton();
    void editAutomation();
    void runAutomaton();
    void simulateAutomaton();

    void connectToServer();
    void connectedToServer();
    void disconnectFromServer();
    void disconnectedFromServer();

    void toggleStatusBar();
    void showProjectSettings();
    void showSettings();

    void showHandbook();
    void aboutDES();

    void unsavedChanges();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void createActions();
    void createToolBars();

    void showProjectView();
    void showEmptyView();

    Ui::MainWindow *ui;
    Project *m_project;
    QSplitter *m_splitter;
    ProjectWidget *m_projectWidget;
    AutomatonWidget *m_automatonWidget;

    QActionGroup* m_autonamtonEdit;
    QActionGroup* m_autonamtonMode;

    bool m_unsavedChanges;
};

#endif // MAINWINDOW_H
