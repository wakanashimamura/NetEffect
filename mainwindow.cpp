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

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_ui(new Ui::MainWindow) {
  m_ui->setupUi(this);
}

MainWindow::~MainWindow() {
  delete m_ui;
}
