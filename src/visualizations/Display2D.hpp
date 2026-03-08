#pragma once
#include "IVisualization.hpp"
#include <SFML/Graphics.hpp>

/**
 * @brief Visualizes RNG data as a 2D scatter plot with a GPU fade effect.
 *
 * Maps pairs of sequential random numbers (X, Y) to pixel coordinates on the
 * screen. Points are rendered into an off-screen sf::RenderTexture; each frame
 * a semi-transparent black overlay is drawn on top to gradually fade old
 * points, producing a smooth trailing effect entirely on the GPU.
 */
class Display2DVis : public IVisualization {
  sf::RenderTexture canvas;
  sf::VertexArray batch;
  sf::RectangleShape fadeOverlay;
  sf::Vector2u lastWindowSize;
  size_t totalPoints;
  int fade_alpha = 8;

  /// Initializes or resizes the internal canvas to match the window.
  void ensureCanvas(const sf::Vector2u &windowSize);

public:
  Display2DVis();

  void processBatch(IGenerator *gen, const sf::Vector2u &windowSize,
                    int batchSize) override;
  void render(sf::RenderTarget &target, const sf::Color &color) override;
  void clear() override;

  size_t getDataPointCount() const override;
  int getRecommendedBatchSize() const override;
  const char *name() const override;
  void setFadeAlpha(int alpha) override;
};
