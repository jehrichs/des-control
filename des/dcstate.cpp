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

#include "dcstate.h"
#include "dcevent.h"

#include <QRectF>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>

#include <QDebug>

DCState::DCState()
    : QGraphicsItem(0)
    , m_name(QString())
    , m_marked(false)
    , m_initial(false)
    , m_circleGap(10.0)
{
    m_outside = new QGraphicsEllipseItem(this);
    m_insideText =  new QGraphicsSimpleTextItem(this);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void DCState::setName(const QString & name)
{
    m_name = name;
    setUpPlace();
}

QString DCState::name() const
{
    return m_name;
}


void DCState::setID(int id)
{
    m_id = id;
}

int DCState::id()
{
    return m_id;
}

void DCState::setMarked(bool marked)
{
    m_marked = marked;

    if(m_marked)
        m_inside = new QGraphicsEllipseItem(this);
    else
        delete m_inside;

    setUpPlace();
}

void DCState::setInitial(bool initial)
{
    m_initial = initial;


    setUpPlace();
}

void DCState::addEventFrom(DCEvent * from)
{
    m_listFrom.append(from);
}

void DCState::addEventTo(DCEvent *to)
{
    m_listTo.append(to);
}

void DCState::setUpPlace()
{
    prepareGeometryChange();

    m_insideText->setText(m_name);
    m_insideText->setPos( pos() );

    QRectF circleRect = m_insideText->boundingRect();

    circleRect.setX( m_insideText->boundingRect().x() - m_insideText->boundingRect().x()/ 2 - m_circleGap);
    circleRect.setY( m_insideText->boundingRect().y() - m_insideText->boundingRect().y()/ 2 - m_circleGap);
    circleRect.setWidth( m_insideText->boundingRect().width() + m_circleGap*2 );
    circleRect.setHeight(m_insideText->boundingRect().height() + m_circleGap*2 );

    m_outside->setRect(circleRect);

    if(m_marked)
    {
        m_inside->setRect(circleRect.x() + 2, circleRect.y() + 2,
                          circleRect.width() - 4, circleRect.height() - 4);
    }

    qDebug() << boundingRect() << m_outside->boundingRect();
}

QPointF DCState::centerPoint() const
{

    return QPointF(m_insideText->boundingRect().width()/2,
                   m_insideText->boundingRect().height()/2);
}
QGraphicsEllipseItem *DCState::outside() const
{
    return m_outside;
}

QRectF DCState::boundingRect () const
{
    return m_outside->boundingRect();
}

void DCState::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{

}

void DCState::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);

    QMenu contextMenu;
    contextMenu.addAction("Edit Place");
    contextMenu.addAction("Delete Place");
    contextMenu.exec(event->screenPos());
}

QVariant DCState::itemChange(GraphicsItemChange change, const QVariant &value)
 {
     switch (change) {
     case ItemPositionHasChanged:
         foreach (DCEvent *eventEdge, m_listFrom)
             eventEdge->adjust();
         foreach (DCEvent *eventEdge, m_listTo)
             eventEdge->adjust();
         //graph->itemMoved();
         break;
     default:
         break;
     };

     return QGraphicsItem::itemChange(change, value);
 }

 void DCState::mousePressEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mousePressEvent(event);
 }

 void DCState::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
 {
     update();
     QGraphicsItem::mouseReleaseEvent(event);
 }
