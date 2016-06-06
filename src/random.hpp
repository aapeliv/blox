#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

namespace AapeliBlox {
  class RandomNumberGenerator {
    std::mt19937 rMersenneTwister;
    std::bernoulli_distribution rCreateBlock;
    std::uniform_int_distribution<int> rBlockDestroy;
    std::uniform_int_distribution<int> rBlock;
    std::uniform_int_distribution<int> rRotation;
    int currentSeed;
  public:
    RandomNumberGenerator (int seed, int blockTowerWidth = 1);
    void reSeed (int seed);
    bool createBlock (void);
    int blockDestroy (void);
    int block (void);
    int rotation (void);
    int getSeed (void);
  };
}

#endif //RANDOM_HPP
