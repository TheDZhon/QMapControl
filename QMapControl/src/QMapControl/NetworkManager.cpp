/*
*
* This file is part of QMapControl,
* an open-source cross-platform map widget
*
* Copyright (C) 2007 - 2008 Kai Winter
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with QMapControl. If not, see <http://www.gnu.org/licenses/>.
*
* Contact e-mail: kaiwinter@gmx.de
* Program URL   : http://qmapcontrol.sourceforge.net/
*
*/

#include "NetworkManager.h"

// Qt includes.
#include <QtCore/QMutexLocker>
#include <QtGui/QImageReader>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

namespace qmapcontrol
{
    NetworkManager::NetworkManager(QObject* parent) : QObject(parent)
    {
        // Connect signal/slot to handle proxy authentication.
        QObject::connect(&m_nam, &QNetworkAccessManager::proxyAuthenticationRequired, this, &NetworkManager::proxyAuthenticationRequired);

        // Connect signal/slot to handle finished downloads.
        QObject::connect(&m_nam, &QNetworkAccessManager::finished, this, &NetworkManager::downloadFinished);
    }

    NetworkManager::~NetworkManager()
    {
        // Ensure all download queues are aborted.
        abortDownloads();
    }

    void NetworkManager::setProxy(const QNetworkProxy& proxy)
    {
        // Set the proxy on the network access manager.
        m_nam.setProxy(proxy);
    }

    void NetworkManager::abortDownloads()
    {
        // Loop through each reply to abort and then remove it from the downloading image queue.
        QMutexLocker lock(&m_mutex_downloading_image);
        QMutableMapIterator<QNetworkReply*, QUrl> itr(m_downloading_image);
        while(itr.hasNext())
        {
            // Tell the reply to abort.
            itr.next().key()->abort();

            // Remove it from the queue.
            itr.remove();
        }
    }

    int NetworkManager::downloadQueueSize() const
    {
        // Default return value.
        int return_size(0);

        // Return the size of the downloading image queue.
        QMutexLocker lock(&m_mutex_downloading_image);
        return_size += m_downloading_image.size();

        // Return the size.
        return return_size;
    }

    bool NetworkManager::isDownloading(const QUrl& url) const
    {
        // Return whether we requested url is downloading image queue.
        QMutexLocker lock(&m_mutex_downloading_image);
        return m_downloading_image.values().contains(url);
    }

    void NetworkManager::downloadImage(const QUrl& url)
    {
        // Keep track of our success.
        bool success(false);

        // Scope this as we later call "downloadQueueSize()" which also locks all download queue mutexes.
        {
            // Gain a lock to protect the downloading image container.
            QMutexLocker lock(&m_mutex_downloading_image);

            // Check this is a new request.
            if(m_downloading_image.values().contains(url) == false)
            {
                // Generate a new request.
                QNetworkRequest request(url);
                request.setRawHeader("User-Agent", "QMapControl");

                // Send the request.
                QNetworkReply* reply = m_nam.get(request);

                // Store the request into the downloading image queue.
                m_downloading_image[reply] = url;

                // Mark our success.
                success = true;

                // Log success.
#ifdef QMAP_DEBUG
                qDebug() << "Downloading image '" << url << "'";
#endif
            }
        }

        // Was we successful?
        if(success)
        {
            // Emit that we are downloading a new image (with details of the current queue size).
            emit downloadingInProgress(downloadQueueSize());
        }
    }

    void NetworkManager::proxyAuthenticationRequired(const QNetworkProxy& proxy, QAuthenticator* authenticator)
    {
        // Log proxy authentication request.
        qDebug() << "Proxy Authentication Required for '" << proxy.hostName() << "' with the authenticator '" << &authenticator << "'";

        // We need to capture the proxy login details.
        // Setup a form to capture these details.
        QDialog dialog;
        QGridLayout layout;

        // Add username/password text fields.
        QLabel username, password;
        username.setText("Username:");
        password.setText("Password:");
        layout.addWidget(&username, 0, 0);
        layout.addWidget(&password, 1, 0);
        QLineEdit user, pass;
        pass.setEchoMode(QLineEdit::Password);
        layout.addWidget(&user, 0, 1);
        layout.addWidget(&pass, 1, 1);

        // Setup signals to exit the form when the return key is entered.
        QObject::connect(&user, &QLineEdit::returnPressed, &dialog, &QDialog::accept);
        QObject::connect(&pass, &QLineEdit::returnPressed, &dialog, &QDialog::accept);

        // Add an "OK" button.
        QPushButton button;
        button.setText("OK");
        layout.addWidget(&button, 2, 0, 1, 2, Qt::AlignCenter);

        // Setup signal to exit the form when the button is clicked.
        QObject::connect(&button, &QPushButton::clicked, &dialog, &QDialog::accept);

        // Set the layout to the dialog to display.
        dialog.setLayout(&layout);

        // Run the form.
        dialog.exec();

        // Set the form's username/password values into the authenticator to use.
        authenticator->setUser(user.text());
        authenticator->setPassword(pass.text());
    }

    void NetworkManager::downloadFinished(QNetworkReply* reply)
    {
        // Did the reply return no errors...
        if(reply->error() != QNetworkReply::NoError)
        {
#ifdef QMAP_DEBUG
            // Log error.
            qDebug() << "Failed to download '" << reply->url() << "' with error '" << reply->errorString() << "'";
#endif
        }
        else
        {
            // Check whether the url has already been processed...
            bool continue_processing_image(false);
            {
                // Is the reply in the downloading image queue?
                QMutexLocker lock(&m_mutex_downloading_image);
                continue_processing_image = m_downloading_image.contains(reply);
                if(continue_processing_image)
                {
#ifdef QMAP_DEBUG
                    // Log success.
                    qDebug() << "Downloaded image '" << m_downloading_image[reply] << "'";
#endif

                    // Remove it from the downloading image queue.
                    m_downloading_image.remove(reply);
                }
            }

            // Should we process this as an image download.
            if(continue_processing_image)
            {
                // Emit that we have downloaded an image.
                QImageReader image_reader(reply);
                emit imageDownloaded(reply->url(), QPixmap::fromImageReader(&image_reader));
            }
        }

        // If the reply did not fail due to cancellation... (as cancellation locks our mutexes!)
        if(reply->error() != QNetworkReply::OperationCanceledError)
        {
            // Check if the current download queue is empty.
            if(downloadQueueSize() == 0)
            {
                // Emit that all queued downloads have finished.
                emit downloadingFinished();
            }
        }
    }
}
