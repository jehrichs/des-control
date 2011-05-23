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

#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QList>

class DCAutomaton;
class QIODevice;

class Project : public QObject
{
    Q_OBJECT
public:
    Project();

    void setName(const QString& name);
    QString name() const;

    void setDescription(const QString& description);
    QString description() const;

    void setFileName(const QString& name);
    QString fileName() const;

    void addAutomaton(DCAutomaton* newAutomaton);
    void removeAutomaton(DCAutomaton* automaton);
    QList<DCAutomaton*> automata() const;

signals:
    void projectChanged();
    void updateAutomata();

private:
    QString m_name;
    QString m_description;

    QString m_filename;
    QList<DCAutomaton*> m_automata;
};

#endif // PROJECT_H
