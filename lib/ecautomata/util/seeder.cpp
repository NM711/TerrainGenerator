#include "./seeder.hpp"

int Seeder::setRandomSeed() {
  int random = std::random_device{}();
  this->setSeed(random);
  return random;
};

void Seeder::setSeed(int seed) {
  this->mt.seed(seed);
};

// From the internal state of the PRNG, return a value applying the given distribution function.

int Seeder::getDistributionOutput(std::uniform_int_distribution<int> &distribution) {
  return distribution(this->mt);
};
