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

#ifndef DCEVENT_H
#define DCEVENT_H

#include <QGraphicsItem>

class DCPlace;

class DCEvent : public QGraphicsItem
{

public:
    DCEvent();

    void setPlaceFrom(DCPlace *from);
    DCPlace *from();
    void setPlaceTo(DCPlace *to);
    DCPlace *to();

    void adjust();

    enum { Type = UserType + 2 };
    int type() const { return Type; }

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QString m_name;
    DCPlace *m_from;
    DCPlace *m_to;

    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;

};

#endif // DCEVENT_H
