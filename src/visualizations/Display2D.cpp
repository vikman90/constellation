#include "Display2D.hpp"

Display2DVis::Display2DVis()
    : batch(sf::Points), totalPoints(0), fade_alpha(4) {}

void Display2DVis::ensureCanvas(const sf::Vector2u &windowSize) {
  if (lastWindowSize == windowSize && canvas.getSize() == windowSize)
    return;

  lastWindowSize = windowSize;
  canvas.create(windowSize.x, windowSize.y);
  canvas.clear(sf::Color::Black);
  fadeOverlay.setSize(sf::Vector2f(static_cast<float>(windowSize.x),
                                   static_cast<float>(windowSize.y)));
}

void Display2DVis::processBatch(IGenerator *gen, const sf::Vector2u &windowSize,
                                int batchSize) {
  ensureCanvas(windowSize);
  batch.clear();

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
  // 1. Apply the fade overlay to darken old points on the canvas
  fadeOverlay.setFillColor(
      sf::Color(0, 0, 0, static_cast<sf::Uint8>(fade_alpha)));
  canvas.draw(fadeOverlay, sf::BlendAlpha);

  // 2. Color and draw the new batch of points onto the canvas
  for (size_t i = 0; i < batch.getVertexCount(); ++i)
    batch[i].color = color;
  canvas.draw(batch);
  canvas.display();

  // 3. Blit the canvas texture to the main window
  sf::Sprite sprite(canvas.getTexture());
  target.draw(sprite);
}

void Display2DVis::clear() {
  batch.clear();
  totalPoints = 0;
  if (canvas.getSize().x > 0)
    canvas.clear(sf::Color::Black);
}

size_t Display2DVis::getDataPointCount() const { return totalPoints; }

int Display2DVis::getRecommendedBatchSize() const { return 1000; }

const char *Display2DVis::name() const { return "Display (2D Points)"; }

void Display2DVis::setFadeAlpha(int alpha) {
  fade_alpha = std::max(0, std::min(255, alpha));
}
