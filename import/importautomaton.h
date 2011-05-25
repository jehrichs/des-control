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

#ifndef IMPORTAUTOMATON_H
#define IMPORTAUTOMATON_H


#include <QObject>
#include <QXmlStreamReader>
#include <QList>

class QIODevice;
class DCAutomaton;

class ImportAutomaton : public QObject
{
    Q_OBJECT

public:
    enum AutomatonFile
    {
        DESUMA,
        SUPREMICA
    };

    explicit ImportAutomaton(QObject *parent = 0);
    ~ImportAutomaton();

    QList<DCAutomaton*> loadAutomaton(AutomatonFile fileType, QIODevice *device);

private:
    QList<DCAutomaton*> loadDesumaFile(QIODevice *device);
    QList<DCAutomaton*> loadSupremicaFile(QIODevice *device);

    void addSupEvent(DCAutomaton* automaton);
    void addSupState(DCAutomaton* automaton);
    void addSupTransition(DCAutomaton* automaton);

    QXmlStreamReader reader;
    QList<DCAutomaton*> m_automatonList;

    bool m_useShortnames;
};

#endif // IMPORTAUTOMATON_H
