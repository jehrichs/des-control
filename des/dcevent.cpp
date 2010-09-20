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

#include "dcevent.h"
#include "dcplace.h"

#include <QPainter>
#include <math.h>

#include <QDebug>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

DCEvent::DCEvent()
    : m_from(0)
    , m_to(0)
    , arrowSize(10)

{
    adjust();
}

void DCEvent::setName(const QString & name)
{
    m_name = name;
}

QString DCEvent::name() const
{
    return m_name;
}
void DCEvent::setPlaceFrom(DCState *from)
{
    m_from = from;
    from->addEventFrom(this);
}

DCState *DCEvent::from()
{
    return m_from;
}

void DCEvent::setPlaceTo(DCState *to)
{
    m_to = to;
    to->addEventTo(this);
}

DCState *DCEvent::to()
{
    return m_to;
}
void DCEvent::adjust()
{
    if (!m_from || !m_to)
        return;

    QLineF line(mapFromItem(m_from, m_from->centerPoint()),
                mapFromItem(m_to, m_to->centerPoint()));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {

        QPointF edgeOffset((line.dx() * 20) / length, (line.dy() * 20) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;

    } else {
        sourcePoint = destPoint = line.p1();
    }
}

QRectF DCEvent::boundingRect() const
{
    if (!m_from || !m_to)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
    .normalized()
    .adjusted(-extra, -extra, extra, extra);
}

void DCEvent::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!m_from || !m_to)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    // Draw the arrows
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

    QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                                              cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                              cos(angle - Pi + Pi / 3) * arrowSize);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}
