#include "generators/GeneratorFactory.hpp"
#include <gtest/gtest.h>


TEST(FactoryTests, CreatesValidGenerators) {
  auto gens = GeneratorFactory::createAllAvailable();

  // We expect at least the cross-platform generators to be present
  EXPECT_GE(gens.size(), 4);

  // Test that all returned pointers are valid and have names
  for (const auto &gen : gens) {
    ASSERT_NE(gen, nullptr);
    ASSERT_NE(gen->name(), nullptr);
    EXPECT_GT(strlen(gen->name()), 0);

    // Ensure a single next() call doesn't crash
    double val = gen->next();
    (void)val;
  }
}
