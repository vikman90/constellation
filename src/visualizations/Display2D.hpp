#pragma once
#include "IVisualization.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

/**
 * @brief Visualizes RNG data as a 2D scatter plot with a GPU fade effect.
 *
 * Maps pairs of sequential random numbers (X, Y) to pixel coordinates on the
 * screen. Points are rendered into an off-screen sf::RenderTexture; each frame
 * a semi-transparent black overlay is drawn on top to gradually fade old
 * points, producing a smooth trailing effect entirely on the GPU.
 *
 * When the window is resized, the existing canvas content is scaled and
 * preserved into the new canvas, avoiding a blank-screen reset.
 */
class Display2DVis : public IVisualization {
  std::unique_ptr<sf::RenderTexture> canvas;
  sf::VertexArray batch;
  sf::RectangleShape fadeOverlay;
  sf::Vector2u lastWindowSize;
  size_t totalPoints;
  int fade_alpha;

  /**
   * @brief Ensures the internal canvas matches the given window size.
   *
   * If the window has been resized, creates a new canvas and blits the
   * previous content scaled to fit, preserving the visual state.
   *
   * @param windowSize Target canvas dimensions.
   */
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
