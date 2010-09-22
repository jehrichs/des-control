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

#ifndef GVGRAPH_H
#define GVGRAPH_H

#include <QString>
#include <QFont>
#include <QMap>
#include <QPair>
#include <QRectF>
#include <QPainterPath>
#include <QPoint>

#include <graphviz/gvc.h>

/// A struct containing the information for a GVGraph's node
struct GVNode
{
    /// The unique identifier of the node in the graph
    QString name;

    /// The position of the center point of the node from the top-left corner
    QPoint centerPos;

    /// The size of the node in pixels
    double height, width;
};

/// A struct containing the information for a GVGraph's edge
struct GVEdge
{
    int id;
    /// The source and target nodes of the edge
    QString source;
    QString target;

    /// Path of the edge's line
    QPainterPath path;

    QPointF labelPos;
};

/// An object containing a libgraph graph and its associated nodes and edges
class GVGraph
{
public:
    /// Default DPI value used by dot (which uses points instead of pixels for coordinates)
    static const qreal DotDefaultDPI;

    /// The agopen method for opening a graph
    static inline Agraph_t* _agopen(QString name, int kind)
    {
        return agopen(const_cast<char *>(qPrintable(name)), kind);
    }

    /// Add an alternative value parameter to the method for getting an object's attribute
    static inline QString _agget(void *object, QString attr, QString alt=QString())
    {
        QString str=agget(object, const_cast<char *>(qPrintable(attr)));

        if(str==QString())
            return alt;
        else
            return str;
    }

    /// Directly use agsafeset which always works, contrarily to agset
    static inline int _agset(void *object, QString attr, QString value)
    {
        return agsafeset(object, const_cast<char *>(qPrintable(attr)),
                         const_cast<char *>(qPrintable(value)),
                         const_cast<char *>(qPrintable(value)));
    }

    static inline Agsym_t* _agnodeattr (Agraph_t * graph,  QString name , QString value )
    {
        return agnodeattr(graph, const_cast<char *>(qPrintable(name)), const_cast<char *>(qPrintable(value)));
    }

    static inline Agsym_t *_agedgeattr(Agraph_t * graph, QString name, QString value)
    {
        return agedgeattr(graph, const_cast<char *>(qPrintable(name)), const_cast<char *>(qPrintable(value)));
    }


    static inline Agnode_t *_agnode(Agraph_t * graph, QString name)
    {
        return agnode(graph, const_cast<char *>(qPrintable(name)));
    }


    static inline int _gvLayout ( GVC_t * gvc, graph_t * g, QString engine)
    {
        return gvLayout(gvc, g, const_cast<char *>(qPrintable(engine)));
    }

    /*!
     * \brief Construct a Graphviz graph object
     * \param name The name of the graph, must be unique in the application
     * \param font The font to use for the graph
     * \param node_size The size in pixels of each node
     */
    GVGraph(QString name);
    ~GVGraph();

    /// Add and remove nodes
    void addNode(const QString& name, qreal width, qreal height);
    void addNode(const QString& name, qreal width, qreal height, qreal x, qreal y);

    void setRootNode(const QString& name);

    /// Add and remove edges
    void addEdge(int id, const QString& source, const QString& target);

    void applyLayout();
    QRectF boundingRect() const;

    QList<GVNode> nodes() const;
    QList<GVEdge> edges() const;

private:
    GVC_t *_context;
    Agraph_t *_graph;
    QMap<QString, Agnode_t*> _nodes;
    QMap<QPair<QString, QString>, Agedge_t*> _edges;
};

#endif // GVGRAPH_H
