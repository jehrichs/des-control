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

#ifndef DCMODELITEM_H
#define DCMODELITEM_H

#include <QObject>

class DCModelItem : public QObject
{
    Q_OBJECT
public:
    explicit DCModelItem();

    void setBusID(int busID);
    int busID() const;

    void setAddress(int address);
    int address() const;

    void setName(const QString & name);
    QString name();

    virtual bool initialize() = 0;

signals:
    void stateChanged();
    void itemChanged();
    void sendSRCPString(const QString & srcpString);

public slots:
    virtual void updateValues(const QString & srcpString) = 0;

private:
    int m_busID;
    int m_address;
    QString m_name;
};

#endif // DCMODELITEM_H
