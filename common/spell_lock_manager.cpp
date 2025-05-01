#include "spell_lock_manager.h"
#include <stdexcept>
#include <string> // For std::to_string (optional)

namespace empyrean
{
    std::unique_ptr<SpellLockManager> SpellLockManager::instance = nullptr;
    std::unordered_map<std::string, float> SpellLockManager::invokes_{};

    SpellLockManager *SpellLockManager::GetInstance()
    {
        if (!instance)
        {
            instance = std::unique_ptr<SpellLockManager>(new SpellLockManager());
        }
        return instance.get();
    }

    SpellLockManager::SpellLockManager()
    {
        event_manager->add_on_process_spell(OnProcessSpell);
        event_manager->add_on_execute_cast_frame(OnExecuteCastFrame);
        event_manager->add_on_game_update(OnGameUpdate);
    }

    void SpellLockManager::OnProcessSpell(AIBaseCommon *sender, SpellCastInfo *spell_cast_info)
    {
        if (sender->get_network_id() != local_player->get_network_id())
        {
            return;
        }

        const auto spell_data_ptr = spell_cast_info->get_spell_data();

        const float cast_delay = spell_cast_info->get_designer_cast_time();
        const float current_time = api->get_riot_clock()->get_time();
        const float expiration_time = current_time + cast_delay + DELAY_BUFFER;

        // Update the static map
        invokes_[spell_data_ptr->get_name()] = expiration_time;
    }

    void SpellLockManager::OnExecuteCastFrame(AIBaseCommon *sender, SpellCastInfo *spell_cast_info)
    {

        // Check if the spell is actually avaiable upon callback
        const auto slot = spell_cast_info->get_spell_slot();
        if (!local_player->get_spellbook()->can_use_spell(slot))
        {
            return;
        }

        // Block until the latest time the spell should be cast
        const float latency = static_cast<float>(api->get_network_client()->get_ping()) / 1000.0f;
        const float current_time = api->get_riot_clock()->get_time();
        invokes_[spell_cast_info->get_spell_data()->get_name()] = current_time + latency + DELAY_BUFFER;
    }

    void SpellLockManager::OnGameUpdate()
    {
        // Expire spells that are no longer valid
        for (auto it = invokes_.begin(); it != invokes_.end();)
        {
            if (it->second < api->get_riot_clock()->get_time())
            {
                it = invokes_.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }




    bool SpellLockManager::ShouldCastSpell(int slot)
    {

        const auto spell_name = local_player->get_spellbook()->get_spell(slot)->get_spell_data()->get_name();
        return invokes_.find(spell_name) == invokes_.end();
    }

    bool SpellLockManager::ShouldCast()
    {
        return invokes_.empty();
    }

} // namespace empyrean