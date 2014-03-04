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

#pragma once

// Qt includes.
#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QUrl>
#include <QtGui/QPixmap>
#include <QtNetwork/QAuthenticator>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkProxy>

// Local includes.
#include "qmapcontrol_global.h"

/*!
 * @author Kai Winter <kaiwinter@gmx.de>
 * @author Chris Stylianou <chris5287@gmail.com>
 */
namespace qmapcontrol
{
    class QMAPCONTROL_EXPORT NetworkManager : public QObject
    {
        Q_OBJECT
    public:
        //! Constructor.
        /*!
         * This construct a Network Manager.
         * @param parent QObject parent ownership.
         */
        explicit NetworkManager(QObject* parent = 0);

        //! Disable copy constructor.
        ///NetworkManager(const NetworkManager&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///NetworkManager& operator=(const NetworkManager&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        ~NetworkManager();

        /*!
         * Set the network proxy to use.
         * @param proxy The network proxy to use.
         */
        void setProxy(const QNetworkProxy& proxy);

        /*!
         * Aborts all current downloading threads.
         * This is useful when changing the zoom-factor, though newly needed images loads faster
         */
        void abortDownloads();

        /*!
        * Get the number of current downloads.
        * @return size of the downloading queues.
        */
        int downloadQueueSize() const;

        /*!
         * Checks if the given url resource is currently being downloaded.
         * @param url The url of the resource.
         * @return boolean, if the url resource is already downloading.
         */
        bool isDownloading(const QUrl& url) const;

    public slots:
        /*!
         * Downloads an image resource for the given url.
         * @param url The image url to download.
         */
        void downloadImage(const QUrl& url);

    signals:
        /*!
         * Signal emitted when a resource has been queued for download.
         * @param count The current size of the download queue.
         */
        void downloadingInProgress(const int& count);

        /*!
         * Signal emitted when a resource download has finished and the current download queue is empty.
         */
        void downloadingFinished();

        /*!
         * Signal emitted when an image has been downloaded.
         * @param url The url that the image was downloaded from.
         * @param pixmap The image.
         */
        void imageDownloaded(const QUrl& url, const QPixmap& pixmap);

    private slots:
        /*!
         * Slot to ask user for proxy authentication details.
         * @param proxy The proxy details.
         * @param authenticator The authenticator to fill out (username/password).
         */
        void proxyAuthenticationRequired(const QNetworkProxy& proxy, QAuthenticator* authenticator);

        /*!
         * Slot to handle a download that has finished.
         * @param reply The reply that contains the downloaded data.
         */
        void downloadFinished(QNetworkReply* reply);

    private:
        //! Disable copy constructor.
        NetworkManager(const NetworkManager&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        NetworkManager& operator=(const NetworkManager&); /// @todo remove once MSVC supports default/delete syntax.

        /// Network access manager.
        QNetworkAccessManager m_nam;

        /// Downloading image queue.
        QMap<QNetworkReply*, QUrl> m_downloading_image;

        /// Mutex protecting downloading image queue.
        mutable QMutex m_mutex_downloading_image;
    };
}
