#include "Histogram.hpp"
#include <algorithm>

HistogramVis::HistogramVis() : max_val(1) {}

void HistogramVis::processBatch(IGenerator *gen, const sf::Vector2u &windowSize,
                                int batchSize) {
  int numBins = static_cast<int>(windowSize.x);

  // Resize (and implicitly reset) when the window width changes
  if (windowSize != lastWindowSize) {
    histogram.assign(numBins, 0);
    max_val = 1;
    lastWindowSize = windowSize;
  }

  for (int i = 0; i < batchSize; ++i) {
    double val = gen->next();
    int bin = static_cast<int>(val * numBins);
    bin = std::max(0, std::min(numBins - 1, bin));
    histogram[bin]++;
    if (histogram[bin] > max_val)
      max_val = histogram[bin];
  }
}

void HistogramVis::render(sf::RenderTarget &target, const sf::Color &color) {
  if (histogram.empty())
    return;

  auto size = target.getSize();
  int numBins = static_cast<int>(histogram.size());

  // One line per bin (= one line per pixel column)
  sf::VertexArray lines(sf::Lines);
  for (int i = 0; i < numBins; ++i) {
    float h = (static_cast<float>(histogram[i]) / max_val) * size.y;
    float x = static_cast<float>(i);
    lines.append(
        sf::Vertex(sf::Vector2f(x, static_cast<float>(size.y)), color));
    lines.append(sf::Vertex(sf::Vector2f(x, size.y - h), color));
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
