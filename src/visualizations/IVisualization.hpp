#pragma once
#include "../generators/IGenerator.hpp"
#include <SFML/Graphics.hpp>


class IVisualization {
public:
  virtual ~IVisualization() = default;

  // Process random numbers using the given generator
  virtual void processBatch(IGenerator *gen, const sf::Vector2u &windowSize,
                            int batchSize) = 0;

  // Draw the visualization onto the window
  virtual void render(sf::RenderTarget &target, const sf::Color &color) = 0;

  // Clear all the generated data
  virtual void clear() = 0;

  // Return performance information to display in UI
  virtual size_t getDataPointCount() const = 0;

  // Required batch size setting
  virtual int getRecommendedBatchSize() const = 0;

  virtual const char *name() const = 0;
};
