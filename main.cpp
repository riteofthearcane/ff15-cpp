#include "components.h"

namespace Empyrean
{
    void on_draw()
    {
        if (local_player->is_dead() || !local_player->is_on_screen())
            return;

        if (menu->get<bool>("Harass.AutoQ"))
        {
            auto position = api->get_render_helper()->world_to_screen(local_player->get_position());
            position.y += 25;
            api->get_imgui_helper()->draw_text(position, 0xFFFFFFFF, "Q Harass");
        }
    }

    void on_draw_world()
    {
        if (local_player->is_dead() || !local_player->is_on_screen())
            return;

        if (menu->get<bool>("Drawings.Q") &&
            (!menu->get<bool>("Drawings.Ready") || local_player->get_spellbook()->can_use_spell(SDK::Enums::SpellSlot::Q) == SDK::Enums::SpellState::Ready))
        {
            auto position = local_player->get_position();
            api->get_imgui_helper()->draw_circle_3d(
                position,
                1135,
                menu->get<SDK::Structs::Math::Vector4>("Drawings.Color").get_color(),
                menu->get<float>("Drawings.Thickness")
            );
        }
    }

    bool try_cast_q(AIBaseCommon* target, bool check_collision = true)
    {
        Prediction::PredictionInput input{};
        input.target_object = target;
        input.delay = .25f;
        input.speed = 2000;
        input.range = 1100;
        input.radius = 120;
        if (check_collision)
            input.collision_flags = Prediction::CollisionFlag::Minions | Prediction::CollisionFlag::Heroes | Prediction::CollisionFlag::WindWall;
        input.spell_type = Prediction::SpellType::Linear;

        const auto pred_output = prediction->get_prediction(input);

        if (!pred_output.target_position.is_bad() && pred_output.hit_chance >= Prediction::HitChance::Low)
        {
            auto position = pred_output.cast_position;
            local_player->get_spellbook()->cast_spell(SDK::Enums::SpellSlot::Q, position);
            return true;
        }

        return false;
    }

    bool try_cast_w(AIBaseCommon* target)
    {
        Prediction::PredictionInput input{};
        input.target_object = target;
        input.delay = .25f;
        input.speed = 1700;
        input.range = 1000;
        input.radius = 120;
        input.spell_type = Prediction::SpellType::Linear;

        const auto pred_output = prediction->get_prediction(input);

        if (!pred_output.target_position.is_bad() && pred_output.hit_chance >= Prediction::HitChance::Low)
        {
            auto position = pred_output.cast_position;
            local_player->get_spellbook()->cast_spell(SDK::Enums::SpellSlot::Q, position);
            return true;
        }

        return false;
    }

    void on_game_update()
    {
        if (orbwalker->get_mode() == Orbwalker::OrbwalkerMode::COMBO)
        {
            const auto target = orbwalker->get_target(1100);

            if (target && target->is_valid())
            {
                if (menu->get<bool>("Combo.Q") && local_player->get_spellbook()->can_use_spell(SDK::Enums::SpellSlot::Q) == SDK::Enums::SpellState::Ready)
                {
                    try_cast_q(target, menu->get<bool>("Combo.CollisionsQ"));
                }

                if (menu->get<bool>("Combo.W") && local_player->get_spellbook()->can_use_spell(SDK::Enums::SpellSlot::W) == SDK::Enums::SpellState::Ready)
                {
                    try_cast_w(target);
                }
            }
        }

        if (menu->get<bool>("Harass.AutoQ") &&
            orbwalker->get_mode() != Orbwalker::OrbwalkerMode::COMBO &&
            orbwalker->get_mode() != Orbwalker::OrbwalkerMode::HARASS &&
            local_player->get_spellbook()->can_use_spell(SDK::Enums::SpellSlot::Q) == SDK::Enums::SpellState::Ready)
        {
            for (const auto& hero : api->get_game_object_manager()->get_heroes())
            {
                if (hero->get_team() == local_player->get_team())
                    continue;

                if (hero->is_dead())
                    continue;

                if (!hero->is_visible())
                    continue;

                if (try_cast_q(hero.get()))
                    break;
            }
        }
    }

    bool on_load(NS SurrenderAt15* ff15)
    {
        api = ff15;

        plugin_loader = api->get_api<PluginLoader::PluginLoaderApi>("PluginLoader");

        plugin_loader->wait_for_dependencies("Empyrean", {{7, "Orbwalker"}, {22, "Prediction"}}, [&]
        {
            menu_api = api->get_api<Menu::AMenuConstructor>("Menu");
            orbwalker = api->get_api<Orbwalker::OrbwalkerApi>("Orbwalker");
            prediction = api->get_api<Prediction::PredictionAPI>("Prediction");

            local_player = std::move(api->get_game_object_manager()->get_local_player());

            menu = menu_api->create_menu("Empyrean", "Champion Plugin", local_player->get_square_texture());

            menu
                ->add_sub_menu("Combo", "Combo")
                ->add_checkbox("Q", "Use Q", true)
                ->add_key_toggle("CollisionsQ", "Disable Q collision check", 'U', false)
                ->add_checkbox("W", "Use W", true)
                ->get_parent()
                ->add_sub_menu("Harass", "Harass")
                ->add_key_toggle("AutoQ", "Auto Q", 'T', true)
                ->get_parent()
                ->add_sub_menu("Drawings", "Drawings")
                ->add_checkbox("Q", "Draw Q", true)
                ->add_checkbox("Ready", "Only draw if ready", true)
                ->add_slider_float("Thickness", "Thickness", 1, .1f, 5.f, .1f)
                ->add_color_edit("Color", "Color", {50, 255, 50, 255});

            api->set_on_draw(on_draw);
            api->set_on_draw_world(on_draw_world);
            api->set_on_game_update(on_game_update);

            api->get_chat_console()->print_chat("[+] Empyrean loaded!");
        });

        return true;
    }

    bool on_unload()
    {
        menu_api->remove_menu("Empyrean");
        api->get_chat_console()->print_chat("[+] Empyrean unloaded!");

        return true;
    }
}

plugin_t PLUGIN = {
    "1.0.0",
    "Empyrean",
    Empyrean::on_load,
    Empyrean::on_unload,
};


BOOL APIENTRY DllMain(const HMODULE hModule, const DWORD dwReason, const LPVOID lpReserved)
{
    return true;
}