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

#ifndef DCTRANSITION_H
#define DCTRANSITION_H

#include <QGraphicsLineItem>
#include <QRectF>
#include <QPainterPath>

class QGraphicsPolygonItem;
class QGraphicsSceneMouseEvent;
class QPainterPath;

class DCEvent;
class DCState;

class DCTransition : public QGraphicsLineItem
{
public:
    DCTransition();

    void setId(int id);
    int id() const;

    void setEvent(DCEvent *event);
    DCEvent* event() const;

    void setStates(DCState* from, DCState *to);
    DCState* sourceState() const;
    DCState* destinationState() const;

    void updatePosition();

    enum { Type = UserType + 10 };
    int type() const { return Type; }

    QPainterPath shape() const;

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget = 0);

private:
    int m_id;
    DCEvent *m_event;
    DCState* m_source;
    DCState* m_destination;

    QPolygonF arrowHead;
    QGraphicsTextItem *m_eventText;

};

#endif // DCTRANSITION_H
