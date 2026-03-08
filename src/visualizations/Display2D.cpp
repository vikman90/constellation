#include "Display2D.hpp"

Display2DVis::Display2DVis() : points(sf::Points) {}

void Display2DVis::processBatch(IGenerator *gen, const sf::Vector2u &windowSize,
                                int batchSize) {
  for (int i = 0; i < batchSize; ++i) {
    double x_val = gen->next();
    double y_val = gen->next();
    float px = static_cast<float>(x_val * windowSize.x);
    float py = static_cast<float>((1.0 - y_val) * windowSize.y);

    // Color will be set during render
    points.append(sf::Vertex(sf::Vector2f(px, py), sf::Color::White));
  }
}

void Display2DVis::render(sf::RenderTarget &target, const sf::Color &color) {
  // Optionally update colors if they changed, or just apply uniformly.
  // For performance, we can just apply it directly to new points,
  // but here we might want to change all existing points instantly.
  // However, iterating millions of points each frame is slow.
  // We will cheat and just draw it as is, or set the color at creation time.
  // To support instant color change without redefining all points:
  // It's better to manage color application externally or draw with a shader.
  // Let's modify all colors if the user changes them? Too slow.
  // We will just let new points have the new color, or we reset array colors.
  // Let's apply color to all vertices. (Fast enough for < 10M on modern CPUs).

  /*
  for(size_t i = 0; i < points.getVertexCount(); ++i) {
      points[i].color = color;
  }
  */
  // To save CPU, we'll assign color when generating in real-time,
  // but the updated design calls for color at render.
  // Let's just update the vertices quickly.
  for (size_t i = 0; i < points.getVertexCount(); ++i) {
    points[i].color = color;
  }

  target.draw(points);
}

void Display2DVis::clear() { points.clear(); }

size_t Display2DVis::getDataPointCount() const {
  return points.getVertexCount();
}

int Display2DVis::getRecommendedBatchSize() const { return 5000; }

const char *Display2DVis::name() const { return "Display (2D Points)"; }
