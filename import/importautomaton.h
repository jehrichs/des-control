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


#include <QThread>
#include <QXmlStreamReader>
#include <QList>

class QIODevice;
class DCAutomaton;

class ImportAutomaton : public QThread
{
    Q_OBJECT

public:
    enum AutomatonFile
    {
        DESUMA,
        SUPREMICA
    };

    explicit ImportAutomaton(AutomatonFile fileType, const QString & filename);
    ~ImportAutomaton();

    void run();

    void loadAutomaton();
    DCAutomaton* getAutomaton();
    void setAutomaton(DCAutomaton* newAutomaton);

signals:
    void importfinished(DCAutomaton* ia);
    void importstatus(const QString & msg);
    void importvalue (int value);

private:
    void loadDesumaFile(QIODevice *m_device);
    void loadSupremicaFile(QIODevice *m_device);

    void addSupEvent(DCAutomaton* automaton);
    void addSupState(DCAutomaton* automaton);
    void addSupTransition(DCAutomaton* automaton);

    AutomatonFile m_fileType;
    QString m_filename;
    QXmlStreamReader m_reader;
    DCAutomaton* m_automaton;

    bool m_useShortnames;
};

#endif // IMPORTAUTOMATON_H
