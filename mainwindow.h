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

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;

 private:
  Ui::MainWindow* m_ui;
};
