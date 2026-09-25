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

#pragma once

#include <QColor>
#include <QTimer>
#include <QWidget>

#include <vector>

class WNetEffect : public QWidget {
  Q_OBJECT

 public:
  WNetEffect(QWidget* parent = nullptr);
  double startMaxFadeDistance() { return sqrt(m_startMaxFadeDistance); }

  QColor pointColor() { return m_pointСolor; };
  QColor lineColor() { return m_lineСolor; };

 public slots:
  void setStartFadeDistance(double startFadeDistance);
  void setPointColor(QColor pointColor);
  void setLineColor(QColor lineColor);

  void wupdate();

 signals:
  void countPaintedLines(int count);
  void startMaxFadeDistanceChanged(double value);

 protected:
  virtual void paintEvent(QPaintEvent* event) override;
  virtual void mouseMoveEvent(QMouseEvent* event) override;
  virtual void showEvent(QShowEvent* event) override;
  virtual void mousePressEvent(QMouseEvent* event) override;
  virtual void resizeEvent(QResizeEvent* event) override;

 private:
  void updatePosition();

 private:
  struct MovingPoint {
    MovingPoint(double x_, double y_, double dx_, double dy_)
        : x(x_),
          y(y_),
          dx(dx_),
          dy(dy_) {}

    double x;
    double y;
    double dx;
    double dy;
  };

  static constexpr int k_initialCountPoints = 50;
  static constexpr int k_minCountPoint      = 5;

  QTimer m_timer;

  bool m_isPointsCreated = false;

  double m_startMaxFadeDistance;
  double m_startFadeDistance;

  double m_minSpeed;
  double m_maxSpeed;

  double m_radius;

  QPointF m_mousePos;

  std::vector<MovingPoint> m_points;

  QColor m_pointСolor;
  QColor m_lineСolor;
};
