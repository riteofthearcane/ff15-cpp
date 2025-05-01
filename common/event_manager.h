#ifndef EMPYREAN_COMMON_EVENT_MANAGER_H_
#define EMPYREAN_COMMON_EVENT_MANAGER_H_

#include <utility>
#include <memory>
#include <vector>
#include "../api/api_public.h"

namespace empyrean
{
    class EventManager
    {
    public:
        static EventManager* GetInstance();

        EventManager(const EventManager&) = delete;
        EventManager& operator=(const EventManager&) = delete;

        void add_on_wnd_proc(void *fn);
        void add_on_draw_world(void *fn);
        void add_on_draw(void *fn);
        void add_on_game_update(void *fn);
        void add_on_delete_object(void *fn);
        void add_on_create_object(void *fn);
        void add_on_process_spell(void *fn);
        void add_on_basic_attack(void *fn);
        void add_on_stop_spell(void *fn);
        void add_on_play_animation(void *fn);
        void add_on_buff_add(void *fn);
        void add_on_buff_update_count(void *fn);
        void add_on_buff_remove(void *fn);
        void add_on_new_path(void *fn);
        void add_on_try_fire_missile(void *fn);
        void add_on_issue_order(void *fn);
        void add_on_terrain_change(void *fn);
        void add_on_update_chargeable_spell(void *fn);
        void add_on_execute_cast_frame(void *fn);
        void add_on_force_stop_spell_instance_client(void *fn);
        void add_on_spellbook_cast_spell(void *fn);
        void add_orbwalker_on_can_not_lasthit(void *fn);
        void add_orbwalker_on_before_attack(void *fn);
        void add_orbwalker_on_movement(void *fn);
        void add_orbwalker_on_reset_auto_attack(void *fn);
        void add_orbwalker_on_pre_update(void *fn);
        void add_orbwalker_on_after_attack(void *fn);

    private:
        EventManager();

        static void on_wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
        static void on_draw_world();
        static void on_draw();
        static void on_game_update();
        static void on_delete_object(GameObject *sender);
        static void on_create_object(GameObject *sender, uint32_t network_id);
        static void on_process_spell(AIBaseCommon *sender, SpellCastInfo *spell_cast_info);
        static void on_basic_attack(AIBaseCommon *sender, SpellCastInfo *spell_cast_info);
        static void on_stop_spell(AIBaseCommon *sender, SpellCastInfo *spell_cast_info, bool keep_animation, bool has_been_cast, bool destroy_missile, uint32_t missile_id);
        static void on_play_animation(AIBaseCommon *sender, const std::string& animation_name);
        static void on_buff_add(AIBaseCommon *sender, BuffInstance *buff);
        static void on_buff_update_count(AIBaseCommon *sender, BuffInstance *buff, uint8_t count);
        static void on_buff_remove(AIBaseCommon *sender, BuffInstance *buff);
        static void on_new_path(AIBaseCommon *sender, const std::vector<SDK::Structs::Math::Vector3>& paths, uint32_t path_count, float speed);
        static void on_try_fire_missile(AIBaseCommon *sender, SpellCastInfo *spell_cast_info, const SDK::Structs::Math::Vector3* start_position, float start_cast_time);
        static void on_issue_order(const SDK::Structs::Math::Vector3& position, int order, int target_id, const char *plugin_name);
        static void on_terrain_change(const std::string& name, int hash, int team);
        static void on_update_chargeable_spell(AIBaseCommon *sender, int slot, const SDK::Structs::Math::Vector3* position, bool release_cast);
        static void on_execute_cast_frame(AIBaseCommon *sender, SpellCastInfo *spell_cast_info);
        static void on_force_stop_spell_instance_client(AIBaseCommon *sender, SpellCastInfo *spell_cast_info, bool keep_animation, bool force, bool destroy_missile, uint32_t missile_id);
        static void on_spellbook_cast_spell(int slot, const SDK::Structs::Math::Vector3& start_position, const SDK::Structs::Math::Vector3& end_position, int target_id);

        static void orbwalker_on_can_not_lasthit(AIBaseCommon *sender);
        static void orbwalker_on_before_attack(AIBaseCommon *sender);
        static void orbwalker_on_movement(const SDK::Structs::Math::Vector3& position);
        static void orbwalker_on_reset_auto_attack();
        static void orbwalker_on_pre_update();
        static void orbwalker_on_after_attack(AIBaseCommon *sender);

        static std::vector<void *> on_wnd_proc_callbacks;
        static std::vector<void *> on_draw_world_callbacks;
        static std::vector<void *> on_draw_callbacks;
        static std::vector<void *> on_game_update_callbacks;
        static std::vector<void *> on_delete_object_callbacks;
        static std::vector<void *> on_create_object_callbacks;
        static std::vector<void *> on_process_spell_callbacks;
        static std::vector<void *> on_basic_attack_callbacks;
        static std::vector<void *> on_stop_spell_callbacks;
        static std::vector<void *> on_play_animation_callbacks;
        static std::vector<void *> on_buff_add_callbacks;
        static std::vector<void *> on_buff_update_count_callbacks;
        static std::vector<void *> on_buff_remove_callbacks;
        static std::vector<void *> on_new_path_callbacks;
        static std::vector<void *> on_try_fire_missile_callbacks;
        static std::vector<void *> on_issue_order_callbacks;
        static std::vector<void *> on_terrain_change_callbacks;
        static std::vector<void *> on_update_chargeable_spell_callbacks;
        static std::vector<void *> on_execute_cast_frame_callbacks;
        static std::vector<void *> on_force_stop_spell_instance_client_callbacks;
        static std::vector<void *> on_spellbook_cast_spell_callbacks;

        static std::vector<void *> orbwalker_on_can_not_lasthit_callbacks;
        static std::vector<void *> orbwalker_on_before_attack_callbacks;
        static std::vector<void *> orbwalker_on_movement_callbacks;
        static std::vector<void *> orbwalker_on_reset_auto_attack_callbacks;
        static std::vector<void *> orbwalker_on_pre_update_callbacks;
        static std::vector<void *> orbwalker_on_after_attack_callbacks;

        static std::unique_ptr<EventManager> instance;

        template <typename... Args>
        static void execute(void *fn, Args... args)
        {
            return reinterpret_cast<void(__stdcall *)(Args...)>(fn)(std::forward<Args>(args)...);
        }
    };
} // namespace empyrean

#endif // EMPYREAN_COMMON_EVENT_MANAGER_H_