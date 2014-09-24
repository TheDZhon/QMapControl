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
#include <QtCore/QDir>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QUrl>
#include <QtGui/QPixmap>
#include <QtNetwork/QNetworkProxy>

// STL includes.
#include <chrono>
#include <map>
#include <memory>

// Local includes.
#include "qmapcontrol_global.h"
#include "NetworkManager.h"

/*!
 * @author Kai Winter <kaiwinter@gmx.de>
 * @author Chris Stylianou <chris5287@gmail.com>
 */
namespace qmapcontrol
{
    class QMAPCONTROL_EXPORT ImageManager : public QObject
    {
        Q_OBJECT
    public:
        /*!
         * Get the singleton instance of the Image Manager.
         * @return the singleton instance.
         */
        static ImageManager& get();

        //! Destroys the singleton instance of Image Manager.
        static void destory();

    public:
        //! Disable copy constructor.
        /// ImageManager(const ImageManager&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ///ImageManager& operator=(const ImageManager&) = delete; @todo re-add once MSVC supports default/delete syntax.

        //! Destructor.
        ~ImageManager() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

        /*!
         * Fetch the tile size in pixels.
         * @return the tile size in pixels.
         */
        int tileSizePx() const;

        /*!
         * Set the new tile size (and resets any resources as required).
         * @param tile_size_px The tile size in pixels to set.
         */
        void setTileSizePx(const int& tile_size_px);

        /*!
         * Set the network proxy to use.
         * @param proxy The network proxy to use.
         */
        void setProxy(const QNetworkProxy& proxy);

        /*!
         * Enables the persistent cache, specifying the directory and expiry timeout.
         * @param path The path where the images should be stored.
         * @param expiry The max age (in minutes) of an image before its removed and a new one is requested (0 to keep forever).
         * @return whether the persistent cache was enabled.
         */
        bool enablePersistentCache(const std::chrono::minutes& expiry, const QDir& path);

        /*!
         * Aborts all current loading threads.
         * This is useful when changing the zoom-factor.
         */
        void abortLoading();

        /*!
         * Number of images pending in the load queue.
         * @return the number of images pending in the load queue.
         */
        int loadQueueSize() const;

        /*!
         * If this component doesn't have the image a network query gets started to load it.
         * Fetch the requested image either from an in-memory cache or persistent file cache (if
         * enabled).
         * If the image does not exist, then it is fetched using a network manager and a "loading"
         * placeholder pixmap is returned. Once the image has been downloaded, the image manager
         * will emit "imageReceived" to inform that the image is now ready.
         * @param url The image url to fetch.
         * @return the pixmap of the image.
         */
        QPixmap getImage(const QUrl& url);

        /*!
         * Fetches the requested image using the getImage function, which has been deemed
         * "offscreen".
         * However, if the image need to be fetched from the network, the "imageReceived" will be
         * emitted on particular hardware platforms only (Eg: mobile platforms do not receive the
         * "imageReceived" emission.
         * @param url The image url to fetch.
         * @return the pixmap of the image.
         */
        QPixmap prefetchImage(const QUrl& url);

        /*!
         * \brief setLoadingPixmap sets the pixmap displayed when a tile is not yet loaded
         * \param pixmap the pixmap to display
         */
        void setLoadingPixmap (const QPixmap &pixmap);

    signals:
        /*!
         * Signal emitted to schedule an image resource to be downloaded.
         * @param url The image url to download.
         */
        void downloadImage(const QUrl& url);

        /*!
         * Signal emitted when a new image has been queued for download.
         * @param count The current size of the download queue.
         */
        void downloadingInProgress(const int& count);

        /*!
         * Signal emitted when a image download has finished and the download queue is empty.
         */
        void downloadingFinished();

        /*!
         * Signal emitted when an image has been downloaded by the network manager.
         * @param url The url that the image was downloaded from.
         */
        void imageUpdated(const QUrl& url);

    private slots:
        /*!
         * Slot to handle an image that has been downloaded.
         * @param url The url that the image was downloaded from.
         * @param pixmap The image.
         */
        void imageDownloaded(const QUrl& url, const QPixmap& pixmap);

    private:
        //! Constructor.
        /*!
         * This construct a Image Manager.
         * @param tile_size_px The tile size in pixels.
         * @param parent QObject parent ownership.
         */
        ImageManager(const int& tile_size_px, QObject* parent = 0);

        //! Disable copy constructor.
        ImageManager(const ImageManager&); /// @todo remove once MSVC supports default/delete syntax.

        //! Disable copy assignment.
        ImageManager& operator=(const ImageManager&); /// @todo remove once MSVC supports default/delete syntax.

        /*!
         * Create a loading pixmap for use.
         */
        void setupLoadingPixmap();

        /*!
         * Generate a md5 hex for the given url.
         * @param url The url to generate a md5 hex for.
         * @return the md5 hex of the url.
         */
        QString md5hex(const QUrl& url);

        /*!
         * Generate the persistent file path for the given url.
         * @param url The url to generate a file path for.
         * @return the file path for the url.
         */
        QString persistentCacheFilename(const QUrl& url);

        /*!
         * Finds and loads the requested image if is exists in the persistent cache.
         * @param url The image url to fetch.
         * @param return_pixmap The pixmap of the image to be populated.
         * @return whether the image was actually found and loaded.
         */
        bool persistentCacheFind(const QUrl& url, QPixmap& return_pixmap);

        /*!
         * Inserts the image into the persistent cache.
         * @param url The image url to insert.
         * @param pixmap The pixmap of the image to insert.
         * @return whether the image was actually inserted.
         */
        bool persistentCacheInsert(const QUrl& url, const QPixmap& pixmap);

    private:
        /// Network manager.
        NetworkManager m_nm;

        /// Cache of pixmaps already loaded.
        std::map<QString, QPixmap> m_pixmap_cache;

        /// The tile size in pixels.
        int m_tile_size_px;

        /// Pixmap of an empty image with "LOADING..." text.
        QPixmap m_pixmap_loading;

        /// A list of image urls being prefetched.
        QList<QUrl> m_prefetch_urls;

        /// Whether persistent caching is enabled.
        bool m_persistent_cache;

        /// The persistent cache's storage directory.
        QDir m_persistent_cache_directory;

        /// The persistent cache's image expiry.
        std::chrono::minutes m_persistent_cache_expiry;
    };
}
