#include "rapidjson-macros/shared/macros.hpp"
#include "GlobalNamespace/PracticeViewController.hpp"

namespace PracticeAddons::Config {

    DECLARE_JSON_CLASS(Difficulty,
        VALUE_DEFAULT(float, speedValue, 1.0f);
        VALUE_DEFAULT(float, startValue, 0.0f);
        VALUE_DEFAULT(float, resetValue, 0.0f);
    );

    DECLARE_JSON_CLASS(Characteristic,
        MAP(Difficulty, difficulties);
    );

    DECLARE_JSON_CLASS(Level,
        MAP(Characteristic, characteristics);
    );

    DECLARE_JSON_CLASS(Levels,
        MAP(Level, levels);
    );

    struct LevelInfo {
        std::string levelId;
        std::string characteristic;
        std::string difficulty;
    };

    void Init();

    Difficulty GetLevelInfo(std::string levelId, std::string characteristic, std::string difficulty);

    Difficulty GetCurrentLevelInfo();

    void SaveLevelInfo(Difficulty difficulty);
}