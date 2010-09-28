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
#include "dctransition.h"
#include "dcevent.h"

#include <QRectF>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>

#include <QDebug>

DCState::DCState()
    : QGraphicsEllipseItem(0)
    , m_name(QString())
    , m_marked(false)
    , m_initial(false)
    , m_isHovered(false)
    , m_circleGap(10.0)
    , m_isActive(false)
{
    m_insideText =  new QGraphicsSimpleTextItem(this);
    m_insideText->setZValue(11);
    setZValue(10);

    setRect(0,0,40,40);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptsHoverEvents(true);
}

DCState::~DCState()
{
    delete m_insideText;
    delete m_markedCircle;
    delete m_editPlace;
    delete m_deletePlace;

}

void DCState::setId(int id)
{
    m_id = id;
}

int DCState::id()
{
    return m_id;
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

void DCState::setMarked(bool marked)
{
    m_marked = marked;

    if(m_marked)
        m_markedCircle = new QGraphicsEllipseItem(this);
    else
        delete m_markedCircle;

    setUpPlace();
}

bool DCState::marked() const
{
    return m_marked;
}

void DCState::setInitial(bool initial)
{
    m_initial = initial;

    setUpPlace();
}

bool DCState::isInitial() const
{
    return m_initial;
}

void DCState::addTransitionFrom(DCTransition * from)
{
    m_listFrom.append(from);
}

void DCState::addTransitionTo(DCTransition *to)
{
    m_listTo.append(to);
}

QList<DCTransition*> DCState::outgoingTransitions()
{
    return m_listFrom;
}

void DCState::setActive(bool active)
{
    m_isActive = active;
    update();
}

void DCState::setUpPlace()
{
    prepareGeometryChange();

    m_insideText->setText(m_name);
    m_insideText->setPos( m_circleGap, m_circleGap );

    QRectF circleRect = m_insideText->boundingRect();

    circleRect.setX( 0);
    circleRect.setY( 0);
    circleRect.setWidth ( m_insideText->boundingRect().width() + m_circleGap*2 );
    circleRect.setHeight( m_insideText->boundingRect().height() + m_circleGap*2 );

    setRect(circleRect);

    if(m_marked)
    {
        m_markedCircle->setRect(circleRect.x() + 4, circleRect.y() + 4,
                                circleRect.width() - 8, circleRect.height() - 8);
    }
}

QPointF DCState::center()
{
    return QPointF(pos().x() + ellipseBounds().width() / 2,
                   pos().y() + ellipseBounds().height() / 2);
}

void DCState::setCenterPoint(const QPoint & point)
{
    setPos(point.x() - ellipseBounds().width() / 2,
           point.y() - ellipseBounds().height() / 2);
}

QSizeF DCState::ellipseBounds()
{
    return QSizeF(rect().width(), rect().height());
}

QRectF DCState::boundingRect () const
{
    QRectF rect = QGraphicsEllipseItem::boundingRect();
    return rect.adjusted(-10,-10,10,10);
}

void DCState::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(isSelected())
    {
        QPen selectPen(Qt::red, 5, Qt::SolidLine);
        painter->setPen(selectPen);
        painter->drawEllipse(rect());
    }
    if(m_isHovered)
    {
        QPen selectPen(Qt::blue, 5, Qt::SolidLine);
        painter->setPen(selectPen);
        painter->drawEllipse(rect());
    }

    QColor lineColor;
    QColor fillColor;
    if(m_isActive)
    {
        lineColor = QColor(Qt::green);
        fillColor = QColor(Qt::green);
    }
    else
    {
        lineColor = QColor(Qt::black);
        fillColor = QColor(Qt::white);
    }

    QPen pen(lineColor, 1, Qt::SolidLine);
    QBrush brush(fillColor);

    painter->setPen(pen);
    painter->setBrush(brush);

    painter->drawEllipse(rect());

    //QBrush debug(Qt::transparent);
    //painter->setBrush(debug);
    //painter->drawRect(boundingRect());
}

void DCState::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    m_listFrom.first()->event()->setActive(!m_listFrom.first()->event()->isActive());

    qDebug() << m_listFrom.first()->event()->name() << m_listFrom.first()->isActive();

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
        foreach (DCTransition *edge, m_listFrom)
            edge->updatePosition();
        foreach (DCTransition *edge, m_listTo)
            edge->updatePosition();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

// void DCState::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
// {
//     if (event->modifiers() & Qt::ShiftModifier) {
//         setPos(event->pos());
//         update();
//         return;
//     }
//     QGraphicsItem::mouseMoveEvent(event);
// }

void DCState::hoverEnterEvent ( QGraphicsSceneHoverEvent * event )
{
    Q_UNUSED(event);
    m_isHovered = true;
    update();
}

void DCState::hoverLeaveEvent ( QGraphicsSceneHoverEvent * event )
{
    Q_UNUSED(event);
    m_isHovered = false;
    update();
}
