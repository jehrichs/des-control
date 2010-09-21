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

#include <QtGui>
#include <QPen>
#include <math.h>

const qreal Pi = 3.14;

DCTransition::DCTransition()
    : QGraphicsPathItem(0)
    , m_eventText(0)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
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
    //m_eventText = new QGraphicsTextItem(this);
    //m_eventText->setPlainText(m_event->name());
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

    //return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
    //                                  line().p2().y() - line().p1().y()))
    //.normalized()
    //.adjusted(-extra, -extra, extra, extra);

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
    QPainterPath path;

    path.moveTo(m_source->center());
    path.lineTo(m_destination->center());

    //setPath(path);
    //QLineF line(m_source->center(), m_destination->center());
    //setLine(line);


}

void DCTransition::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *)
{
    if (m_source->collidesWithItem(m_destination))
        return;


    QPen myPen = pen();
    myPen.setColor(Qt::black);
    qreal arrowSize = 10;
    painter->setPen(myPen);
    //painter->setBrush(Qt::black);

//    QLineF centerLineDestination(m_source->center(), m_destination->center());
//    QPolygonF endPolygonDest = m_destination->shape().toFillPolygon();

//    QPointF pd1 = endPolygonDest.first() + m_destination->pos();
//    QPointF pd2;
//    QPointF intersectPointDestination;
//    QLineF polyLineDest;
//    for (int i = 1; i < endPolygonDest.count(); ++i) {
//        pd2 = endPolygonDest.at(i) + m_destination->pos();
//        polyLineDest = QLineF(pd1, pd2);
//        QLineF::IntersectType intersectType =
//                polyLineDest.intersect(centerLineDestination, &intersectPointDestination);
//        if (intersectType == QLineF::BoundedIntersection)
//            break;
//        pd1 = pd2;
//    }

//    QLineF centerLineSource( m_destination->center(), m_source->center());
//    QPolygonF endPolygonSource = m_source->shape().toFillPolygon();

//    QPointF ps1 = endPolygonSource.first() + m_source->pos();
//    QPointF ps2;
//    QPointF intersectPointSource;
//    QLineF polyLineSource;
//    for (int i = 1; i < endPolygonSource.count(); ++i) {
//        ps2 = endPolygonSource.at(i) + m_source->pos();
//        polyLineSource = QLineF(ps1, ps2);
//        QLineF::IntersectType intersectType =
//                polyLineSource.intersect(centerLineSource, &intersectPointSource);
//        if (intersectType == QLineF::BoundedIntersection)
//            break;
//        ps1 = ps2;
//    }

//    QPainterPath ppath;
//    ppath.moveTo(intersectPointDestination);
//    ppath.lineTo(intersectPointSource);


    QPainterPath pathOriginal = path();

    //pathOriginal.moveTo(m_source->center());
    //pathOriginal.lineTo(m_destination->center());
    //QPainterPath pp2;
    //QPainterPath pp3;

    QPainterPath source = m_source->shape();
    source.translate(m_source->pos());
    QPainterPath dest = m_destination->shape();
    dest.translate(m_destination->pos());

    QPainterPath pp2 = pathOriginal.subtracted(source);
    QPainterPath pp3 = pp2.subtracted(dest);

    //if(pathOriginal.intersects(source))
    //    qDebug() << "intersection" << pathOriginal << pp2 << pp3;
    //else
     //   qDebug() << "no intresection";
    //pp3 = pp2.subtracted(m_destination->shape());

    //setLine(QLineF(intersectPointDestination, intersectPointSource));

    //double angle = ::acos(line().dx() / line().length());
    //if (line().dy() >= 0)
    //    angle = (Pi * 2) - angle;

    //path.controlPointRect()

    //QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
     //                                       cos(angle + Pi / 3) * arrowSize);
    //QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
    //                                        cos(angle + Pi - Pi / 3) * arrowSize);

    //arrowHead.clear();
    //arrowHead << line().p1() << arrowP1 << arrowP2;

    //painter->setPen(QPen(Qt::black, 2));




    painter->drawPath(path());

    //painter->drawLine(line());

    /*
    painter->drawPolygon(arrowHead);

    if (isSelected()) {
        painter->setPen(QPen(Qt::black, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }

    if(m_event)
    {
        m_eventText->setPos(line().pointAt(0.5));
        //QPointF textPos = line().pointAt(0.5);
        //textPos.rx() +=2;
        //textPos.ry() +=2;
        //painter->drawText(textPos, m_event->name());
    }
    */

}
