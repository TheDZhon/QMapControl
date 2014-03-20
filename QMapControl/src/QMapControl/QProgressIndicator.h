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

#pragma once

// Qt includes.
#include <QtGui/QColor>
#include <QtWidgets/QWidget>

/*!
 * The QProgressIndicator class lets an application display a progress indicator to show that a lengthy task is under way.
 * Progress indicators are indeterminate and do nothing more than spin to show that the application is busy.
*/
class QProgressIndicator : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int delay READ animationDelay WRITE setAnimationDelay)
    Q_PROPERTY(bool displayedWhenStopped READ isDisplayedWhenStopped WRITE setDisplayedWhenStopped)
    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    //! Constructor.
    /*!
     * This construct a QProgressIndicator.
     * @param parent QWidget parent ownership.
     */
    explicit QProgressIndicator(QWidget* parent = 0);

    //! Copy constructor.
    ///QProgressIndicator(const QProgressIndicator&) = default; @todo re-add once MSVC supports default/delete syntax.

    //! Copy assignment.
    ///QProgressIndicator& operator=(const QProgressIndicator&) = default; @todo re-add once MSVC supports default/delete syntax.

    //! Destructor.
    ~QProgressIndicator() { } /// = default; @todo re-add once MSVC supports default/delete syntax.

    /*!
     * Returns the delay between animation steps.
     * @return The number of milliseconds between animation steps. By default, the animation delay is set to 40 milliseconds.
     * @sa setAnimationDelay
     */
    int animationDelay() const;

    /*!
     * Returns a Boolean value indicating whether the component is currently animated.
     * @return The animation state.
     * @sa startAnimation stopAnimation
     */
    bool isAnimated() const;

    /*!
     * Returns a Boolean value indicating whether the receiver shows itself even when it is not animating.
     * @return Return true if the progress indicator shows itself even when it is not animating. By default, it returns false.
     * @sa setDisplayedWhenStopped
     */
    bool isDisplayedWhenStopped() const;

    /*!
     * Returns the color of the component.
     * @return The color of the component.
     * @sa setColor
     */
    const QColor& color() const;

    /*!
     * Returns the recommended size for the widget.
     * @return The recommended size.
     */
    QSize sizeHint() const final;

    /*!
     * Returns the preferred height for this widget, given the width.
     * @param width The given width.
     * @return The preferred height.
     */
    int heightForWidth(int width) const final;

public slots:
    /*!
     * Starts the spin animation.
     * @sa stopAnimation isAnimated
     */
    void startAnimation();

    /*!
     * Stops the spin animation.
     * @sa startAnimation isAnimated
     */
    void stopAnimation();

    /*!
     * Sets the delay between animation steps.
     * Setting the delay to a value larger than 40 slows the animation, while setting the delay to a smaller value speeds it up.
     * @param delay The delay, in milliseconds.
     * @sa animationDelay
     */
    void setAnimationDelay(const int& delay);

    /*!
     * Sets whether the component hides itself when it is not animating.
     * @param state The animation state. Set false to hide the progress indicator when it is not animating; otherwise true.
     * @sa isDisplayedWhenStopped
     */
    void setDisplayedWhenStopped(const bool& state);

    /*!
     * Sets the color of the components to the given color.
     * @param color The color to set.
     * @sa color
     */
    void setColor(const QColor& color);

protected:
    /*!
     * Updates the progress indicator rotation angle and then schedules a repaint.
     * @param event The QTimerEvent.
     */
    void timerEvent(QTimerEvent* event) final;

    /*!
     * Paints the current scene.
     * @param event The QPaintEvent.
     */
    void paintEvent(QPaintEvent* event) final;

private:
    /// The current rotation angle of the progress indicator.
    int m_angle;

    /// The current timer identifier.
    int m_timer_identifier;

    /// The delay between animations.
    int m_delay;

    /// Whether the progress indicator is always visible.
    bool m_always_visible;

    /// The brush color of the progress capsule indicators.
    QColor m_color;
};
