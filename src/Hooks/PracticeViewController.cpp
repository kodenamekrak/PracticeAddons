#include "main.hpp"
#include "Hooks/PracticeViewController.hpp"
#include "Config.hpp"
#include "SliderManager.hpp"

#include "GlobalNamespace/PracticeViewController.hpp"
#include "GlobalNamespace/IBeatmapLevel.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"
#include "GlobalNamespace/BeatmapDifficulty.hpp"

#include "GlobalNamespace/PerceivedLoudnessPerLevelModel.hpp"
#include "GlobalNamespace/IBeatmapLevelData.hpp"
#include "GlobalNamespace/PracticeSettings.hpp"

#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"

using namespace GlobalNamespace;
using namespace PracticeAddons;

MAKE_HOOK_MATCH(PracticeViewController_Init, &PracticeViewController::Init, void, PracticeViewController* self, GlobalNamespace::IBeatmapLevel* level, GlobalNamespace::BeatmapCharacteristicSO* beatmapCharacteristic, GlobalNamespace::BeatmapDifficulty beatmapDifficulty)
{
    PracticeViewController_Init(self, level, beatmapCharacteristic, beatmapDifficulty);

    // something is fucking up here with the config
    // probably because pointers ????
    auto values = Config::GetLevelInfo(    
        static_cast<std::string>(level->i_IPreviewBeatmapLevel()->get_levelID()),
        static_cast<std::string>(beatmapCharacteristic->serializedName),
        std::to_string(beatmapDifficulty)
    );
    getLogger().info("Values for level '%s' are %f, %f, %f", static_cast<std::string>(level->i_IPreviewBeatmapLevel()->get_levelID()).c_str(), values.speedValue, values.startValue, values.resetValue);
    self->practiceSettings->songSpeedMul = values.speedValue;
    self->practiceSettings->startSongTime = values.startValue;

    float volume =  self->perceivedLoudnessPerLevelModel->GetLoudnessCorrectionByLevelId(level->i_IPreviewBeatmapLevel()->get_levelID());
    SliderManager::SetPreview(level->get_beatmapLevelData()->get_audioClip(), volume);
}

MAKE_HOOK_MATCH(PracticeViewController_DidActivate, &GlobalNamespace::PracticeViewController::DidActivate, void, GlobalNamespace::PracticeViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    PracticeViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if(firstActivation)
    {
        SliderManager::CreateSlider(self->get_transform());
        auto rect = self->playButton->GetComponent<UnityEngine::RectTransform*>();
        rect->set_anchoredPosition({rect->get_anchoredPosition().x, -67});
    }
    SliderManager::HandleSliderValues(self->speedSlider, self->songStartSlider);
}

MAKE_HOOK_MATCH(PracticeViewController_DidDeactivate, &PracticeViewController::DidDeactivate, void, PracticeViewController* self, bool removedFromHierarchy, bool screenSystemDisabling)
{
    PracticeViewController_DidDeactivate(self, removedFromHierarchy, screenSystemDisabling);

    Config::Difficulty diff;
    diff.speedValue = self->speedSlider->get_value();
    diff.startValue = self->songStartSlider->get_value();
    diff.resetValue = SliderManager::GetSliderValue();
    Config::SaveLevelInfo(diff);
}

namespace PracticeAddons::Hooks {
    void InstallPracticeViewControllerHooks()
    {
        INSTALL_HOOK(getLogger(), PracticeViewController_Init);
        INSTALL_HOOK(getLogger(), PracticeViewController_DidDeactivate);
        INSTALL_HOOK(getLogger(), PracticeViewController_DidActivate);
    }
}