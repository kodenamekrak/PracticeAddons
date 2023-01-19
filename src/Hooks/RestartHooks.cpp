#include "main.hpp"
#include "Hooks/RestartHooks.hpp"
#include "Config.hpp"

#include "GlobalNamespace/AudioTimeSyncController.hpp"
#include "GlobalNamespace/PauseController.hpp"
#include "GlobalNamespace/ILevelRestartController.hpp"
#include "GlobalNamespace/SinglePlayerLevelSelectionFlowCoordinator.hpp"

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

MAKE_HOOK_MATCH(AudioTimeSyncController_Start, &GlobalNamespace::AudioTimeSyncController::Start, void, GlobalNamespace::AudioTimeSyncController* self)
{
    AudioTimeSyncController_Start(self);
    shouldReset = true;
}

MAKE_HOOK_MATCH(SinglePlayerLevelSelectionFlowCoordinator_StartLevel, &GlobalNamespace::SinglePlayerLevelSelectionFlowCoordinator::StartLevel, void, GlobalNamespace::SinglePlayerLevelSelectionFlowCoordinator* self, System::Action* beforeSceneSwitchCallback, bool practice)
{
    SinglePlayerLevelSelectionFlowCoordinator_StartLevel(self, beforeSceneSwitchCallback, practice);
    isPractice = practice;
}

namespace PracticeAddons::Hooks {
    void InstallRestartHooks()
    {
        INSTALL_HOOK(getLogger(), AudioTimeSyncController_Update);
        INSTALL_HOOK(getLogger(), AudioTimeSyncController_Start);
        INSTALL_HOOK(getLogger(), SinglePlayerLevelSelectionFlowCoordinator_StartLevel);
    }
}