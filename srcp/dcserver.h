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

#ifndef DCSERVER_H
#define DCSERVER_H

#include <QObject>
#include <QAbstractSocket>

class QTcpSocket;

class DCServer : public QObject
{

    enum ConnectionMode
    {
        SERVER_COMMAND_MODE,
        SERVER_INFO_MODE
    };

    enum ConnectionState
    {
        Disconnected,
        Handshake,
        Connected
    };

    Q_OBJECT
public:
    explicit DCServer(QObject *parent = 0);

    void setHost(const QString & host);
    QString ip();

    void setPort(const QString & port);
    QString port();

    void setConnectionMode(DCServer::ConnectionMode mode);
    DCServer::ConnectionMode connectionMode();


signals:
    void connectionEstablished();
    void connectionClosed();

public slots:
    void connectSRCP();
    void disconnectSRCP();

    void sendSRCP(const QString & srcpString);

private slots:
    void handshake();
    void displayError(QAbstractSocket::SocketError socketError);
    void displayError(const QString & titel, const QString & msg);
    void disconnectFromSRCP();

    void readSRCPInput();
    bool parseSRCP(const QString & srcpString);

private:
    bool parseWelcomeMessage(const QString & msg);


    QString m_host;
    QString m_port;
    QTcpSocket *m_tcpSocket;

    ConnectionMode m_connectionMode;
    ConnectionState m_connectionState;

    QString m_serverVersion;
    QString m_srcpVersion;
    QString m_srcpOtherVersion;
    int     m_sessionId;
};

#endif // DCSERVER_H
