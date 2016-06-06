#include <random>

#include "random.hpp"

namespace AapeliBlox {
  RandomNumberGenerator::RandomNumberGenerator (int seed, int blockTowerWidth) {
    currentSeed = seed;

    // Seed the Mersenne Twister with a real random number
    rMersenneTwister = std::mt19937(currentSeed);

    // Initialise different random number distributions
    // When creating an undo map, probability of placing block
    rCreateBlock = std::bernoulli_distribution(0.6);

    // If for some reason, we create a row with all blocks, which one to remove?
    rBlockDestroy = std::uniform_int_distribution<int>(0, blockTowerWidth - 1);

    // Randomly get the next block
    rBlock = std::uniform_int_distribution<int>(0, 6);

    // Randomly rotate the next block
    rRotation = std::uniform_int_distribution<int>(0, 3);
  }
  void RandomNumberGenerator::reSeed (int seed) {
    currentSeed = seed;
    rMersenneTwister = std::mt19937(currentSeed);
  }
  bool RandomNumberGenerator::createBlock (void) {
    return rCreateBlock(rMersenneTwister);
  }
  int RandomNumberGenerator::blockDestroy (void) {
    return rBlockDestroy(rMersenneTwister);
  }
  int RandomNumberGenerator::block (void) {
    return rBlock(rMersenneTwister);
  }
  int RandomNumberGenerator::rotation (void) {
    return rRotation(rMersenneTwister);
  }
  int RandomNumberGenerator::getSeed (void) {
    return currentSeed;
  }
}
