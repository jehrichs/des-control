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

#include "dcserver.h"
#include <QtNetwork>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QMessageBox>

#include <QDebug>

DCServer::DCServer(QObject *parent)
    : QObject(parent)
    , m_connectionState(Disconnected)
{
    m_tcpSocket = new QTcpSocket(this);

    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readSRCPInput()));
    connect(m_tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnectFromSRCP()));
    connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
}

void DCServer::setHost(const QString & host)
{
    m_host = host;
}

QString DCServer::ip()
{
    return m_host;
}

void DCServer::setPort(const QString & port)
{
    m_port = port;
}

QString DCServer::port()
{
    return m_port;
}

void DCServer::setConnectionMode(DCServer::ConnectionMode mode)
{
    m_connectionMode = mode;
}

DCServer::ConnectionMode DCServer::connectionMode()
{
    return m_connectionMode;
}

void DCServer::connectSRCP()
{
    if(m_tcpSocket->state() == QAbstractSocket::UnconnectedState)
    {
        m_tcpSocket->connectToHost(m_host, m_port.toInt());
    }
}

void DCServer::disconnectSRCP()
{
    if( m_tcpSocket->state() == QAbstractSocket::ConnectedState )
    {
        qDebug() << "send kill request";
        m_tcpSocket->write(QString("TERM 0 SESSION %1\n").arg(m_sessionId).toAscii());
        m_tcpSocket->waitForBytesWritten(500);
        m_tcpSocket->disconnectFromHost();

        m_connectionState = Disconnected;
    }
}

void DCServer::sendSRCP(const QString & srcpString)
{
    qDebug() << "write srcp string" << QString("%1\n").arg(srcpString).toAscii();
    m_tcpSocket->write(QString("%1\n").arg(srcpString).toAscii());
    m_tcpSocket->waitForBytesWritten();
}

void DCServer::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        displayError(tr("SRCP Server"),
                     tr("The host was not found. Please check the "
                        "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        displayError(tr("SRCP Server"),
                     tr("The connection was refused by the peer. "
                        "Make sure the srcp server is running, "
                        "and check that the host name and port "
                        "settings are correct."));
        break;
    default:
        displayError(tr("SRCP Server"),
                     tr("The following error occurred: %1.")
                     .arg(m_tcpSocket->errorString()));
    }
}

void DCServer::displayError(const QString & titel, const QString & msg)
{
    // maybe emit a signal and show the error otherwise
    QMessageBox::information(0, titel,msg );
}

void DCServer::disconnectFromSRCP()
{
    if(m_tcpSocket->state() != QAbstractSocket::UnconnectedState)
    {
        m_tcpSocket->abort();
        m_connectionState = Disconnected;
    }

    emit connectionClosed();
}

void DCServer::readSRCPInput()
{
    QString readLine = m_tcpSocket->readLine();

    switch(m_connectionState)
    {
        // reply from server we expect is the welcome message and thus SRCP Version
    case Disconnected:
        if(parseWelcomeMessage(readLine.simplified()))
        {
            // succesfull connected
            m_connectionState = Handshake;
            handshake();
        }
        break;

    case Handshake:
        //expected reply -> 200 OK GO <ID>
        //TODO support other handshake commands

        if( parseSRCP( readLine.simplified() ) )
        {
            // succesfull started session
            m_connectionState = Connected;

            emit connectionEstablished();
        }
        break;

    case Connected:
                parseSRCP( readLine.simplified() );
        break;
    }
}

bool DCServer::parseSRCP( const QString & srcpString )
{
    QRegExp rxlen("(\\d{10}).\\d{3}\\s(\\d{3})\\s(OK|INFO|ERROR)\\s(.*)");
    int pos = rxlen.indexIn( srcpString );

    if (pos > -1) {

        QDateTime timeStamp = QDateTime::fromTime_t(rxlen.cap(1).toUInt());
        QString returnCode = rxlen.cap(2);
        QString returnString = rxlen.cap(3);
        QString returnText = rxlen.cap(4);

        qDebug() << "[" << timeStamp.time().toString() << "]" << returnCode << returnString << returnText;

        if(returnCode.toInt() < 200)
        {
            displayError(tr("SRCP Server Info"),tr("Info Code: %1.\n\n%2").arg(returnCode).arg(returnText) );
            return true;
        }
        else if(returnCode.toInt() >= 400)
        {
            displayError(tr("SRCP Server Error"),tr("Error Code: %1.\n\n%2").arg(returnCode).arg(returnText) );
            return false;
        }
        // Handshake GO message
        else if(returnCode.toInt() == 200)
        {
            QStringList handshakeGo = returnText.split(' ');
            m_sessionId = handshakeGo.last().toInt();
            return true;
        }
    }

    else
    {
        qDebug() << "srcp parsing failed" << srcpString;
    }

    return false;
}

bool DCServer::parseWelcomeMessage(const QString & msg)
{

    QStringList wm = msg.split("; ");

    foreach(const QString &wmPart, wm)
    {
        QStringList partsList = wmPart.split(' ');
        if(partsList.first() == "SRCP")
        {
            m_srcpVersion = partsList.last();
        }
        else if (partsList.first() == "SRCPOTHER")
        {
            m_srcpOtherVersion = partsList.last();
        }
        else
        {
            m_serverVersion = wmPart;
        }
    }

    //no srcp version found
    //TODO extend to srcp version check
    if(m_srcpVersion.isEmpty())
        return false;

    else
        return true;

}

void DCServer::handshake()
{
    //default settings are used
    // - command mode
    // - default srcp protocoll from the server
    //TODO maybe set other supportet protocoll here

    //start session
    m_tcpSocket->write("GO\n");
}
