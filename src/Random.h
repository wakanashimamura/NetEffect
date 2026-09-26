// ================================================================================================
//
// ConsoleBattleShip - classic game of “Battleship” implemented in the console.
//
// Copyright (C) 2024 Wakana Shimamura
//
// This file is part of ConsoleBattleShip.
//
// ConsoleBattleShip is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// ConsoleBattleShip is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with ConsoleBattleShip. If not, see <https://www.gnu.org/licenses/>.
//
// Repository: https://github.com/wakanashimamura/ConsoleBattleShip
//
// ================================================================================================

#pragma once

#include <random>

class Random {
 public:
  Random() = delete;

  static int GenerateInt(int min, int max);
  static size_t GenerateSize_t(size_t max);
  static double GenerateDouble(double min, double max);
  static double GenerateDouble();


  template <typename It>
  static void Shuffle(It begin, It end);

 private:
  static std::random_device randomDevice;
  static std::mt19937_64 generator;
};

template <typename It>
inline void Random::Shuffle(It begin, It end) {
  std::shuffle(begin, end, generator);
}