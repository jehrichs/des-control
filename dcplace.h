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

#ifndef DCPLACE_H
#define DCPLACE_H

#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QList>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSimpleTextItem>
#include <QAction>

class DCEvent;

class DCPlace : public QGraphicsItem
{

public:
    explicit DCPlace(QGraphicsItem *parent = 0);

    void setName(const QString & name);
    QString name() const;

    void setMarked(bool marked);
    void setInitial(bool initial);

    void addEventFrom(DCEvent * from);
    void addEventTo(DCEvent *to);

    QPointF centerPoint() const;
    QGraphicsEllipseItem *outside() const;

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    QRectF boundingRect () const;
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    void createActions();
    void setUpPlace();

    QString m_name;
    QList<DCEvent*> m_listFrom;
    QList<DCEvent*> m_listTo;

    bool m_marked;
    bool m_initial;

    QGraphicsEllipseItem *m_outside;
    QGraphicsEllipseItem *m_inside;
    QGraphicsSimpleTextItem *m_insideText;

    qreal m_circleGap;

    QAction *m_editPlace;
    QAction *m_deletePlace;
};

#endif // DCPLACE_H
