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

#include "ImageManager.h"

// Qt includes.
#include <QDateTime>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDateTime>
#include <QtGui/QPainter>

// Local includes.
#include "Projection.h"

namespace qmapcontrol
{
    namespace
    {
        /// Singleton instance of Image Manager.
        std::unique_ptr<ImageManager> m_instance = nullptr;
    }

    ImageManager& ImageManager::get()
    {
        // Does the singleton instance exist?
        if(m_instance == nullptr)
        {
            // Create a default instance (256px tiles).
            m_instance.reset(new ImageManager(256));
        }

        // Return the reference to the instance object.
        return *(m_instance.get());
    }

    void ImageManager::destory()
    {
        // Ensure the singleton instance is destroyed.
        m_instance.reset(nullptr);
    }

    ImageManager::ImageManager(const int& tile_size_px, QObject* parent)
        : QObject(parent),
          m_tile_size_px(tile_size_px),
          m_pixmap_loading(),
          m_persistent_cache(false),
          m_persistent_cache_expiry(0)
    {
        // Setup a loading pixmap.
        setupLoadingPixmap();

        // Connect signal/slot for image downloads.
        QObject::connect(this, &ImageManager::downloadImage, &m_nm, &NetworkManager::downloadImage);
        QObject::connect(&m_nm, &NetworkManager::imageDownloaded, this, &ImageManager::imageDownloaded);
        QObject::connect(&m_nm, &NetworkManager::downloadingInProgress, this, &ImageManager::downloadingInProgress);
        QObject::connect(&m_nm, &NetworkManager::downloadingFinished, this, &ImageManager::downloadingFinished);
    }

    int ImageManager::tileSizePx() const
    {
        // Return the tiles size in pixels.
        return m_tile_size_px;
    }

    void ImageManager::setTileSizePx(const int& tile_size_px)
    {
        // Set the new tile size.
        m_tile_size_px = tile_size_px;

        // Create a new loading pixmap.
        setupLoadingPixmap();
    }

    void ImageManager::setProxy(const QNetworkProxy& proxy)
    {
        // Set the proxy on the network manager.
        m_nm.setProxy(proxy);
    }

    bool ImageManager::enablePersistentCache(const std::chrono::minutes& expiry, const QDir& path)
    {
        // Ensure that the path exists (still returns true when path already exists.
        bool success = path.mkpath(path.absolutePath());

        // If the path does exist, enable persistent cache.
        if(success)
        {
            // Set the persistent cache directory path.
            m_persistent_cache_directory = path;

            // Set the persistent cache expiry.
            /// @TODO should each map adapter should provide their own specific exipry?
            m_persistent_cache_expiry = expiry;

            // Enable persistent caching.
            m_persistent_cache = true;
        }
        else
        {
            // Log error.
            qDebug() << "Unable to create directory for persistent cache '" << path.absolutePath() << "'";
        }

        // Return success.
        return success;
    }

    void ImageManager::abortLoading()
    {
        // Abort any remaing network manager downloads.
        m_nm.abortDownloads();
    }

    int ImageManager::loadQueueSize() const
    {
        // Return the network manager downloading queue size.
        return m_nm.downloadQueueSize();
    }

    QPixmap ImageManager::getImage(const QUrl& url)
    {
        // Holding resource for image to be loaded into.
        QPixmap return_pixmap(m_pixmap_loading);

        // Is the image already been downloaded by the network manager?
        if(m_nm.isDownloading(url) == false)
        {
            // Is the image in our volatile "in-memory" cache?
            const auto find_itr = m_pixmap_cache.find(md5hex(url));
            if(find_itr != m_pixmap_cache.end())
            {
                // Set the return image to the "in-memory" cached version.
                return_pixmap = find_itr->second;
            }
            // Is the persistent cache enabled?
            else if(m_persistent_cache)
            {
                // Does the image exist in the persistent cache.
                if(persistentCacheFind(url, return_pixmap))
                {
                    // Add the image to the volatile cache.
                    m_pixmap_cache[md5hex(url)] = return_pixmap;
                }
                else
                {
                    // Emit that we need to download the image using the network manager.
                    emit downloadImage(url);
                }
            }
            else
            {
                // Emit that we need to download the image using the network manager.
                emit downloadImage(url);
            }
        }

        // Default return the image.
        return return_pixmap;
    }

