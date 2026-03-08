#pragma once
#include "generators/IGenerator.hpp"
#include "visualizations/IVisualization.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

/**
 * @brief Handles the rendering of the Dear ImGui control panel.
 *
 * Separates UI layout and state management from the core Application logic.
 * Keeps track of user selections and configuration values.
 */
class GuiPanel {
  int current_gen_idx = 0;
  int current_viz_idx = 0;
  int current_batch_size = 1000;
  int current_fade_alpha = 4;
  bool playing = false;
  float color_float[3] = {0.0f, 1.0f, 1.0f};

public:
  /**
   * @brief Draws the ImGui layout and applies user-selected state via
   * out-parameters.
   *
   * @param generators List of available generators (for the combo box).
   * @param visualizations List of available visualizations (for the combo box).
   * @param outPlaying Reference to Application playing state.
   * @param outGenIdx Reference to Application selected generator index.
   * @param outVizIdx Reference to Application selected visualization index.
   * @param outColor Reference to Application selected tint color.
   * @param outBatchSize Reference to Application requested points per frame.
   * @param outFadeAlpha Reference to Application requested fade overlay alpha.
   * @param outClearRequested True if the user manually triggers a canvas clear
   * this frame.
   */
  void
  render(const std::vector<std::unique_ptr<IGenerator>> &generators,
         const std::vector<std::unique_ptr<IVisualization>> &visualizations,
         bool &outPlaying, int &outGenIdx, int &outVizIdx, sf::Color &outColor,
         int &outBatchSize, int &outFadeAlpha, bool &outClearRequested);

  /**
   * @brief Inverts the playing state (Play/Pause toggle).
   */
  void togglePlaying() { playing = !playing; }

  /**
   * @brief Retrieves the current play state.
   *
   * @return True if geometry is currently generating, False otherwise.
   */
  bool isPlaying() const { return playing; }
};
