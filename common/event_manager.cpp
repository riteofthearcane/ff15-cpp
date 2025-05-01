#include "event_manager.h"
#include "components.h"

namespace empyrean
{
    std::unique_ptr<EventManager> EventManager::instance = nullptr;

    // Initialize all static vectors
    std::vector<void*> EventManager::on_wnd_proc_callbacks;
    std::vector<void*> EventManager::on_draw_world_callbacks;
    std::vector<void*> EventManager::on_draw_callbacks;
    std::vector<void*> EventManager::on_game_update_callbacks;
    std::vector<void*> EventManager::on_delete_object_callbacks;
    std::vector<void*> EventManager::on_create_object_callbacks;
    std::vector<void*> EventManager::on_process_spell_callbacks;
    std::vector<void*> EventManager::on_basic_attack_callbacks;
    std::vector<void*> EventManager::on_stop_spell_callbacks;
    std::vector<void*> EventManager::on_play_animation_callbacks;
    std::vector<void*> EventManager::on_buff_add_callbacks;
    std::vector<void*> EventManager::on_buff_update_count_callbacks;
    std::vector<void*> EventManager::on_buff_remove_callbacks;
    std::vector<void*> EventManager::on_new_path_callbacks;
    std::vector<void*> EventManager::on_try_fire_missile_callbacks;
    std::vector<void*> EventManager::on_issue_order_callbacks;
    std::vector<void*> EventManager::on_terrain_change_callbacks;
    std::vector<void*> EventManager::on_update_chargeable_spell_callbacks;
    std::vector<void*> EventManager::on_execute_cast_frame_callbacks;
    std::vector<void*> EventManager::on_force_stop_spell_instance_client_callbacks;
    std::vector<void*> EventManager::on_spellbook_cast_spell_callbacks;
    std::vector<void*> EventManager::orbwalker_on_can_not_lasthit_callbacks;
    std::vector<void*> EventManager::orbwalker_on_before_attack_callbacks;
    std::vector<void*> EventManager::orbwalker_on_movement_callbacks;
    std::vector<void*> EventManager::orbwalker_on_reset_auto_attack_callbacks;
    std::vector<void*> EventManager::orbwalker_on_pre_update_callbacks;
    std::vector<void*> EventManager::orbwalker_on_after_attack_callbacks;

    EventManager* EventManager::GetInstance()
    {
        if (!instance)
        {
            instance.reset(new EventManager());
        }
        return instance.get();
    }

    EventManager::EventManager()
    {
        api->set_on_wnd_proc(on_wnd_proc);
        api->set_on_draw_world(on_draw_world);
        api->set_on_draw(on_draw);
        api->set_on_game_update(on_game_update);
        api->set_on_delete_object(on_delete_object);
        api->set_on_create_object(on_create_object);
        api->set_on_process_spell(on_process_spell);
        api->set_on_basic_attack(on_basic_attack);
        api->set_on_stop_spell(on_stop_spell);
        api->set_on_play_animation(on_play_animation);
        api->set_on_buff_add(on_buff_add);
        api->set_on_buff_update_count(on_buff_update_count);
        api->set_on_buff_remove(on_buff_remove);
        api->set_on_new_path(on_new_path);
        api->set_on_try_fire_missile(on_try_fire_missile);
        api->set_on_issue_order(on_issue_order);
        api->set_on_terrain_change(on_terrain_change);
        api->set_on_update_chargeable_spell(on_update_chargeable_spell);
        api->set_on_execute_cast_frame(on_execute_cast_frame);
        api->set_on_force_stop_spell_instance_client(on_force_stop_spell_instance_client);
        api->set_on_spellbook_cast_spell(on_spellbook_cast_spell);

        orbwalker->set_on_can_not_lasthit(orbwalker_on_can_not_lasthit);
        orbwalker->set_on_before_attack(orbwalker_on_before_attack);
        orbwalker->set_on_movement(orbwalker_on_movement);
        orbwalker->set_on_reset_auto_attack(orbwalker_on_reset_auto_attack);
        orbwalker->set_on_pre_update(orbwalker_on_pre_update);
        orbwalker->set_on_after_attack(orbwalker_on_after_attack);
    }

