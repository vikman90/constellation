#include "GuiPanel.hpp"
#include <imgui.h>

void GuiPanel::render(
    const std::vector<std::unique_ptr<IGenerator>> &generators,
    const std::vector<std::unique_ptr<IVisualization>> &visualizations,
    bool &outPlaying, int &outGenIdx, int &outVizIdx, sf::Color &outColor,
    bool &outClearRequested) {
  outClearRequested = false;

  ImGui::Begin("Controls");

  // Generator Selection
  if (ImGui::BeginCombo("Generator", generators[current_gen_idx]->name())) {
    for (int n = 0; n < static_cast<int>(generators.size()); n++) {
      bool is_selected = (current_gen_idx == n);
      if (ImGui::Selectable(generators[n]->name(), is_selected)) {
        current_gen_idx = n;
        outClearRequested = true;
      }
      if (is_selected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }

  // Visualization Selection
  if (ImGui::BeginCombo("Visualization",
                        visualizations[current_viz_idx]->name())) {
    for (int n = 0; n < static_cast<int>(visualizations.size()); n++) {
      bool is_selected = (current_viz_idx == n);
      if (ImGui::Selectable(visualizations[n]->name(), is_selected)) {
        current_viz_idx = n;
        outClearRequested = true;
      }
      if (is_selected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }

  // Color Picker
  if (ImGui::ColorEdit3("Color", color_float)) {
    // Updated color internally
  }

  ImGui::Separator();

  if (ImGui::Button(playing ? "Pause (Space)" : "Play (Space)",
                    ImVec2(120, 30))) {
    playing = !playing;
  }
  ImGui::SameLine();
  if (ImGui::Button("Clear", ImVec2(80, 30))) {
    outClearRequested = true;
  }

  ImGui::Text("Data points: %zu",
              visualizations[current_viz_idx]->getDataPointCount());

  ImGui::End();

  // Output state to Application
  outPlaying = playing;
  outGenIdx = current_gen_idx;
  outVizIdx = current_viz_idx;
  outColor = sf::Color(color_float[0] * 255, color_float[1] * 255,
                       color_float[2] * 255);
}
