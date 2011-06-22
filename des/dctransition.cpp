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

#include "dctransition.h"

#include "dcstate.h"
#include "dcevent.h"

#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <math.h>

const qreal Pi = 3.14;

DCTransition::DCTransition()
    : QGraphicsPathItem(0)
    , m_eventText(0)
{
    m_eventText = new QGraphicsTextItem(this);
    m_eventText->hide();
    m_eventText->setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_eventText->setFlag(QGraphicsItem::ItemIsMovable, true);

    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setZValue(5);
}

void DCTransition::setId(int id)
{
    m_id = id;
}

int DCTransition::id() const
{
    return m_id;
}

void DCTransition::setEvent(DCEvent *event)
{
    m_event = event;
    m_eventText->setPlainText(m_event->name());
    m_eventText->show();
    automaticLabelPosition();
}

DCEvent* DCTransition::event() const
{
    return m_event;
}

void DCTransition::setStates(DCState* from, DCState *to)
{
    m_source = from;
    from->addTransitionFrom(this);
    m_destination = to;
    to->addTransitionTo(this);

    DCEvent *event = new DCEvent();
    event->setName("test");
    setEvent(event);
}

DCState* DCTransition::sourceState() const
{
    return m_source;
}

DCState* DCTransition::destinationState() const
{
    return m_destination;
}

QRectF DCTransition::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QGraphicsPathItem::boundingRect().adjusted(-extra, -extra, extra, extra);
}

QPainterPath DCTransition::shape() const
{
    QPainterPath path = QGraphicsPathItem::shape();
    path.addPolygon(arrowHead);
    return path;
}

void DCTransition::updatePosition()
{
    QPainterPath original = path();

    if(!original.isEmpty())
    {
        original.setElementPositionAt(0, m_source->center().x(), m_source->center().y());
        original.setElementPositionAt(original.elementCount()-1, m_destination->center().x(), m_destination->center().y());
    }

    setPath(original);
}

void DCTransition::setLabelPosition(const QPointF & pos)
{
    m_eventText->setPos(pos);
}

void DCTransition::automaticLabelPosition()
{
    QLineF arrowLine(m_source->center(), m_destination->center());

    QPointF labelPos = path().pointAtPercent(0.5);

    qreal angle = arrowLine.angle();
    if(angle > 180.0 && angle < 360.0 )
        labelPos.rx() -= 5 + m_eventText->boundingRect().width();
    else
        labelPos.rx() += 5;
    m_eventText->setPos(labelPos);
}

void DCTransition::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *)
{
    if (m_source->collidesWithItem(m_destination))
        return;

    if(isSelected())
    {
        painter->setPen(QPen(Qt::red, 3));
        painter->setBrush(QBrush(Qt::NoBrush));
        painter->drawPath(path());

        for(int i=1; i<path().elementCount(); i++)
        {
            if(path().elementAt(i).type == QPainterPath::CurveToDataElement)
            {
                painter->drawEllipse(path().elementAt(i).x,path().elementAt(i).y,5,5);
            }
        }
    }

    createArrow();

    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path());

    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(Qt::black);

    painter->drawPolygon(arrowHead);
}


void DCTransition::createArrow()
{
    qreal arrowSize = 10;

    //find arrow position
    QPainterPath original = path();
    QPainterPath dest = m_destination->shape();
    dest.translate(m_destination->pos());

    QPainterPath destClip = original.intersected(dest);

    if(destClip.isEmpty())
    {
        int last = original.elementCount()-1;
        original.lineTo(original.elementAt(last).x-1, original.elementAt(last).y-1);

        dest = m_destination->shape();
        dest.translate(m_destination->pos());

        destClip = original.intersected(dest);
    }

    if(!destClip.isEmpty())
    {
        QPointF arrowPosition1( destClip.elementAt(0).x, destClip.elementAt(0).y);
        QPointF arrowPosition2( destClip.elementAt(1).x, destClip.elementAt(1).y);
        QLineF arrowLine = QLineF(arrowPosition1, arrowPosition2);

        double angle = ::acos(arrowLine.dx() / arrowLine.length());
        if (arrowLine.dy() >= 0)
            angle = (Pi * 2) - angle;

        QPointF arrowP1 = arrowLine.p1() - QPointF(sin(angle + Pi / 3) * arrowSize,
                                                   cos(angle + Pi / 3) * arrowSize);
        QPointF arrowP2 = arrowLine.p1() - QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                   cos(angle + Pi - Pi / 3) * arrowSize);

        arrowHead.clear();
        arrowHead << arrowLine.p1() << arrowP1 << arrowP2;
    }

}
