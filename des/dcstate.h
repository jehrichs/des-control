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

#ifndef DCSTATE_H
#define DCSTATE_H

#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QList>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSimpleTextItem>
#include <QAction>
#include <QPointF>

class DCTransition;

class DCState : public QGraphicsEllipseItem
{

public:
    DCState();
    ~DCState();

    void setId(int id);
    int id();

    void setName(const QString & name);
    QString name() const;

    void setMarked(bool marked);
    bool marked() const;

    void setInitial(bool initial);
    bool isInitial() const;

    void addTransitionFrom(DCTransition * from);
    void addTransitionTo(DCTransition *to);

    QList<DCTransition*> outgoingTransitions();

    void setActive(bool active);


    enum { Type = UserType + 1 };
    int type() const { return Type; }

    QPointF center();
    QSizeF ellipseBounds();
    void setCenterPoint(const QPoint & point);

    QRectF boundingRect () const;
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );

private:
    void createActions();
    void setUpPlace();

    int m_id;
    QString m_name;
    QList<DCTransition*> m_listFrom;
    QList<DCTransition*> m_listTo;

    bool m_marked;
    bool m_initial;

    QGraphicsEllipseItem *m_markedCircle;
    QGraphicsSimpleTextItem *m_insideText;
    bool m_isHovered;

    qreal m_circleGap;
    QAction *m_editPlace;
    QAction *m_deletePlace;

    bool m_isActive;
};

#endif // DCSTATE_H
