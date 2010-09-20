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

#include "dcplace.h"
#include "dcevent.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QtGui>
#include <QDebug>

DCAutomaton::DCAutomaton(QObject *parent)
    : QGraphicsScene(parent)
    , m_type(Plant)
    , m_name("Automata")
{
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

DCState *DCAutomaton::getStateFromId(int id)
{
    foreach(DCState* state, m_stateList)
    {
        if(state->id() == id)
            return state;
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

void DCAutomaton::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
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
}

void DCAutomaton::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_mode == InsertEvent && m_line != 0) {
        QLineF newLine(m_line->line().p1(), mouseEvent->scenePos());
        m_line->setLine(newLine);
    } else if (m_mode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
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
            DCEvent *event = new DCEvent();

            event->setPlaceFrom(startItem);
            event->setPlaceTo(endItem);

            qDebug() << startItem->pos() << endItem->pos();

            event->adjust();
            addItem(event);
        }
    }
    m_line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
