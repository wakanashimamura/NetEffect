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

#include <random>

WNetEffect::WNetEffect(QWidget* parent)
    : QWidget(parent) {
  m_minSpeed = 1;
  m_maxSpeed = 3;

  m_radius = 4;

  m_pointСolor.setRgb(100, 100, 255);
  m_lineСolor.setRgb(30, 33, 54);

  m_timer.start(33);
  connect(&m_timer, &QTimer::timeout, this, &WNetEffect::wupdate);

  setMouseTracking(true);
}

void WNetEffect::setStartFadeDistance(double startFadeDistance) {
  if (startFadeDistance <= m_startMaxFadeDistance) {
    m_startFadeDistance = pow(startFadeDistance, 2);
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

  int countline = 0;
  for (int i = 0; i < m_points.size(); i++) {
    for (int j = 0; j < m_points.size(); j++) {
      if (i == j) {
        continue;
      }

      double distance =
          std::pow(m_points[i].x - m_points[j].x, 2) + std::pow(m_points[i].y - m_points[j].y, 2);

      if (distance <= m_startFadeDistance) {
        color.setAlpha(255 * (1 - (distance / m_startFadeDistance)));
        pen.setColor(color);
        pen.setWidth(m_radius);
        painter.setPen(pen);

        painter.drawLine(m_points[i].x, m_points[i].y, m_points[j].x, m_points[j].y);
        countline++;
      }
    }

    if (underMouse()) {
      double distance =
          std::pow(m_points[i].x - m_mousePos.x(), 2) + std::pow(m_points[i].y - m_mousePos.y(), 2);

      if (distance <= m_startFadeDistance) {
        color.setAlpha(255 * (1 - (distance / m_startFadeDistance)));
        pen.setColor(color);
        pen.setWidth(m_radius / 2);
        painter.setPen(pen);

        painter.drawLine(m_points[i].x, m_points[i].y, m_mousePos.x(), m_mousePos.y());
        countline++;
      }
    }
  }

  emit countPaintedLines(countline);

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

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_real_distribution<double> dis(0, 1);
  std::uniform_real_distribution<double> disSpeed(-m_maxSpeed, m_maxSpeed);

  auto randSpeed = [this, &gen, &disSpeed]() {
    for (;;) {
      double speed = disSpeed(gen);

      if (speed > m_minSpeed || speed < -m_minSpeed) {
        return speed;
      }
    }
  };

  double w = width();
  double h = height();
  for (int i = 0; i < k_initialCountPoints; ++i) {
    m_points.emplace_back(w * dis(gen), h * dis(gen), randSpeed(), randSpeed());
  }

  m_startMaxFadeDistance = std::pow(std::sqrt(std::pow(width(), 2) + std::pow(height(), 2)) / 2, 2);

  emit startMaxFadeDistanceChanged(startMaxFadeDistance());

  m_isPointsCreated = true;
}

void WNetEffect::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> disSpeed(-m_maxSpeed, m_maxSpeed);

    auto randSpeed = [this, &gen, &disSpeed]() {
      for (;;) {
        double speed = disSpeed(gen);

        if (speed > m_minSpeed || speed < -m_minSpeed) {
          return speed;
        }
      }
    };

    m_points.emplace_back(event->position().x(), event->position().y(), randSpeed(), randSpeed());
  }

  if (event->button() == Qt::RightButton) {
    if (m_points.size() > k_minCountPoint && underMouse()) {
      int index       = 0;
      int minDistance = m_startMaxFadeDistance;
      for (int i = 0; i < m_points.size(); i++) {
        double distance = std::pow(m_points[i].x - m_mousePos.x(), 2) +
                          std::pow(m_points[i].y - m_mousePos.y(), 2);

        if (distance < minDistance) {
          minDistance = distance;
          index       = i;
        }
      }

      m_points.erase(m_points.begin() + index);
    }
  }
}

void WNetEffect::resizeEvent(QResizeEvent* event) {
  if (!m_isPointsCreated) {
    return;
  }

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

  m_startMaxFadeDistance = std::pow(std::sqrt(std::pow(width(), 2) + std::pow(height(), 2)) / 2, 2);
  emit startMaxFadeDistanceChanged(startMaxFadeDistance());
}

void WNetEffect::updatePosition() {
  for (int i = 0; i < m_points.size(); i++) {
    m_points[i].x += m_points[i].dx;
    m_points[i].y += m_points[i].dy;

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
