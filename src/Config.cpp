#include "main.hpp"
#include "Config.hpp"

#include <filesystem>

const std::string configPath = "/sdcard/moddata/com.beatgames.beatsaber/mods/PracticeAddons/LevelData.json";

PracticeAddons::Config::Levels levels;
PracticeAddons::Config::Difficulty* currentDifficulty;
std::string currentLevelId;

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
        currentLevelId = levelId;
        bool exists = levels.levels[levelId].characteristics[characteristic].difficulties.contains(difficulty);
        currentDifficulty = &levels.levels[levelId].characteristics[characteristic].difficulties[difficulty];

        Difficulty diff;
        if(exists)
        {
            diff = *currentDifficulty;
            getLogger().info("Has value for level id '%s'", levelId.c_str());
        }
        else
            getLogger().info("Does not have value for level id '%s'", levelId.c_str());
        return diff;
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