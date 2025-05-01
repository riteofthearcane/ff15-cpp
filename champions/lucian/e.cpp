#include "e.h"

namespace empyrean
{
    std::unique_ptr<Lucian::EHandler> Lucian::EHandler::instance = nullptr;
    std::optional<Lucian::EHandler::QueueData> Lucian::EHandler::queued_data_ = std::nullopt;
    std::float_t Lucian::EHandler::last_basic_attack_time_ = 0.0f;

    Lucian::EHandler *Lucian::EHandler::GetInstance()
    {
        if (!instance)
        {
            instance = std::unique_ptr<EHandler>(new EHandler());
        }
        return instance.get();
    }

    Lucian::EHandler::EHandler()
    {
        event_manager->add_on_game_update(OnGameUpdate);
        event_manager->add_on_draw(OnDraw);
        event_manager->add_on_basic_attack(OnBasicAttack);
    }

    void Lucian::EHandler::OnGameUpdate()
    {
        if (!queued_data_)
        {
            return;
        }

        if (api->get_riot_clock()->get_time() > queued_data_->expire_time)
        {
            queued_data_ = std::nullopt;
        }
    }

    void Lucian::EHandler::OnDraw()
    {
        if (!queued_data_)
        {
            return;
        }
        auto start_pos = api->get_render_helper()->world_to_screen(local_player->get_position());
        const auto dash_pos = local_player->get_position() + queued_data_->direction * kMinimumDashDist;
        auto end_pos = api->get_render_helper()->world_to_screen(dash_pos);

        api->get_imgui_helper()->draw_line(start_pos, end_pos, 0xFF0000FF);
        api->get_imgui_helper()->draw_filled_circle(end_pos, 10, 0xFF0000FF);
    }

    void Lucian::EHandler::OnBasicAttack(AIBaseCommon *sender, SpellCastInfo *spell_cast_info)
    {
        if (sender->get_network_id() != local_player->get_network_id())
        {
            return;
        }

        last_basic_attack_time_ = api->get_riot_clock()->get_time() + local_player->get_attack_cast_delay();
    }

    void Lucian::EHandler::Queue(const SDK::Structs::Math::Vector3 &position)
    {
        const auto direction = (position - local_player->get_position()).normalize();
        queued_data_ = QueueData{std::move(direction), api->get_riot_clock()->get_time() + kQueueExpireTime};
    }

    bool Lucian::EHandler::InvokeQueued()
    {
        if (!IsQueued())
        {
            return false;
        }

        if (api->get_riot_clock()->get_time() - last_basic_attack_time_ > 0.15f)
        {
            return false;
        }

        auto cast_pos = local_player->get_position() + queued_data_->direction * kMinimumDashDist;
        const auto cast_result = local_player->get_spellbook()->cast_spell(SDK::Enums::SpellSlot::E, cast_pos);
        if (!cast_result)
        {
            return false;
        }

        queued_data_ = std::nullopt;
        return true;
    }

} // namespace empyrean