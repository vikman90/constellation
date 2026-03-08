#pragma once
#include "../generators/IGenerator.hpp"
#include <SFML/Graphics.hpp>

/**
 * @brief Interface for drawing random number distributions.
 *
 * Defines the contract for taking data from a generator and rendering it
 * to an SFML RenderTarget (window).
 */
class IVisualization {
public:
  virtual ~IVisualization() = default;

  /**
   * @brief Processes a batch of random numbers and stores them internally.
   *
   * @param gen Pointer to the generator to pull numbers from.
   * @param windowSize The dimensions of the target rendering window.
   * @param batchSize The number of random values to pull in this frame.
   */
  virtual void processBatch(IGenerator *gen, const sf::Vector2u &windowSize,
                            int batchSize) = 0;

  /**
   * @brief Draws the processed data onto the target window.
   *
   * @param target The SFML render target (usually the main window).
   * @param color The user-selected color to tint the visualization.
   */
  virtual void render(sf::RenderTarget &target, const sf::Color &color) = 0;

  /**
   * @brief Clears all internal generated geometry and data.
   */
  virtual void clear() = 0;

  /**
   * @brief Gets the total number of mapped data points currently stored.
   *
   * @return The number of generated data points.
   */
  virtual size_t getDataPointCount() const = 0;

  /**
   * @brief Provides a generic batch size that works optimally for this viz.
   *
   * @return Recommended number of points to generate per frame.
   */
  virtual int getRecommendedBatchSize() const = 0;

  /**
   * @brief Gets the human-readable name of the visualization type.
   *
   * @return A null-terminated string containing the name.
   */
  virtual const char *name() const = 0;

  /**
   * @brief Sets the fade alpha for visualizations that support it.
   *
   * Default implementation is a no-op. Override in visualizations that
   * support a GPU-side fade/trail effect, such as Display2DVis.
   *
   * @param alpha Opacity subtracted each frame (0 = no fade, 255 = instant
   * clear).
   */
  virtual void setFadeAlpha(int alpha) { (void)alpha; }
};
