#include <algorithm>
#include <cstdint>
#include <cstring>
#include <limits>
#include <optional>

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

namespace Training {
    struct MiscStat {
        const char* name;
        const char* additionalName;
        std::uint32_t value;
        std::uint32_t additionalValue;
        std::uint8_t category;
    };

    RE::Setting* trainingNumAllowedPerLevelSetting = nullptr;
    std::uint32_t baseTrainingNumAllowedPerLevel = 0;
    RE::UI::Create_t* originalCreateTrainingMenu = nullptr;

    std::optional<std::uint32_t> getTrainingSessions() {
        static REL::Relocation<const RE::BSTArray<MiscStat>*> stats{
            REL::VariantID{514503, 400662, 0x2F8A3A8}.address()};
        if (!stats->data()) {
            return std::nullopt;
        }
        for (const auto& stat : *stats) {
            if (stat.name && ::_stricmp(stat.name, "Training Sessions") == 0) {
                return stat.value;
            }
        }
        return std::nullopt;
    }

    std::uint32_t getSkillTrainingsThisLevel(const RE::PlayerCharacter& player) {
        if (REL::Module::IsVR()) {
            return player.GetVRInfoRuntimeData()->skillTrainingsThisLevel;
        }
        return player.GetInfoRuntimeData().skillTrainingsThisLevel;
    }

    std::uint32_t calculateTrainingLimit(std::uint32_t baseLimit, std::uint32_t level, std::uint32_t trainingSessions,
                                         std::uint32_t skillTrainingsThisLevel) {
        const auto earned = static_cast<std::uint64_t>(level) * baseLimit;
        const auto spentBeforeLevel =
            trainingSessions > skillTrainingsThisLevel ? trainingSessions - skillTrainingsThisLevel : 0;
        const auto remaining = earned > spentBeforeLevel ? earned - spentBeforeLevel : 0;
        const auto limit = std::max(remaining, static_cast<std::uint64_t>(skillTrainingsThisLevel));
        return static_cast<std::uint32_t>(std::min(limit, std::uint64_t{std::numeric_limits<std::int32_t>::max()}));
    }

    void updateTrainingLimit() {
        const auto* player = RE::PlayerCharacter::GetSingleton();
        const auto trainingSessions = getTrainingSessions();
        if (!player || !trainingSessions) {
            SKSE::log::error("Cannot read player training data");
            return;
        }
        trainingNumAllowedPerLevelSetting->data.i =
            static_cast<std::int32_t>(calculateTrainingLimit(baseTrainingNumAllowedPerLevel, player->GetLevel(),
                                                             *trainingSessions, getSkillTrainingsThisLevel(*player)));
    }

    RE::IMenu* createTrainingMenu() {
        updateTrainingLimit();
        return originalCreateTrainingMenu();
    }

    void initialize() {
        auto* settings = RE::GameSettingCollection::GetSingleton();
        auto* setting = settings ? settings->GetSetting("iTrainingNumAllowedPerLevel") : nullptr;
        auto* ui = RE::UI::GetSingleton();
        if (!setting || !ui) {
            SKSE::log::error("Cannot initialize training");
            return;
        }
        const auto entry = ui->menuMap.find(RE::TrainingMenu::MENU_NAME);
        if (entry == ui->menuMap.end() || !entry->second.create) {
            SKSE::log::error("Training menu is not available");
            return;
        }

        baseTrainingNumAllowedPerLevel = static_cast<std::uint32_t>(std::max(setting->GetInteger(), 0));
        trainingNumAllowedPerLevelSetting = setting;
        originalCreateTrainingMenu = entry->second.create;
        entry->second.create = createTrainingMenu;
        SKSE::log::info("Runtime {}, trainings per level {}", REL::Module::get().version().string(),
                        baseTrainingNumAllowedPerLevel);
    }

    void handleSkseMessage(SKSE::MessagingInterface::Message* message) {
        if (message->type == SKSE::MessagingInterface::kDataLoaded) {
            initialize();
        }
    }
}

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* skse) {
    if (skse->IsEditor()) {
        return false;
    }
    SKSE::Init(skse);
    const auto* messaging = SKSE::GetMessagingInterface();
    if (!messaging || !messaging->RegisterListener(Training::handleSkseMessage)) {
        SKSE::log::error("Cannot register SKSE handlers");
        return false;
    }
    return true;
}
