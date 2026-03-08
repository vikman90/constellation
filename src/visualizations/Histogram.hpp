#pragma once
#include "IVisualization.hpp"
#include <vector>

/**
 * @brief Visualizes RNG data as a 1D probability histogram.
 *
 * Distributes single random numbers into an array of vertical bins, dynamically
 * scaling the highest bin to fit the window height.
 */
class HistogramVis : public IVisualization {
  const int BINS = 1000;
  std::vector<unsigned long long> histogram;
  unsigned long long max_val;

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
