#ifndef ECAUTOMATA_UTIL_SEEDER_HPP
#define ECAUTOMATA_UTIL_SEEDER_HPP

#include <random>

class Seeder {
  public:
    int setRandomSeed();
    void setSeed(int seed);
    int getDistributionOutput(std::uniform_int_distribution<int> &distribution);

  private:
    std::mt19937 mt;
};

#endif
