#pragma once
#include "generators/IGenerator.hpp"
#include "visualizations/IVisualization.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class GuiPanel {
  int current_gen_idx = 0;
  int current_viz_idx = 0;
  bool playing = false;
  float color_float[3] = {0.0f, 1.0f, 1.0f};

public:
  void
  render(const std::vector<std::unique_ptr<IGenerator>> &generators,
         const std::vector<std::unique_ptr<IVisualization>> &visualizations,
         bool &outPlaying, int &outGenIdx, int &outVizIdx, sf::Color &outColor,
         bool &outClearRequested);

  void togglePlaying() { playing = !playing; }
  bool isPlaying() const { return playing; }
};
