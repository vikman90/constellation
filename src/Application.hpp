#pragma once

#include "GuiPanel.hpp"
#include "generators/IGenerator.hpp"
#include "visualizations/IVisualization.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

/**
 * @brief Core application engine that anchors the program.
 *
 * Responsible for initializing the SFML RenderWindow, managing the main game
 * loop, routing events down to the UI, and driving the visualization and
 * generator components.
 */
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
  /**
   * @brief Constructs the application and initializes internal resources.
   *
   * Pre-allocates the standard list of Generators and Visualizations, opens
   * the OS window, and initializes Dear ImGui.
   */
  Application();

  /**
   * @brief Destructor. Safely shuts down ImGui and graphics components.
   */
  ~Application();

  /**
   * @brief Enters the main execution loop.
   *
   * Blocks execution and processes window events, logic updates, and renders
   * frames continuously until the main window is closed.
   */
  void run();

private:
  /**
   * @brief Polls and processes all pending SFML window events.
   */
  void processEvents();

  /**
   * @brief Updates internal state and logic for the current frame.
   *
   * @param deltaTime The time elapsed since the last frame.
   */
  void update(sf::Time deltaTime);

  /**
   * @brief Clears the screen and draws the current visualization and UI frame.
   */
  void render();

  /**
   * @brief Purges internal drawing state to wipe the visual canvas.
   */
  void resetCanvas();
};
