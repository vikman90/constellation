#include "Application.hpp"
#include "generators/GeneratorFactory.hpp"
#include "visualizations/Display2D.hpp"
#include "visualizations/Histogram.hpp"
#include <imgui-SFML.h>
#include <imgui.h>

Application::Application()
    : window(sf::VideoMode(1000, 800), "Constellation RNG Visualizer"),
      playing(false), current_gen_idx(0), current_viz_idx(0),
      current_color(sf::Color::Cyan) {
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);

  generators = GeneratorFactory::createAllAvailable();

  visualizations.push_back(std::make_unique<Display2DVis>());
  visualizations.push_back(std::make_unique<HistogramVis>());
}

Application::~Application() { ImGui::SFML::Shutdown(); }

void Application::run() {
  sf::Clock deltaClock;
  while (window.isOpen()) {
    processEvents();
    update(deltaClock.restart());
    render();
  }
}

void Application::processEvents() {
  sf::Event event;
  while (window.pollEvent(event)) {
    ImGui::SFML::ProcessEvent(window, event);

    if (event.type == sf::Event::Closed) {
      window.close();
    }
    if (event.type == sf::Event::Resized) {
      // Reset the view to match the new window size so SFML's coordinate
      // system stays 1:1 with pixels after a resize.
      sf::FloatRect visibleArea(0.f, 0.f, static_cast<float>(event.size.width),
                                static_cast<float>(event.size.height));
      window.setView(sf::View(visibleArea));
    }
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Space) {
      gui.togglePlaying();
    }
  }
}

void Application::update(sf::Time deltaTime) {
  ImGui::SFML::Update(window, deltaTime);

  bool clearRequested = false;
  int batchSize = 0;
  int fadeAlpha = 8;

  gui.render(generators, visualizations, playing, current_gen_idx,
             current_viz_idx, current_color, batchSize, fadeAlpha,
             clearRequested);

  visualizations[current_viz_idx]->setFadeAlpha(fadeAlpha);

  if (clearRequested) {
    resetCanvas();
  }

  if (playing) {
    auto *gen = generators[current_gen_idx].get();
    auto *viz = visualizations[current_viz_idx].get();

    viz->processBatch(gen, window.getSize(), batchSize);
  }
}

void Application::render() {
  window.clear(sf::Color(20, 20, 20));

  // Draw visualizer output
  visualizations[current_viz_idx]->render(window, current_color);

  // Draw GUI
  ImGui::SFML::Render(window);
  window.display();
}

void Application::resetCanvas() {
  for (auto &viz : visualizations) {
    viz->clear();
  }
}
