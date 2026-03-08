#include "Histogram.hpp"
#include <algorithm>

HistogramVis::HistogramVis() : total_count(0) {}

void HistogramVis::processBatch(IGenerator *gen, const sf::Vector2u &windowSize,
                                int batchSize) {
  int numBins = static_cast<int>(windowSize.x);

  // Resize (and implicitly reset) when the window width changes
  if (windowSize != lastWindowSize) {
    histogram.assign(numBins, 0);
    total_count = 0;
    lastWindowSize = windowSize;
  }

  for (int i = 0; i < batchSize; ++i) {
    double val = gen->next();
    if (val < 0.0 || val >= 1.0)
      continue; // skip out-of-range values (e.g. from exponential distribution)
    int bin = static_cast<int>(val * numBins);
    bin = std::max(0, std::min(numBins - 1, bin)); // safety clamp
    histogram[bin]++;
    total_count++;
  }
}

void HistogramVis::render(sf::RenderTarget &target, const sf::Color &color) {
  if (histogram.empty() || total_count == 0)
    return;

  auto size = target.getSize();
  int numBins = static_cast<int>(histogram.size());

  // Normalize using density = histogram[i] * numBins (proportional to the
  // continuous PDF), so the visual appearance is width-independent.
  // For a uniform distribution, mean density = total_count → H/2.
  // For skewed distributions, the peak density limits the scale.
  unsigned long long max_bin =
      *std::max_element(histogram.begin(), histogram.end());
  double max_density = static_cast<double>(max_bin) * numBins;

  double scale_by_mean = (total_count > 0) ? (size.y / 2.0) / total_count : 1.0;
  double scale_by_max =
      (max_density > 0) ? static_cast<double>(size.y) / max_density : 1.0;
  double scale = std::min(scale_by_mean, scale_by_max);

  sf::VertexArray lines(sf::Lines);
  for (int i = 0; i < numBins; ++i) {
    // density = histogram[i] * numBins; cast to double to avoid integer
    // overflow
    float h =
        static_cast<float>(static_cast<double>(histogram[i]) * numBins * scale);
    float x = static_cast<float>(i);
    lines.append(
        sf::Vertex(sf::Vector2f(x, static_cast<float>(size.y)), color));
    lines.append(sf::Vertex(sf::Vector2f(x, size.y - h), color));
  }
  target.draw(lines);
}

void HistogramVis::clear() {
  std::fill(histogram.begin(), histogram.end(), 0);
  total_count = 0;
}

size_t HistogramVis::getDataPointCount() const { return total_count; }

int HistogramVis::getRecommendedBatchSize() const { return 1000; }

const char *HistogramVis::name() const { return "Histogram (1D Bars)"; }
