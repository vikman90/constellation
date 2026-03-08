#pragma once
#include "IVisualization.hpp"

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