    QPixmap ImageManager::prefetchImage(const QUrl& url)
    {
        // Add the url to the prefetch list.
        m_prefetch_urls.append(url);

        // Return the image for the url.
        return getImage(url);
    }

    void ImageManager::setLoadingPixmap(const QPixmap &pixmap)
    {
        m_pixmap_loading = pixmap;
    }

    void ImageManager::imageDownloaded(const QUrl& url, const QPixmap& pixmap)
    {
#ifdef QMAP_DEBUG
        qDebug() << "ImageManager::imageDownloaded '" << url << "'";
#endif

        // Add it to the pixmap cache.
        m_pixmap_cache[md5hex(url)] = pixmap;

        // Do we have the persistent cache enabled?
        if(m_persistent_cache)
        {
            // Add the pixmap to the persistent cache.
            persistentCacheInsert(url, pixmap);
        }

        // Is this a prefetch request?
        if(m_prefetch_urls.contains(url))
        {
            // Remove the url from the prefetch list.
            m_prefetch_urls.removeAt(m_prefetch_urls.indexOf(url));
        }
        else
        {
            // Let the world know we have received an updated image.
            emit imageUpdated(url);
        }
    }

    void ImageManager::setupLoadingPixmap()
    {
        // Create a new pixmap.
        m_pixmap_loading = QPixmap(m_tile_size_px, m_tile_size_px);

        // Make is transparent.
        m_pixmap_loading.fill(Qt::transparent);

        // Add a pattern.
        QPainter painter(&m_pixmap_loading);
        QBrush brush(Qt::lightGray, Qt::Dense5Pattern);
        painter.fillRect(m_pixmap_loading.rect(), brush);

        // Add "LOADING..." text.
        painter.setPen(Qt::black);
        painter.drawText(m_pixmap_loading.rect(), Qt::AlignCenter, "LOADING...");
    }

    QString ImageManager::md5hex(const QUrl& url)
    {
        // Return the md5 hex value of the given url at a specific projection and tile size.
        return QString(QCryptographicHash::hash((url.toString() + QString::number(projection::get().epsg()) + QString::number(m_tile_size_px)).toUtf8(), QCryptographicHash::Md5).toHex());
    }

    QString ImageManager::persistentCacheFilename(const QUrl& url)
    {
        // Return the persistent file path for the given url.
        return m_persistent_cache_directory.absolutePath() + QDir::separator() + md5hex(url);
    }

    bool ImageManager::persistentCacheFind(const QUrl& url, QPixmap& return_pixmap)
    {
        // Track our success.
        bool success(false);

        // The file for the given url from the persistent cache.
        QFile file(persistentCacheFilename(url));

        // Does the file exist?
        if(file.exists())
        {
            // Get the file information.
            QFileInfo file_info(file);

            // Is the persistent cache expiry set, and if so is the file older than the expiry time
            // allowed?
            if(m_persistent_cache_expiry.count() > 0
               && file_info.lastModified().msecsTo(QDateTime::currentDateTime()) > std::chrono::duration_cast<std::chrono::milliseconds>(m_persistent_cache_expiry).count())
            {
                // The file is too old, remove it.
                m_persistent_cache_directory.remove(file.fileName());

                // Log removing the file.
#ifdef QMAP_DEBUG
                qDebug() << "Removing '" << file.fileName() << "' from persistent cache for url '" << url << "'";
#endif
            }
            else
            {
                // Try to load the file into the pixmap, store the success result.
                success = return_pixmap.load(persistentCacheFilename(url));
            }
        }

        // Return success.
        return success;
    }

    bool ImageManager::persistentCacheInsert(const QUrl& url, const QPixmap& pixmap)
    {
        // Return the result of saving the pixmap to the persistent cache.
        return pixmap.save(persistentCacheFilename(url), "PNG");
    }
}
