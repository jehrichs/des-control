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

#include "dcautomaton.h"

#include "dcstate.h"
#include "dctransition.h"
#include "dcevent.h"

#include "gvgraph.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QDebug>

DCAutomaton::DCAutomaton(ImportMode mode, QObject *parent)
    : QGraphicsScene(parent)
    , m_visualMode(mode)
    , m_sceneMode(Edit)
    , m_type(Plant)
    , m_name("Automata")
    , m_line(0)
{
    setSceneRect ( 0, 0, 1000, 1000 );
}

DCAutomaton::~DCAutomaton()
{
    delete  m_line;

    m_stateList.clear();
    m_transitionList.clear();
    m_eventList.clear();
}

void DCAutomaton::setSceneMode(SceneMode mode)
{
    m_sceneMode = mode;

    switch(m_sceneMode)
    {
    case Edit:
        setBackgroundBrush(Qt::white);
        break;
    case Run:
        setBackgroundBrush(Qt::lightGray);
        break;
    }

    update();
}

DCAutomaton::SceneMode DCAutomaton::sceneMode()
{
    return m_sceneMode;
}

void DCAutomaton::setName(const QString & name)
{
    m_name = name;
}

QString DCAutomaton::name() const
{
    return m_name;
}

void DCAutomaton::setAutomatonType(AutomatonType type)
{
    m_type = type;
}

DCAutomaton::AutomatonType DCAutomaton::automatonType() const
{
    return m_type;
}

void DCAutomaton::addState(DCState* newState)
{
    if(m_visualMode == DCAutomaton::Visual) {
        addItem(newState);
        newState->setPos(50,50);
    }
    m_stateList.append(newState);
}

void DCAutomaton::addTransition(DCTransition *newtransition)
{
    if(m_visualMode == DCAutomaton::Visual) {
        addItem(newtransition);
    }
    m_transitionList.append(newtransition);
}

void DCAutomaton::addEvent(DCEvent *newEvent)
{
    m_eventList.append(newEvent);
}

QList<DCTransition *> DCAutomaton::getTransitionList()
{
    return m_transitionList;
}

QList<DCEvent *> DCAutomaton::getEventList()
{
    return m_eventList;
}

DCState *DCAutomaton::getStateFromId(int id)
{
    foreach(DCState* state, m_stateList)
    {
        if(state->id() == id)
            return state;
    }

    return 0;
}

DCEvent *DCAutomaton::getEventFromId(int id)
{
    foreach(DCEvent* event, m_eventList)
    {
        if(event->id() == id)
            return event;
    }

    return 0;
}

DCTransition *DCAutomaton::getTransitionFromId(int id)
{
    foreach(DCTransition* transition, m_transitionList)
    {
        if(transition->id() == id)
            return transition;
    }

    return 0;
}

DCState *DCAutomaton::getInitialState()
{
    foreach(DCState* state, m_stateList)
    {
        if(state->isInitial())
            return state;
    }

    return 0;
}

void DCAutomaton::selectItem()
{
    m_mode = MoveItem;
}

void DCAutomaton::addState()
{
    m_mode = InsertPlace;
}

void DCAutomaton::addEvent()
{
    m_mode = InsertEvent;
}

void DCAutomaton::doLayout()
{
    if(m_visualMode != DCAutomaton::Visual) {
        return;
    }

    GVGraph gvTest("layout1");

    foreach(DCState *state, m_stateList)
    {
        gvTest.addNode(QString("%1").arg(state->id()), state->ellipseBounds().width(), state->ellipseBounds().height());

        if(state->isInitial())
            gvTest.setRootNode(QString("%1").arg(state->id()));
    }

    int i = 0;
    foreach(const DCTransition *transition, m_transitionList)
    {
        gvTest.addEdge( i,
                        QString("%1").arg(transition->sourceState()->id()),
                        QString("%1").arg(transition->destinationState()->id()));

        i++;
    }

    gvTest.applyLayout();

    foreach(const GVNode & node, gvTest.nodes())
    {
        getStateFromId(node.name.toInt())->setCenterPoint(node.centerPos);
    }

    foreach(const GVEdge & edge, gvTest.edges())
    {
        m_transitionList.at(edge.id)->setPath(edge.path);
        m_transitionList.at(edge.id)->automaticLabelPosition();
    }

    setSceneRect(gvTest.boundingRect().adjusted(-100,-100,100,100));
}

void DCAutomaton::lineLayout()
{
    foreach(DCTransition *transition, m_transitionList)
    {
        transition->pathToLine();
    }
}

void DCAutomaton::bezierLayout()
{
    foreach(DCTransition *transition, m_transitionList)
    {
        transition->pathToBezier();
    }
}

void DCAutomaton::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void DCAutomaton::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_mode == InsertEvent && m_line != 0) {
        QLineF newLine(m_line->line().p1(), mouseEvent->scenePos());
        m_line->setLine(newLine);
    } else if (m_mode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void DCAutomaton::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_line != 0 && m_mode == InsertEvent) {
        QList<QGraphicsItem *> startItems = items(m_line->line().p1());
        if (startItems.count() && startItems.first() == m_line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(m_line->line().p2());
        if (endItems.count() && endItems.first() == m_line)
            endItems.removeFirst();

        removeItem(m_line);
        delete m_line;

        //qDebug() << "startItems" << startItems.count() << startItems.first()->type() << DCPlace::Type;

        // foreach(QGraphicsItem *item,startItems)
        // qDebug() << item->type();

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.last()->type() == DCState::Type &&
            endItems.last()->type() == DCState::Type &&
            startItems.last() != endItems.last()) {
            DCState *startItem =
                    qgraphicsitem_cast<DCState *>(startItems.last());
            DCState *endItem =
                    qgraphicsitem_cast<DCState *>(endItems.last());
            DCTransition *transition = new DCTransition();

            transition->setStates(startItem, endItem);

            transition->updatePosition();
            addItem(transition);
        }
    }
    m_line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
