#include "main.hpp"
#include "Config.hpp"

#include "GlobalNamespace/PracticeViewController.hpp"
#include "GlobalNamespace/IBeatmapLevel.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"
#include "GlobalNamespace/BeatmapDifficulty.hpp"

using namespace GlobalNamespace;

MAKE_HOOK_MATCH(PracticeViewController_Init, &PracticeViewController::Init, void, PracticeViewController* self, GlobalNamespace::IBeatmapLevel* level, GlobalNamespace::BeatmapCharacteristicSO* beatmapCharacteristic, GlobalNamespace::BeatmapDifficulty beatmapDifficulty)
{
    PracticeViewController_Init(self, level, beatmapCharacteristic, beatmapDifficulty);

    auto diff = PracticeAddons::Config::GetLevelInfo(    
        static_cast<std::string>(level->i_IPreviewBeatmapLevel()->get_levelID()),
        static_cast<std::string>(beatmapCharacteristic->serializedName),
        std::to_string(beatmapDifficulty)
    );
    getLogger().info("%i, %f, %f", diff.speedValue, diff.startValue, diff.resetValue);
}

MAKE_HOOK_MATCH(PracticeViewController_DidDeactivate, &PracticeViewController::DidDeactivate, void, PracticeViewController* self, bool removedFromHierarchy, bool screenSystemDisabling)
{
    PracticeViewController_DidDeactivate(self, removedFromHierarchy, screenSystemDisabling);

    PracticeAddons::Config::Difficulty diff;
    diff.speedValue = 40;
    diff.startValue = 31.58f;
    diff.resetValue = 79.42f;
    PracticeAddons::Config::SaveLevelInfo(diff);
}

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

// Loads the config from disk using our modInfo, then returns it for use
// other config tools such as config-utils don't use this config, so it can be removed if those are in use
Configuration& getConfig() {
    static Configuration config(modInfo);
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = MOD_ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load();
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();

    PracticeAddons::Config::Init();

    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), PracticeViewController_Init);
    INSTALL_HOOK(getLogger(), PracticeViewController_DidDeactivate);
    getLogger().info("Installed all hooks!");
}