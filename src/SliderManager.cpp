#include "main.hpp"
#include "SliderManager.hpp"
#include "Config.hpp"

#include "GlobalNamespace/PracticeViewController.hpp"
#include "GlobalNamespace/SongPreviewPlayer.hpp"
#include "GlobalNamespace/PerceivedLoudnessPerLevelModel.hpp"

#include "HMUI/TimeSlider.hpp"
#include "HMUI/RangeValuesTextSlider.hpp"

#include "UnityEngine/Resources.hpp"
#include "UnityEngine/RectTransform.hpp"

#include "System/Action_2.hpp"

#include "custom-types/shared/delegate.hpp"

#include "questui/shared/BeatSaberUI.hpp"

GlobalNamespace::SongPreviewPlayer* previewPlayer;
HMUI::RangeValuesTextSlider* restartSlider;
float previewVolume;
UnityEngine::AudioClip* previewClip;

namespace PracticeAddons::SliderManager {

    void PlayPreview(HMUI::RangeValuesTextSlider* slider, float value)
    {
        if(!previewPlayer)
            previewPlayer = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::SongPreviewPlayer*>().First();
        
        previewPlayer->CrossfadeTo(previewClip, previewVolume, value, 5, nullptr);
    }

    void SetPreview(UnityEngine::AudioClip* audioClip, float volume)
    {
        previewClip = audioClip;
        previewVolume = volume;
    }

    void CreateSlider(UnityEngine::Transform* parent)
    {
        auto slider = QuestUI::BeatSaberUI::CreateSliderSetting(parent, "", 1, 5, 0, 10, 0.01f);
        // auto slider = BSML::Lite::CreateSliderSetting(parent, "", 1, 5, 0, 10, 0.01f);
        slider->FormatString = std::move([](float value)
        {
            int m = value / 60;
            int s = value - m * 60;
            if(m) return std::to_string(m) + " m " + std::to_string(s) + " s";
            return std::to_string(s) + " s";
        });
        restartSlider = slider->slider;

        auto resetTrans = restartSlider->GetComponent<UnityEngine::RectTransform*>();
        resetTrans->set_sizeDelta({110, 3});
        resetTrans->set_anchoredPosition({-25, -57});

        restartSlider->valueDidChangeEvent = custom_types::MakeDelegate<System::Action_2<HMUI::RangeValuesTextSlider* , float>*>(std::function(PlayPreview));
        QuestUI::BeatSaberUI::CreateText(parent, "Song Restart", {21, -16});
    }
    
    void HandleSliderValues(HMUI::PercentSlider* speedSlider, HMUI::TimeSlider* startSlider)
    {
        auto values = Config::GetCurrentLevelInfo();

        speedSlider->set_value(values.speedValue);
        startSlider->set_value(values.startValue);

        restartSlider->set_maxValue(startSlider->get_maxValue() - 1);
        restartSlider->set_numberOfSteps(startSlider->get_numberOfSteps());
        restartSlider->set_value(values.resetValue);
    }

    float GetSliderValue()
    {
        if(!restartSlider)
            return 0;

        return restartSlider->get_value();
    }
}