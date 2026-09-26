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
// Repository: https://github.com/wakanashimamura/console_sea_battle
//
// ================================================================================================

#include "Random.h"

std::random_device Random::randomDevice;
std::mt19937_64 Random::generator(Random::randomDevice());

int Random::GenerateInt(int min, int max) {
  if (min > max) {
    std::swap(min, max);
  }

  std::uniform_int_distribution<int> distribution(min, max);

  return distribution(generator);
}

size_t Random::GenerateSize_t(size_t max) {
  if (max < 0) {
    return 0;
  }

  std::uniform_int_distribution<size_t> distribution(0, max);

  return distribution(generator);
}

double Random::GenerateDouble(double min, double max) {
  if (min > max) {
    std::swap(min, max);
  }

  std::uniform_real_distribution<double> distribution(
      min,
      std::nextafter(max, std::numeric_limits<double>::max())
  );

  return distribution(generator);
}

double Random::GenerateDouble() {
  static std::uniform_real_distribution<double> distribution(0, 1);
  return distribution(generator);
}
