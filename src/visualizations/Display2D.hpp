#pragma once
#include "IVisualization.hpp"

/**
 * @brief Visualizes RNG data as a 2D scatter plot.
 *
 * Maps pairs of sequential random numbers (X, Y) to pixel coordinates on the
 * screen. Useful for detecting spatial patterns, clustering, or striping in the
 * generator.
 */
class Display2DVis : public IVisualization {
  sf::VertexArray points;

public:
  Display2DVis();

  void processBatch(IGenerator *gen, const sf::Vector2u &windowSize,
                    int batchSize) override;
  void render(sf::RenderTarget &target, const sf::Color &color) override;
  void clear() override;

  size_t getDataPointCount() const override;
  int getRecommendedBatchSize() const override;
  const char *name() const override;
};
