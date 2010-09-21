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
#include <QtGui>
#include <QPen>
#include <QDebug>

DCAutomaton::DCAutomaton(QObject *parent)
    : QGraphicsScene(parent)
    , m_type(Plant)
    , m_name("Automata")
{
    setSceneRect ( 0, 0, 1000, 1000 );
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

    addItem(newState);
    newState->setPos(50,50);
    m_stateList.append(newState);
}

void DCAutomaton::addTransition(DCTransition *newtransition)
{

    addItem(newtransition);
    m_transitionList.append(newtransition);
}

void DCAutomaton::addEvent(DCEvent *newEvent)
{

    //addItem(newEvent);
    m_eventList.append(newEvent);
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

void DCAutomaton::selectItem()
{
    m_mode = MoveItem;
    qDebug() << "select item";
}

void DCAutomaton::addState()
{
    m_mode = InsertPlace;
}

DCState * DCAutomaton::newState()
{
    DCState *newState = new DCState();
    newState->setName("ON");
    newState->setMarked(true);

    addItem(newState);
    m_stateList.append(newState);
    return newState;

}

void DCAutomaton::addEvent()
{
    m_mode = InsertEvent;
}

void DCAutomaton::deleteSelected()
{

}


void DCAutomaton::editSelected()
{

}

void DCAutomaton::doLayout()
{
    GVGraph gvTest("layout1");

    foreach(DCState *state, m_stateList)
    {
        gvTest.addNode(QString("%1").arg(state->id()), state->ellipseBounds().width(), state->ellipseBounds().height());

        if(state->isInitial())
            gvTest.setRootNode(QString("%1").arg(state->id()));
    }

    int i = 0;
    foreach(DCTransition *transition, m_transitionList)
    {
        gvTest.addEdge( i,
                        QString("%1").arg(transition->sourceState()->id()),
                        QString("%1").arg(transition->destinationState()->id()));

        i++;
    }

    gvTest.applyLayout();

    foreach(GVNode node, gvTest.nodes())
    {
        getStateFromId(node.name.toInt())->setCenterPoint(node.centerPos);
    }

    foreach(GVEdge edge, gvTest.edges())
    {
        m_transitionList.at(edge.id)->setPath(edge.path);
    }

    setSceneRect(gvTest.boundingRect());
    //update(sceneRect());

}

void DCAutomaton::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //if (mouseEvent->button() != Qt::RightButton)
        //qDebug() << m_stateList.size() << m_transitionList.size() << m_eventList.size();
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    switch (m_mode) {
    case InsertPlace:
        {
            DCState *place = newState();
            place->setPos(mouseEvent->scenePos());
        }
        //emit itemInserted(item);
        break;
    case InsertEvent:
        m_line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                              mouseEvent->scenePos()));
        m_line->setPen(QPen(Qt::black, 2));
        addItem(m_line);
        break;
    case InsertText:
        //        textItem = new DiagramTextItem();
        //        textItem->setFont(myFont);
        //        textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        //        textItem->setZValue(1000.0);
        //        connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)),
        //                this, SLOT(editorLostFocus(DiagramTextItem*)));
        //        connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
        //                this, SIGNAL(itemSelected(QGraphicsItem*)));
        //        addItem(textItem);
        //        textItem->setDefaultTextColor(myTextColor);
        //        textItem->setPos(mouseEvent->scenePos());
        //emit textInserted(textItem);
    default:
        ;
    }

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

//void DCAutomaton::drawBackground ( QPainter * painter, const QRectF & rect )
//{

//    QPen pen;
//    pen.setStyle(Qt::DashLine);
//    pen.setWidth(1);
//    pen.setBrush(Qt::green);

//    painter->setPen(pen);

//    painter->drawRect(sceneRect());

//    pen.setBrush(Qt::gray);
//    painter->setPen(pen);

//    for(int i=0; i <= sceneRect().width(); i +=10)
//    {
//        painter->drawLine(i, 0, i, sceneRect().height());
//    }

//    for(int j=0; j <= sceneRect().height(); j +=10)
//    {
//        painter->drawLine(0, j, sceneRect().width(), j);
//    }
//}
