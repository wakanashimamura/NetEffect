// ================================================================================================
//
// Copyright (C) 2026 Wakana Shimamura
//
// This file is part of NetEffect.
//
// NetEffect is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// NetEffect is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with NetEffect. If not, see <https://www.gnu.org/licenses/>.
//
// Repository: https://github.com/wakanashimamura/NetEffect
//
// ================================================================================================

#include "wneteffect.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPalette>

WNetEffect::WNetEffect(QWidget* parent)
    : QWidget(parent) {
  m_minSpeed = 1;
  m_maxSpeed = 3;

  m_radius = 4;

  m_pointСolor.setRgb(130, 141, 220);
  m_lineСolor.setRgb(98, 108, 176);

  m_timer.start(33);
  connect(&m_timer, &QTimer::timeout, this, &WNetEffect::wupdate);

  setMouseTracking(true);
}

void WNetEffect::setStartFadeDistance(double startFadeDistance) {
  // Accepts a value in pixels. We square the value because the maximum value is half the hypotenuse squared.
  int startFadeDistanceSquared = pow(startFadeDistance, 2);
  if (startFadeDistanceSquared <= m_startMaxFadeDistance) {
    m_startFadeDistance = startFadeDistanceSquared;
  } else {
    m_startFadeDistance = m_startMaxFadeDistance;
  }
}

void WNetEffect::setPointColor(QColor pointColor) {
  m_pointСolor = pointColor;
}

void WNetEffect::setLineColor(QColor lineColor) {
  m_lineСolor = lineColor;
}

void WNetEffect::wupdate() {
  updatePosition();
  update();
}

void WNetEffect::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  QPen pen;

  QBrush brush;
  brush.setStyle(Qt::SolidPattern);

  pen.setColor(m_pointСolor);
  pen.setWidth(m_radius);
  painter.setPen(pen);

  QColor color = m_lineСolor;

  // Draw lines.
  int countline = 0;
  for (int i = 0; i < m_points.size(); i++) {
    // Draw lines between points.
    for (int j = i + 1; j < m_points.size(); j++) {
      // Calculate the distance between points.
      double distance = pointsDistance(m_points[i].x, m_points[i].y, m_points[j].x, m_points[j].y);

      if (distance <= m_startFadeDistance) {
        // Normalize the value inversely based on the distance and use it to determine the line's opacity and width.
        double normalizedDistance = (1.0 - (distance / m_startFadeDistance));

        color.setAlpha(255 * normalizedDistance);

        pen.setColor(color);
        pen.setWidth(m_radius * normalizedDistance + 1);
        painter.setPen(pen);

        painter.drawLine(m_points[i].x, m_points[i].y, m_points[j].x, m_points[j].y);
        countline++;
      }
    }

    if (!underMouse()) {
      continue;
    }

    // Draw lines to the mouse cursor.
    double distance = pointsDistance(m_points[i].x, m_points[i].y, m_mousePos.x(), m_mousePos.y());

    if (distance <= m_startFadeDistance) {
      double normalizedDistance = (1.0 - (distance / m_startFadeDistance));

      color.setAlpha(255 * normalizedDistance);

      pen.setColor(color);
      pen.setWidth(m_radius * normalizedDistance + 1);
      painter.setPen(pen);

      painter.drawLine(m_points[i].x, m_points[i].y, m_mousePos.x(), m_mousePos.y());
      countline++;
    }
  }

  emit countPaintedLines(countline);

  // Draw points.
  brush.setColor(m_pointСolor);
  painter.setBrush(brush);

  pen.setColor(m_pointСolor);
  painter.setPen(pen);

  for (int i = 0; i < m_points.size(); i++) {
    painter.drawEllipse(QPointF(m_points[i].x, m_points[i].y), m_radius, m_radius);
  }
}

void WNetEffect::mouseMoveEvent(QMouseEvent* event) {
  m_mousePos = event->position();
}

void WNetEffect::showEvent(QShowEvent* event) {
  QWidget::showEvent(event);

  if (m_isPointsCreated) {
    return;
  }

  // Randomly create points when the window is first displayed.

  double w = width();
  double h = height();
  for (int i = 0; i < k_initialCountPoints; ++i) {
    m_points.emplace_back(
        w * Random::GenerateDouble(),
        h * Random::GenerateDouble(),
        randSpeed(),
        randSpeed()
    );
  }

  m_startMaxFadeDistance = callStartMaxFadeDistance();

  emit startMaxFadeDistanceChanged(startMaxFadeDistance());

  m_isPointsCreated = true;
}

void WNetEffect::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    m_points.emplace_back(event->position().x(), event->position().y(), randSpeed(), randSpeed());
  }

  if (event->button() == Qt::RightButton) {
    if (m_points.size() > k_minCountPoint && underMouse()) {
      deleteNearestPointToCursor();
    }
  }
}

void WNetEffect::resizeEvent(QResizeEvent* event) {
  if (!m_isPointsCreated) {
    return;
  }

  // Handle points going outside the screen when the window is resized.

  for (int i = 0; i < m_points.size(); i++) {
    if (m_points[i].x - m_radius <= -(m_radius * 2)) {
      m_points[i].x = 0;
    }

    if (m_points[i].x + m_radius >= width() + m_radius * 2) {
      m_points[i].x = width() - 1;
    }

    if (m_points[i].y - m_radius <= -(m_radius * 2)) {
      m_points[i].y = 0;
    }

    if (m_points[i].y + m_radius >= height() + m_radius * 2) {
      m_points[i].y = height() - 1;
    }
  }

  m_startMaxFadeDistance = callStartMaxFadeDistance();
  emit startMaxFadeDistanceChanged(startMaxFadeDistance());
}

void WNetEffect::updatePosition() {
  for (int i = 0; i < m_points.size(); i++) {
    // Move the point according to its velocity.
    m_points[i].x += m_points[i].dx;
    m_points[i].y += m_points[i].dy;

    // Handle points going outside the screen.
    if (m_points[i].x - m_radius <= 0) {
      m_points[i].dx = abs(m_points[i].dx);
    }

    if (m_points[i].x + m_radius >= width()) {
      m_points[i].dx = -abs(m_points[i].dx);
    }

    if (m_points[i].y - m_radius <= 0) {
      m_points[i].dy = abs(m_points[i].dy);
    }

    if (m_points[i].y + m_radius >= height()) {
      m_points[i].dy = -abs(m_points[i].dy);
    }
  }
}

void WNetEffect::deleteNearestPointToCursor() {
  int index       = 0;
  int minDistance = m_startMaxFadeDistance;
  for (int i = 0; i < m_points.size(); i++) {
    double distance = pointsDistance(m_points[i].x, m_points[i].y, m_mousePos.x(), m_mousePos.y());

    if (distance < minDistance) {
      minDistance = distance;
      index       = i;
    }
  }

  m_points.erase(m_points.begin() + index);
}
