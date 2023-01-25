#include "main.hpp"
#include "Config.hpp"

#include <filesystem>

const std::string configPath = "/sdcard/moddata/com.beatgames.beatsaber/mods/PracticeAddons/LevelData.json";

PracticeAddons::Config::Levels levels;
PracticeAddons::Config::Difficulty* currentDifficulty;

namespace PracticeAddons::Config {

    void Init()
    {
        if(!std::filesystem::exists(configPath))
        {   
            std::filesystem::create_directories("/sdcard/moddata/com.beatgames.beatsaber/mods/PracticeAddons");
            // Create blank config file
            WriteToFile(configPath, levels);
            return;
        }
        ReadFromFile(configPath, levels);
    }

    Difficulty GetLevelInfo(std::string levelId, std::string characteristic, std::string difficulty)
    {
        bool exists = levels.levels[levelId].characteristics[characteristic].difficulties.contains(difficulty);
        currentDifficulty = &levels.levels[levelId].characteristics[characteristic].difficulties[difficulty];

        if(!exists)
            *currentDifficulty = Difficulty();

        return *currentDifficulty;
    }

    Difficulty GetCurrentLevelInfo() 
    {
        return *currentDifficulty;
    }

    void SaveLevelInfo(Difficulty difficulty)
    {
        *currentDifficulty = difficulty;
        WriteToFile(configPath, levels);
    }
}