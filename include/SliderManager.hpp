#include "HMUI/PercentSlider.hpp"
#include "HMUI/TimeSlider.hpp"

#include "UnityEngine/AudioClip.hpp"
#include "UnityEngine/Transform.hpp"

namespace PracticeAddons::SliderManager {

    void CreateSlider(UnityEngine::Transform* parent);

    void HandleSliderValues(HMUI::PercentSlider* speedSlider, HMUI::TimeSlider* startSlider);

    void SetPreview(UnityEngine::AudioClip* audioClip, float volume);

    float GetSliderValue();
}