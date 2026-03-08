#pragma once

#include "GuiPanel.hpp"
#include "generators/IGenerator.hpp"
#include "visualizations/IVisualization.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>


class Application {
  sf::RenderWindow window;
  GuiPanel gui;

  std::vector<std::unique_ptr<IGenerator>> generators;
  std::vector<std::unique_ptr<IVisualization>> visualizations;

  // State
  bool playing;
  int current_gen_idx;
  int current_viz_idx;
  sf::Color current_color;

public:
  Application();
  ~Application();

  void run();

private:
  void processEvents();
  void update(sf::Time deltaTime);
  void render();
  void resetCanvas();
};
