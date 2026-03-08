#pragma once
#include "IVisualization.hpp"
#include <vector>

/**
 * @brief Visualizes RNG data as a 1D probability histogram.
 *
 * Uses one bin per pixel column, so each generated value maps directly and
 * unambiguously to a single screen column. Automatically resizes and resets
 * when the window width changes.
 */
class HistogramVis : public IVisualization {
  std::vector<unsigned long long> histogram;
  unsigned long long max_val;
  sf::Vector2u lastWindowSize;

public:
  HistogramVis();

  void processBatch(IGenerator *gen, const sf::Vector2u &windowSize,
                    int batchSize) override;
  void render(sf::RenderTarget &target, const sf::Color &color) override;
  void clear() override;

  size_t getDataPointCount() const override;
  int getRecommendedBatchSize() const override;
  const char *name() const override;
};