    void EventManager::on_wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        for (const auto fn : on_wnd_proc_callbacks)
            execute(fn, hwnd, msg, wparam, lparam);
    }

    void EventManager::on_draw_world()
    {
        for (const auto fn : on_draw_world_callbacks)
            execute(fn);
    }

    void EventManager::on_draw()
    {
        for (const auto fn : on_draw_callbacks)
            execute(fn);
    }

    void EventManager::on_game_update()
    {
        for (const auto fn : on_game_update_callbacks)
            execute(fn);
    }

    void EventManager::on_delete_object(GameObject *sender)
    {
        for (const auto fn : on_delete_object_callbacks)
            execute(fn, sender);
    }

    void EventManager::on_create_object(GameObject *sender, uint32_t network_id)
    {
        for (const auto fn : on_create_object_callbacks)
            execute(fn, sender, network_id);
    }

    void EventManager::on_process_spell(AIBaseCommon *sender, SpellCastInfo *spell_cast_info)
    {
        for (const auto fn : on_process_spell_callbacks)
            execute(fn, sender, spell_cast_info);
    }

    void EventManager::on_basic_attack(AIBaseCommon *sender, SpellCastInfo *spell_cast_info)
    {
        for (const auto fn : on_basic_attack_callbacks)
            execute(fn, sender, spell_cast_info);
    }

    void EventManager::on_stop_spell(AIBaseCommon *sender, SpellCastInfo *spell_cast_info, bool keep_animation, bool has_been_cast, bool destroy_missile, uint32_t missile_id)
    {
        for (const auto fn : on_stop_spell_callbacks)
            execute(fn, sender, spell_cast_info, keep_animation, has_been_cast, destroy_missile, missile_id);
    }

    void EventManager::on_play_animation(AIBaseCommon* sender, const std::string& animation_name)
    {
        for (const auto& fn : on_play_animation_callbacks)
            execute(fn, sender, animation_name);
    }

    void EventManager::on_buff_add(AIBaseCommon *sender, BuffInstance *buff)
    {
        for (const auto fn : on_buff_add_callbacks)
            execute(fn, sender, buff);
    }

    void EventManager::on_buff_update_count(AIBaseCommon *sender, BuffInstance *buff, uint8_t count)
    {
        for (const auto fn : on_buff_update_count_callbacks)
            execute(fn, sender, buff, count);
    }

    void EventManager::on_buff_remove(AIBaseCommon *sender, BuffInstance *buff)
    {
        for (const auto fn : on_buff_remove_callbacks)
            execute(fn, sender, buff);
    }

    void EventManager::on_new_path(AIBaseCommon* sender, const std::vector<SDK::Structs::Math::Vector3>& paths, uint32_t path_count, float speed)
    {
        for (const auto& fn : on_new_path_callbacks)
            execute(fn, sender, paths, path_count, speed);
    }

    void EventManager::on_try_fire_missile(AIBaseCommon* sender, SpellCastInfo* spell_cast_info, const SDK::Structs::Math::Vector3* start_position, float start_cast_time)
    {
        for (const auto& fn : on_try_fire_missile_callbacks)
            execute(fn, sender, spell_cast_info, start_position, start_cast_time);
    }

    void EventManager::on_issue_order(const SDK::Structs::Math::Vector3& position, int order, int target_id, const char* plugin_name)
    {
        for (const auto& fn : on_issue_order_callbacks)
            execute(fn, position, order, target_id, plugin_name);
    }

    void EventManager::on_terrain_change(const std::string& name, int hash, int team)
    {
        for (const auto& fn : on_terrain_change_callbacks)
            execute(fn, name, hash, team);
    }

    void EventManager::on_update_chargeable_spell(AIBaseCommon* sender, int slot, const SDK::Structs::Math::Vector3* position, bool release_cast)
    {
        for (const auto& fn : on_update_chargeable_spell_callbacks)
            execute(fn, sender, slot, position, release_cast);
    }

    void EventManager::on_execute_cast_frame(AIBaseCommon *sender, SpellCastInfo *spell_cast_info)
    {
        for (const auto fn : on_execute_cast_frame_callbacks)
            execute(fn, sender, spell_cast_info);
    }

    void EventManager::on_force_stop_spell_instance_client(AIBaseCommon *sender, SpellCastInfo *spell_cast_info, bool keep_animation, bool force, bool destroy_missile, uint32_t missile_id)
    {
        for (const auto fn : on_force_stop_spell_instance_client_callbacks)
            execute(fn, sender, spell_cast_info, keep_animation, force, destroy_missile, missile_id);
    }

    void EventManager::on_spellbook_cast_spell(int slot, const SDK::Structs::Math::Vector3& start_position, const SDK::Structs::Math::Vector3& end_position, int target_id)
    {
        for (const auto& fn : on_spellbook_cast_spell_callbacks)
            execute(fn, slot, start_position, end_position, target_id);
    }

    void EventManager::orbwalker_on_can_not_lasthit(AIBaseCommon *sender)
    {
        for (const auto fn : orbwalker_on_can_not_lasthit_callbacks)
            execute(fn, sender);
    }

    void EventManager::orbwalker_on_before_attack(AIBaseCommon *sender)
    {
        for (const auto fn : orbwalker_on_before_attack_callbacks)
            execute(fn, sender);
    }

    void EventManager::orbwalker_on_movement(const SDK::Structs::Math::Vector3& position)
    {
        for (const auto& fn : orbwalker_on_movement_callbacks)
            execute(fn, position);
    }

    void EventManager::orbwalker_on_reset_auto_attack()
    {
        for (const auto fn : orbwalker_on_reset_auto_attack_callbacks)
            execute(fn);
    }

    void EventManager::orbwalker_on_pre_update()
    {
        for (const auto fn : orbwalker_on_pre_update_callbacks)
            execute(fn);
    }

    void EventManager::orbwalker_on_after_attack(AIBaseCommon *sender)
    {
        for (const auto fn : orbwalker_on_after_attack_callbacks)
            execute(fn, sender);
    }

    void EventManager::add_on_wnd_proc(void *fn)
    {
        on_wnd_proc_callbacks.push_back(fn);
    }

    void EventManager::add_on_draw_world(void *fn)
    {
        on_draw_world_callbacks.push_back(fn);
    }

    void EventManager::add_on_draw(void *fn)
    {
        on_draw_callbacks.push_back(fn);
    }

    void EventManager::add_on_game_update(void *fn)
    {
        on_game_update_callbacks.push_back(fn);
    }

    void EventManager::add_on_delete_object(void *fn)
    {
        on_delete_object_callbacks.push_back(fn);
    }

    void EventManager::add_on_create_object(void *fn)
    {
        on_create_object_callbacks.push_back(fn);
    }

    void EventManager::add_on_process_spell(void *fn)
    {
        on_process_spell_callbacks.push_back(fn);
    }

    void EventManager::add_on_basic_attack(void *fn)
    {
        on_basic_attack_callbacks.push_back(fn);
    }

    void EventManager::add_on_stop_spell(void *fn)
    {
        on_stop_spell_callbacks.push_back(fn);
    }

    void EventManager::add_on_play_animation(void *fn)
    {
        on_play_animation_callbacks.push_back(fn);
    }

    void EventManager::add_on_buff_add(void *fn)
    {
        on_buff_add_callbacks.push_back(fn);
    }

    void EventManager::add_on_buff_update_count(void *fn)
    {
        on_buff_update_count_callbacks.push_back(fn);
    }

    void EventManager::add_on_buff_remove(void *fn)
    {
        on_buff_remove_callbacks.push_back(fn);
    }

    void EventManager::add_on_new_path(void *fn)
    {
        on_new_path_callbacks.push_back(fn);
    }

    void EventManager::add_on_try_fire_missile(void *fn)
    {
        on_try_fire_missile_callbacks.push_back(fn);
    }

    void EventManager::add_on_issue_order(void *fn)
    {
        on_issue_order_callbacks.push_back(fn);
    }

    void EventManager::add_on_terrain_change(void *fn)
    {
        on_terrain_change_callbacks.push_back(fn);
    }

    void EventManager::add_on_update_chargeable_spell(void *fn)
    {
        on_update_chargeable_spell_callbacks.push_back(fn);
    }

    void EventManager::add_on_execute_cast_frame(void *fn)
    {
        on_execute_cast_frame_callbacks.push_back(fn);
    }

    void EventManager::add_on_force_stop_spell_instance_client(void *fn)
    {
        on_force_stop_spell_instance_client_callbacks.push_back(fn);
    }

    void EventManager::add_on_spellbook_cast_spell(void *fn)
    {
        on_spellbook_cast_spell_callbacks.push_back(fn);
    }

    void EventManager::add_orbwalker_on_can_not_lasthit(void *fn)
    {
        orbwalker_on_can_not_lasthit_callbacks.push_back(fn);
    }

    void EventManager::add_orbwalker_on_before_attack(void *fn)
    {
        orbwalker_on_before_attack_callbacks.push_back(fn);
    }

    void EventManager::add_orbwalker_on_movement(void *fn)
    {
        orbwalker_on_movement_callbacks.push_back(fn);
    }

    void EventManager::add_orbwalker_on_reset_auto_attack(void *fn)
    {
        orbwalker_on_reset_auto_attack_callbacks.push_back(fn);
    }

    void EventManager::add_orbwalker_on_pre_update(void *fn)
    {
        orbwalker_on_pre_update_callbacks.push_back(fn);
    }

    void EventManager::add_orbwalker_on_after_attack(void *fn)
    {
        orbwalker_on_after_attack_callbacks.push_back(fn);
    }
} // namespace empyrean
