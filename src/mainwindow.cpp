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

#include "mainwindow.h"

#include "./ui_mainwindow.h"

#include <QSlider>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_ui(new Ui::MainWindow) {
  m_ui->setupUi(this);

  connect(m_ui->netEffect, &WNetEffect::countPaintedLines, this, [this](int count) {
    m_ui->lineCount->setText(QString::number(count));
  });

  connect(m_ui->netEffect, &WNetEffect::startMaxFadeDistanceChanged, this, [this](double value) {
    m_ui->startFadeDistance->setMaximum(value);
  });

  connect(
      m_ui->startFadeDistance,
      &QSlider::valueChanged,
      m_ui->netEffect,
      &WNetEffect::setStartFadeDistance
  );

  connect(
      &m_lineColor,
      &QColorDialog::currentColorChanged,
      m_ui->netEffect,
      &WNetEffect::setLineColor
  );

  connect(
      &m_pointColor,
      &QColorDialog::currentColorChanged,
      m_ui->netEffect,
      &WNetEffect::setPointColor
  );

  connect(&m_background, &QColorDialog::currentColorChanged, this, [this](QColor color) {
    QPalette curPalette = palette();
    curPalette.setColor(QPalette::Window, color);
    setPalette(curPalette);
  });

  connect(m_ui->butLineColor, &QPushButton::clicked, this, &MainWindow::butLineColor);
  connect(m_ui->butPointColor, &QPushButton::clicked, this, &MainWindow::butPointColor);
  connect(m_ui->butBackground, &QPushButton::clicked, this, &MainWindow::butBackground);
}

MainWindow::~MainWindow() {
  delete m_ui;
}

void MainWindow::butLineColor() {
  m_lineColor.setCurrentColor(m_ui->netEffect->lineColor());
  QColor lastColor = m_lineColor.currentColor();

  if (m_lineColor.exec() == QDialog::Rejected) {
    m_ui->netEffect->setLineColor(lastColor);
  }
}

void MainWindow::butPointColor() {
  m_pointColor.setCurrentColor(m_ui->netEffect->pointColor());
  QColor lastColor = m_pointColor.currentColor();

  if (m_pointColor.exec() == QDialog::Rejected) {
    m_ui->netEffect->setPointColor(lastColor);
  }
}

void MainWindow::butBackground() {
  QPalette curPalette = palette();
  m_background.setCurrentColor(curPalette.color(QPalette::Window));

  QColor lastColor = m_background.currentColor();

  if (m_background.exec() == QDialog::Rejected) {
    curPalette.setColor(QPalette::Window, lastColor);
    setPalette(curPalette);
  }
}

void MainWindow::showEvent(QShowEvent* event) {
  static bool init = false;

  if (!init) {
    m_ui->startFadeDistance->setValue(m_ui->startFadeDistance->maximum() * (1.0 / 5.0));
    init = true;
  }
}
