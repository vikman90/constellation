#include "Display2D.hpp"

Display2DVis::Display2DVis()
    : batch(sf::Points), totalPoints(0), fade_alpha(4) {}

void Display2DVis::ensureCanvas(const sf::Vector2u &windowSize) {
  if (canvas && lastWindowSize == windowSize)
    return;

  // Create the new canvas at the requested size
  auto newCanvas = std::make_unique<sf::RenderTexture>();
  newCanvas->create(windowSize.x, windowSize.y);
  newCanvas->clear(sf::Color::Black);

  // If a previous canvas exists, scale its content into the new one
  if (canvas && canvas->getSize().x > 0) {
    canvas->display();
    sf::Sprite oldContent(canvas->getTexture());
    oldContent.setScale(static_cast<float>(windowSize.x) / canvas->getSize().x,
                        static_cast<float>(windowSize.y) / canvas->getSize().y);
    newCanvas->draw(oldContent);
  }

  newCanvas->display();
  canvas = std::move(newCanvas);
  lastWindowSize = windowSize;

  fadeOverlay.setSize(sf::Vector2f(static_cast<float>(windowSize.x),
                                   static_cast<float>(windowSize.y)));
}

void Display2DVis::processBatch(IGenerator *gen, const sf::Vector2u &windowSize,
                                int batchSize) {
  ensureCanvas(windowSize);

  for (int i = 0; i < batchSize; ++i) {
    double x_val = gen->next();
    double y_val = gen->next();
    float px = static_cast<float>(x_val * windowSize.x);
    float py = static_cast<float>((1.0 - y_val) * windowSize.y);
    batch.append(sf::Vertex(sf::Vector2f(px, py), sf::Color::White));
    ++totalPoints;
  }
}

void Display2DVis::render(sf::RenderTarget &target, const sf::Color &color) {
  if (!canvas)
    return;

  // Only apply fade and draw new points when there is an active batch.
  // This ensures pausing stops the fade immediately.
  if (batch.getVertexCount() > 0) {
    // 1. Apply the fade overlay to darken old points on the canvas
    fadeOverlay.setFillColor(
        sf::Color(0, 0, 0, static_cast<sf::Uint8>(fade_alpha)));
    canvas->draw(fadeOverlay, sf::BlendAlpha);

    // 2. Color and draw the new batch of points onto the canvas
    for (size_t i = 0; i < batch.getVertexCount(); ++i)
      batch[i].color = color;
    canvas->draw(batch);
  }

  canvas->display();

  // 3. Blit canvas to the main window, scaling to fill it entirely
  sf::Sprite sprite(canvas->getTexture());
  sprite.setScale(static_cast<float>(target.getSize().x) / canvas->getSize().x,
                  static_cast<float>(target.getSize().y) / canvas->getSize().y);
  target.draw(sprite);

  // Clear batch after drawing so the next frame starts empty.
  // If not playing, processBatch won't refill it and the fade won't run.
  batch.clear();
}

void Display2DVis::clear() {
  batch.clear();
  totalPoints = 0;
  if (canvas)
    canvas->clear(sf::Color::Black);
}

size_t Display2DVis::getDataPointCount() const { return totalPoints; }

int Display2DVis::getRecommendedBatchSize() const { return 1000; }

const char *Display2DVis::name() const { return "Display (2D Points)"; }

void Display2DVis::setFadeAlpha(int alpha) {
  fade_alpha = std::max(0, std::min(255, alpha));
}
