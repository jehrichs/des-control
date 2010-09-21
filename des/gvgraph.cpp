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

#include "gvgraph.h"

#include <QStringList>

/*! Dot uses a 72 DPI value for converting it's position coordinates from points to pixels
    while we display at 96 DPI on most operating systems. */
const qreal GVGraph::DotDefaultDPI=72.0;


GVGraph::GVGraph(QString name) :
        _context(gvContext()),
        _graph(_agopen(name, AGDIGRAPHSTRICT)) // Strict directed graph, see libgraph doc
        //AGDIGRAPHSTRICT
        //AGRAPHSTRICT
{
    //Set graph attributes
    _agset(_graph, "overlap", "prism");
    _agset(_graph, "splines", "true");
    //_agset(_graph, "pad", "0,2");
    _agset(_graph, "dpi", "96,0");
    //_agset(_graph, "nodesep", "0,4");
    //_agset(_graph, "rankdir", "LR");

    //Set default attributes for the future nodes
    _agnodeattr(_graph, "fixedsize", "false");
    _agnodeattr(_graph, "regular", "false");
    _agedgeattr(_graph, "headclip", "true");
    _agedgeattr(_graph, "tailclip", "true");
    _agedgeattr(_graph, "arrowhead", "normal");
    _agedgeattr(_graph, "arrowtail", "none");
}

GVGraph::~GVGraph()
{
    gvFreeLayout(_context, _graph);
    agclose(_graph);
    gvFreeContext(_context);
}

void GVGraph::addNode(const QString& name, qreal width, qreal height)
{
    if(!_nodes.contains(name)) {

    //_agset
    _nodes.insert(name, _agnode(_graph, name));

    QString nodePtsHeight = QString("%1").arg(height/_agget(_graph, "dpi", "96,0").toDouble());
    QString nodePtsWidth = QString("%1").arg(width/_agget(_graph, "dpi", "96,0").toDouble());
    //GV uses , instead of . for the separator in floats
    _agset(_nodes.value(name), "height", nodePtsHeight.replace('.', ","));
    _agset(_nodes.value(name), "width", nodePtsWidth.replace('.', ","));
}
}

void GVGraph::setRootNode(const QString& name)
{
    if(_nodes.contains(name))
        _agset(_graph, "root", name);
}

void GVGraph::addEdge(int id, const QString &source, const QString &target)
{
    if(_nodes.contains(source) && _nodes.contains(target))
    {
        QPair<QString, QString> key(source, target);
        if(!_edges.contains(key))
        {
            Agedge_t* edge = agedge(_graph, _nodes[source], _nodes[target]);
            _edges.insert(key, edge);

            _agset(edge, "label", QString("%1").arg(id));
        }
    }
}

void GVGraph::applyLayout()
{
    gvFreeLayout(_context, _graph);
    _gvLayout(_context, _graph, "dot");
    //_gvLayout(_context, _graph, "neato");
    //_gvLayout(_context, _graph, "circo");
}

QRectF GVGraph::boundingRect() const
{
    qreal dpi=_agget(_graph, "dpi", "96,0").toDouble();
    return QRectF(_graph->u.bb.LL.x*(dpi/DotDefaultDPI), _graph->u.bb.LL.y*(dpi/DotDefaultDPI),
                  _graph->u.bb.UR.x*(dpi/DotDefaultDPI), _graph->u.bb.UR.y*(dpi/DotDefaultDPI));
}

QList<GVNode> GVGraph::nodes() const
{
    QList<GVNode> list;
    qreal dpi=_agget(_graph, "dpi", "96,0").toDouble();
    for(QMap<QString, Agnode_t*>::const_iterator it=_nodes.begin(); it!=_nodes.end();++it)
    {
        Agnode_t *node=it.value();
        GVNode object;

        //Set the name of the node
        object.name=node->name;

        //Fetch the X coordinate, apply the DPI conversion rate (actual DPI / 72, used by dot)
        qreal x=node->u.coord.x*(dpi/DotDefaultDPI);

        //Translate the Y coordinate from bottom-left to top-left corner
        qreal y=(_graph->u.bb.UR.y - node->u.coord.y)*(dpi/DotDefaultDPI);
        object.centerPos=QPoint(x, y);

        //Transform the width and height from inches to pixels
        object.height=node->u.height*dpi;
        object.width=node->u.width*dpi;

        list << object;
    }

    return list;
}

QList<GVEdge> GVGraph::edges() const
{
    QList<GVEdge> list;
    qreal dpi=_agget(_graph, "dpi", "96,0").toDouble();

    for(QMap<QPair<QString, QString>, Agedge_t* >::const_iterator it=_edges.constBegin(); it != _edges.constEnd(); ++it)
    {
        Agedge_t *edge=it.value(); //.first;
        GVEdge object;

        //Fill the source and target node names
        object.source=edge->tail->name;
        object.target=edge->head->name;
        object.id = _agget(edge, "label", "-1").toInt();

        //Calculate the path from the spline (only one spline, as the graph is strict. If it
        //wasn't, we would have to iterate over the first list too)
        //Calculate the path from the spline (only one as the graph is strict)
        if((edge->u.spl->list!=0) && (edge->u.spl->list->size%3 == 1))
        {
            //If there is a starting point, draw a line from it to the first curve point
            if(edge->u.spl->list->sflag)
            {
                object.path.moveTo(edge->u.spl->list->sp.x*(dpi/DotDefaultDPI),
                                   (_graph->u.bb.UR.y - edge->u.spl->list->sp.y)*(dpi/DotDefaultDPI));
                object.path.lineTo(edge->u.spl->list->list[0].x*(dpi/DotDefaultDPI),
                                   (_graph->u.bb.UR.y - edge->u.spl->list->list[0].y)*(dpi/DotDefaultDPI));
            }
            else
                object.path.moveTo(edge->u.spl->list->list[0].x*(dpi/DotDefaultDPI),
                                   (_graph->u.bb.UR.y - edge->u.spl->list->list[0].y)*(dpi/DotDefaultDPI));

            //Loop over the curve points
            for(int i=1; i<edge->u.spl->list->size; i+=3)
                object.path.cubicTo(edge->u.spl->list->list[i].x*(dpi/DotDefaultDPI),
                                    (_graph->u.bb.UR.y - edge->u.spl->list->list[i].y)*(dpi/DotDefaultDPI),
                                    edge->u.spl->list->list[i+1].x*(dpi/DotDefaultDPI),
                                    (_graph->u.bb.UR.y - edge->u.spl->list->list[i+1].y)*(dpi/DotDefaultDPI),
                                    edge->u.spl->list->list[i+2].x*(dpi/DotDefaultDPI),
                                    (_graph->u.bb.UR.y - edge->u.spl->list->list[i+2].y)*(dpi/DotDefaultDPI));

            //If there is an ending point, draw a line to it
            if(edge->u.spl->list->eflag)
                object.path.lineTo(edge->u.spl->list->ep.x*(dpi/DotDefaultDPI),
                                   (_graph->u.bb.UR.y - edge->u.spl->list->ep.y)*(dpi/DotDefaultDPI));
        }

        list << object;
    }

    return list;
}
