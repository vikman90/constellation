#include "Histogram.hpp"
#include <algorithm>

HistogramVis::HistogramVis() : histogram(BINS, 0), max_val(1) {}

void HistogramVis::processBatch(IGenerator *gen, const sf::Vector2u &windowSize,
                                int batchSize) {
  for (int i = 0; i < batchSize; ++i) {
    double val = gen->next();
    int bin = std::max(0, std::min(BINS - 1, static_cast<int>(val * BINS)));
    histogram[bin]++;
    if (histogram[bin] > max_val) {
      max_val = histogram[bin];
    }
  }
}

void HistogramVis::render(sf::RenderTarget &target, const sf::Color &color) {
  auto size = target.getSize();
  float bin_width = static_cast<float>(size.x) / BINS;

  sf::VertexArray lines(sf::Lines);
  for (int i = 0; i < BINS; ++i) {
    float h = (static_cast<float>(histogram[i]) / max_val) * size.y;
    float px = i * bin_width;
    lines.append(sf::Vertex(sf::Vector2f(px, size.y), color));
    lines.append(sf::Vertex(sf::Vector2f(px, size.y - h), color));
  }
  target.draw(lines);
}

void HistogramVis::clear() {
  std::fill(histogram.begin(), histogram.end(), 0);
  max_val = 1;
}

size_t HistogramVis::getDataPointCount() const {
  size_t total = 0;
  for (auto v : histogram)
    total += v;
  return total;
}

int HistogramVis::getRecommendedBatchSize() const { return 1000; }

const char *HistogramVis::name() const { return "Histogram (1D Bars)"; }
