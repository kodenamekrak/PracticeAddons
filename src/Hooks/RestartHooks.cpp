#include "main.hpp"
#include "Hooks/RestartHooks.hpp"
#include "Config.hpp"

#include "GlobalNamespace/AudioTimeSyncController.hpp"
#include "GlobalNamespace/PauseController.hpp"
#include "GlobalNamespace/ILevelRestartController.hpp"
#include "GlobalNamespace/SinglePlayerLevelSelectionFlowCoordinator.hpp"
#include "GlobalNamespace/MultiplayerLevelScenesTransitionSetupDataSO.hpp"

#include "UnityEngine/Resources.hpp"

bool isPractice, shouldReset;

MAKE_HOOK_MATCH(AudioTimeSyncController_Update, &GlobalNamespace::AudioTimeSyncController::Update, void, GlobalNamespace::AudioTimeSyncController* self)
{
    AudioTimeSyncController_Update(self);
    if(!isPractice || !shouldReset)
        return;
    auto values = PracticeAddons::Config::GetCurrentLevelInfo();
    if(values.resetValue > 1 && values.resetValue > values.startValue && self->songTime > values.resetValue)
    {
        auto pauseController = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::PauseController*>().First();
        pauseController->levelRestartController->RestartLevel();
        shouldReset = false;
    }
}

MAKE_HOOK_MATCH(SinglePlayerLevelSelectionFlowCoordinator_StartLevel, &GlobalNamespace::SinglePlayerLevelSelectionFlowCoordinator::StartLevel, void, GlobalNamespace::SinglePlayerLevelSelectionFlowCoordinator* self, System::Action* beforeSceneSwitchCallback, bool practice)
{
    SinglePlayerLevelSelectionFlowCoordinator_StartLevel(self, beforeSceneSwitchCallback, practice);
    isPractice = practice;
    shouldReset = true;
}

MAKE_HOOK_MATCH(MultiplayerLevelScenesTransitionSetupDataSO_Init, &GlobalNamespace::MultiplayerLevelScenesTransitionSetupDataSO::Init, void, GlobalNamespace::MultiplayerLevelScenesTransitionSetupDataSO* self, StringW gameMode, GlobalNamespace::IPreviewBeatmapLevel* previewBeatmapLevel, GlobalNamespace::BeatmapDifficulty beatmapDifficulty, GlobalNamespace::BeatmapCharacteristicSO* beatmapCharacteristic, GlobalNamespace::IDifficultyBeatmap* difficultyBeatmap, GlobalNamespace::ColorScheme* overrideColorScheme, GlobalNamespace::GameplayModifiers* gameplayModifiers, GlobalNamespace::PlayerSpecificSettings* playerSpecificSettings, GlobalNamespace::PracticeSettings* practiceSettings, bool useTestNoteCutSoundEffects)
{
    MultiplayerLevelScenesTransitionSetupDataSO_Init(self, gameMode, previewBeatmapLevel, beatmapDifficulty, beatmapCharacteristic, difficultyBeatmap, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, useTestNoteCutSoundEffects);
    isPractice = false;
}

namespace PracticeAddons::Hooks {
    void InstallRestartHooks()
    {
        INSTALL_HOOK(getLogger(), MultiplayerLevelScenesTransitionSetupDataSO_Init);
        INSTALL_HOOK(getLogger(), AudioTimeSyncController_Update);
        INSTALL_HOOK(getLogger(), SinglePlayerLevelSelectionFlowCoordinator_StartLevel);
    }
}