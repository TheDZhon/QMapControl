/*
*
* This file is part of QProgressIndicator,
* an open-source recent files menu widget
*
* Copyright (C) 2009 - 2010 Morgan Leborgne
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
* along with QProgressIndicator. If not, see <http://www.gnu.org/licenses/>.
*
*/
#include "QProgressIndicator.h"

#include <QPainter>

QProgressIndicator::QProgressIndicator(QWidget* parent)
    : QWidget(parent),
      m_angle(0),
      m_timer_identifier(-1),
      m_delay(40),
      m_always_visible(false),
      m_color(Qt::black)
{
    // Set the default size and focus policies.
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFocusPolicy(Qt::NoFocus);
}

int QProgressIndicator::animationDelay() const
{
    // Return the delay (milliseconds).
    return m_delay;
}

bool QProgressIndicator::isAnimated() const
{
    // Return if the timer is currently active.
    return (m_timer_identifier != -1);
}

bool QProgressIndicator::isDisplayedWhenStopped() const
{
    // Return whether the progress indicator is displayed when stopped.
    return m_always_visible;
}

const QColor & QProgressIndicator::color() const
{
    // Return the color.
    return m_color;
}

QSize QProgressIndicator::sizeHint() const
{
    // Return the recommended size.
    return QSize(20, 20);
}

int QProgressIndicator::heightForWidth(int width) const
{
    // Return the width (as the progress indicator is looks best as a square).
    return width;
}

void QProgressIndicator::startAnimation()
{
    // Check that the timer hasn't already started.
    if(m_timer_identifier == -1)
    {
        // Start the timer with the delay.
        m_timer_identifier = QWidget::startTimer(m_delay);
    }
}

void QProgressIndicator::stopAnimation()
{
    // Check if the timer is running.
    if(m_timer_identifier != -1)
    {
        // Kill the timer.
        QWidget::killTimer(m_timer_identifier);

        // Reset the timer identifier.
        m_timer_identifier = -1;

        // Schedule a repaint.
        QWidget::update();
    }
}

void QProgressIndicator::setAnimationDelay(const int& delay)
{
    // Check if the timer is running.
    if(m_timer_identifier != -1)
    {
        // Kill the timer.
        QWidget::killTimer(m_timer_identifier);
    }

    // Set the new delay setting.
    m_delay = delay;

    // Was the timer previously running.
    if(m_timer_identifier != -1)
    {
        // Restart the timer with the new delay setting.
        m_timer_identifier = QWidget::startTimer(m_delay);
    }
}

void QProgressIndicator::setDisplayedWhenStopped(const bool& state)
{
    // Set the new state.
    m_always_visible = state;

    // Schedule a repaint.
    QWidget::update();
}

void QProgressIndicator::setColor(const QColor& color)
{
    // Set the new color.
    m_color = color;

    // Schedule a repaint.
    QWidget::update();
}

void QProgressIndicator::timerEvent(QTimerEvent* /*event*/)
{
    // Update the current rotation angle.
    m_angle = (m_angle + 30) % 360;

    // Schedule a repaint.
    QWidget::update();
}

void QProgressIndicator::paintEvent(QPaintEvent* /*event*/)
{
    // Check if we should alawys display, or we are currently animating.
    if(m_always_visible || isAnimated())
    {
        // Create a painter for this QWidget.
        QPainter painter(this);

        // Ensure antialiasing is enabled.
        painter.setRenderHint(QPainter::Antialiasing, true);

        // Calculate the smallest dimension of the QWidget.
        const int width = qMin(this->width(), this->height());

        // Calculate the inner and outer radius'.
        const int radius_outer = (width - 1.0) * 0.5;
        const int radius_inner = (width - 1.0) * 0.5 * 0.38;

        // Calculate the capsule dimensions.
        const int capsule_height = radius_outer - radius_inner;
        const int capsule_width  = (width > 32 ) ? capsule_height * 0.23 : capsule_height * 0.35;
        const int capsule_radius = capsule_width / 2.0;

        // Loop and create the progress capsule indicators.
        for(int i = 0; i < 12; ++i)
        {
            // Set the painter's brush color.
            QColor color = m_color;
            color.setAlphaF(1.0f - (i/12.0f));
            painter.setPen(Qt::NoPen);
            painter.setBrush(color);

            // Save the current painter's state.
            painter.save();

            // Translate and rotate the painter as required.
            painter.translate(rect().center());
            painter.rotate(m_angle - i * 30.0f);

            // Draw the progress capsule indicator.
            painter.drawRoundedRect(-capsule_width * 0.5, -(radius_inner + capsule_height), capsule_width, capsule_height, capsule_radius, capsule_radius);

            // Restore the painter's previous state.
            painter.restore();
        }
    }
}
