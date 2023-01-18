#include "main.hpp"
#include "Config.hpp"
#include "Hooks/PracticeViewController.hpp"
#include "Hooks/RestartHooks.hpp"

static ModInfo modInfo;

Configuration& getConfig() {
    static Configuration config(modInfo);
    return config;
}

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
    PracticeAddons::Hooks::InstallPracticeViewControllerHooks();
    PracticeAddons::Hooks::InstallRestartHooks();
    getLogger().info("Installed all hooks!");
}