#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <random>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")
#endif

// Generator Wrappers
struct Generator {
    virtual ~Generator() {}
    virtual double next() = 0; // Return value between 0.0 and 1.0
    virtual const char* name() const = 0;
};

struct GenMT19937 : public Generator {
    std::mt19937 gen;
    std::uniform_real_distribution<double> dist{0.0, 1.0};
    GenMT19937() : gen(std::random_device{}()) {}
    double next() override { return dist(gen); }
    const char* name() const override { return "Mersenne Twister (std::mt19937)"; }
};

struct GenCRand : public Generator {
    GenCRand() { srand(time(nullptr)); }
    double next() override { return (double)rand() / RAND_MAX; }
    const char* name() const override { return "C rand()"; }
};

#ifdef _WIN32
struct GenWindows : public Generator {
    BCRYPT_ALG_HANDLE hAlg = NULL;
    GenWindows() {
        BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_RNG_ALGORITHM, NULL, 0);
    }
    ~GenWindows() {
        if (hAlg) BCryptCloseAlgorithmProvider(hAlg, 0);
    }
    double next() override {
        unsigned int val = 0;
        if (hAlg) {
            BCryptGenRandom(hAlg, (PUCHAR)&val, sizeof(val), 0);
        }
        return (double)val / 0xFFFFFFFF;
    }
    const char* name() const override { return "Windows BCryptGenRandom"; }
};
#endif

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 800), "RNG Visualizer Contellation");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    // Instances
    std::vector<Generator*> generators;
    generators.push_back(new GenMT19937());
    generators.push_back(new GenCRand());
#ifdef _WIN32
    generators.push_back(new GenWindows());
#endif

    int current_gen_idx = 0;
    int current_viz_idx = 0; // 0 = Display, 1 = Histogram
    const char* viz_types[] = { "Display (2D Points)", "Histogram (1D Bars)" };
    
    // State
    bool playing = false;
    
    // Display mode state
    sf::VertexArray points(sf::Points);
    sf::Color current_color = sf::Color::Cyan;
    float color_float[3] = {0.0f, 1.0f, 1.0f};
    
    // Histogram mode state
    // We will use 1000 bins for the histogram
    const int HIST_BINS = 1000;
    std::vector<unsigned long long> histogram(HIST_BINS, 0);
    unsigned long long max_hist_val = 1;

    sf::Clock deltaClock;
    
    auto clear_data = [&]() {
        points.clear();
        std::fill(histogram.begin(), histogram.end(), 0);
        max_hist_val = 1;
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                playing = !playing; // Toggle with space
            }
            // If window resized, we might want to adapt, but let's keep it simple
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        // ImGui Window
        ImGui::Begin("Controls");
        
        // Generator Selection
        if (ImGui::BeginCombo("Generator", generators[current_gen_idx]->name())) {
            for (int n = 0; n < generators.size(); n++) {
                bool is_selected = (current_gen_idx == n);
                if (ImGui::Selectable(generators[n]->name(), is_selected)) {
                    current_gen_idx = n;
                    clear_data();
                }
                if (is_selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        
        // Visualization Selection
        if (ImGui::Combo("Visualization", &current_viz_idx, viz_types, 2)) {
            clear_data();
        }

        // Color Picker
        if (ImGui::ColorEdit3("Point/Bar Color", color_float)) {
            current_color = sf::Color(color_float[0] * 255, color_float[1] * 255, color_float[2] * 255);
        }

        ImGui::Separator();

        if (ImGui::Button(playing ? "Pause (Space)" : "Play (Space)", ImVec2(120, 30))) {
            playing = !playing;
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear", ImVec2(80, 30))) {
            clear_data();
        }
        
        ImGui::Text("Points generated: %zu", points.getVertexCount());
        
        ImGui::End();

        // Application Logic
        if (playing) {
            Generator* gen = generators[current_gen_idx];
            // Generate multiple per frame to speed up visualization
            int batch_size = (current_viz_idx == 0) ? 5000 : 50000;
            
            auto size = window.getSize();
            
            for (int i = 0; i < batch_size; ++i) {
                if (current_viz_idx == 0) {
                    // Display: 2 consecutive numbers for X and Y
                    double x_val = gen->next();
                    double y_val = gen->next();
                    float px = x_val * size.x;
                    float py = y_val * size.y;
                    points.append(sf::Vertex(sf::Vector2f(px, py), current_color));
                } else if (current_viz_idx == 1) {
                    // Histogram
                    double val = gen->next();
                    int bin = std::min(HIST_BINS - 1, std::max(0, (int)(val * HIST_BINS)));
                    histogram[bin]++;
                    if (histogram[bin] > max_hist_val) {
                        max_hist_val = histogram[bin];
                    }
                }
            }
        }

        window.clear(sf::Color(20, 20, 20));

        // Draw Canvas
        if (current_viz_idx == 0) {
            window.draw(points);
        } else if (current_viz_idx == 1) {
            // Draw Histogram
            auto size = window.getSize();
            float bin_width = (float)size.x / HIST_BINS;
            
            sf::VertexArray lines(sf::Lines);
            for (int i = 0; i < HIST_BINS; ++i) {
                float h = ((float)histogram[i] / max_hist_val) * size.y;
                float px = i * bin_width;
                lines.append(sf::Vertex(sf::Vector2f(px, size.y), current_color));
                lines.append(sf::Vertex(sf::Vector2f(px, size.y - h), current_color));
            }
            window.draw(lines);
        }

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    
    for (auto g : generators) delete g;
    return 0;
}
