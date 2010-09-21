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

#ifndef DCAUTOMATON_H
#define DCAUTOMATON_H

#include <QObject>
#include <QGraphicsScene>

#include <QString>
#include <QAction>

class DCState;
class DCTransition;
class DCEvent;

class DCAutomaton : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode { InsertPlace, InsertEvent, InsertText, MoveItem };

    enum AutomatonType
    {
        Plant,
        Specification,
        Supervisor,
        Property
    };

    DCAutomaton(QObject *parent = 0);

    void setName(const QString & name);
    QString name() const;

    void setAutomatonType(AutomatonType type);
    DCAutomaton::AutomatonType automatonType() const;

    void addState(DCState* newState);
    void addTransition(DCTransition *newtransition);
    void addEvent(DCEvent *newEvent);

    DCState *getStateFromId(int id);
    DCEvent *getEventFromId(int id);

    void doLayout();

public slots:
    void selectItem();
    void addState();
    void addEvent();
    void deleteSelected();
    void editSelected();

private:
    DCState *newState();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

    //void drawBackground ( QPainter * painter, const QRectF & rect );

private:
    AutomatonType m_type;
    QString m_name;
    Mode m_mode;
    QGraphicsLineItem *m_line;

    QList<DCState *> m_stateList;
    QList<DCTransition *> m_transitionList;
    QList<DCEvent *> m_eventList;
};

#endif // DCAUTOMATON_H
