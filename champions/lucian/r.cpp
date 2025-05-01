#include "r.h"
#include "../../common/nearest_enemy_tracker.h"
#include "../../common/line_segment.h"

namespace empyrean
{
    Lucian::RHandler::CastData Lucian::RHandler::cast_data_ = Lucian::RHandler::CastData();
    std::unique_ptr<Lucian::RHandler> Lucian::RHandler::instance = nullptr;

    Lucian::RHandler *Lucian::RHandler::GetInstance()
    {
        if (!instance)
        {
            instance = std::unique_ptr<RHandler>(new RHandler());
        }
        return instance.get();
    }

    Lucian::RHandler::RHandler()
    {
        api->set_on_create_object(OnCreateObject);
    }
    bool Lucian::RHandler::InvokeSemiR()
    {
        if (HasBuff() || local_player->get_spellbook()->can_use_spell(SDK::Enums::SpellSlot::R) != SDK::Enums::SpellState::Ready)
        {
            return false;
        }


        const auto &target = NearestEnemyTracker::GetInstance()->GetTarget();
        if (!target)
        {
            return false;
        }

        const auto pred_result = prediction->get_prediction(GetInitialPredInput(target));
        if (pred_result.cast_rate == Prediction::CastRate::Unknown)
        {
            return false;
        }
        auto position = pred_result.cast_position;
        const auto cast_result = local_player->get_spellbook()->cast_spell(SDK::Enums::SpellSlot::R, position);

        if (!cast_result)
        {
            return false;
        }

        return true;
    }

    bool Lucian::RHandler::InvokeAutoFollow()
    {
        if (!HasBuff())
        {
            return false;
        }

        const auto targets = ts->get_targets(1500.0f);

        for (const auto &target : targets)
        {
            const auto pred_result = prediction->get_prediction(GetCheckPredInput(target));
            if (pred_result.cast_rate == Prediction::CastRate::Unknown)
            {
                continue;
            }
            const auto player_pos = local_player->get_position();
            const auto end_pos = player_pos + cast_data_.direction * (kRange + target->get_bounding_radius());
            const auto direction = (end_pos - player_pos).normalize();
            const auto extended_start_pos = player_pos - (direction * 2.0f);
            const auto extended_end_pos = end_pos + (direction * 2.0f);
            const auto seg = LineSegment(extended_start_pos, extended_end_pos);
            const auto dist = seg.DistanceTo(target->get_position());
            if (dist > kWidth + target->get_bounding_radius())
            {
                continue;
            }

            // Calculate perpendicular vector to r direction
            const auto perpendicular = SDK::Structs::Math::Vector3(cast_data_.direction.z, 0, -cast_data_.direction.x);
            // Calculate horizontal positions on either side
            const auto pos1 = player_pos + perpendicular * dist;
            const auto pos2 = player_pos - perpendicular * dist;
            // Choose position closer to target
            const auto adjust_pos = pos1.Distance2D(target->get_position()) > pos2.Distance2D(target->get_position()) ? pos2 : pos1;
            auto move_pos = adjust_pos;

            // Further adjust if we're close to target's hitbox
            if (dist <= target->get_bounding_radius() + kWidth / 2)
            {
                const SDK::Structs::Math::Vector3 mouse_pos = *api->get_hud_manager()->get_cursor_logic()->get_cursor_position();
                const auto ver_dist = sqrt(pow(target->get_bounding_radius() + kWidth / 2, 2) - pow(dist, 2));
                const auto end_pos2 = local_player->get_position() - cast_data_.direction * (kRange + target->get_bounding_radius());
                const float dir = mouse_pos.Distance2D(end_pos) < mouse_pos.Distance2D(end_pos2) ? 1.0f : -1.0f;
                move_pos = adjust_pos + cast_data_.direction * (ver_dist * dir);
            }

            api->get_hud_manager()->get_cursor_logic()->right_click_move(&move_pos, false);
            return true;
        }

        return false;
    }

    void Lucian::RHandler::OnCreateObject(GameObject *object)
    {
        if (object->get_name() != kMissileName)
        {
            return;
        }

        Missile *missile = reinterpret_cast<Missile *>(object->as_missile());
        if (!missile)
        {
            return;
        }

        auto movement = missile->get_movement();
        if (!movement)
        {
            return;
        }

        cast_data_.direction = (movement->get_target_position() - movement->get_start_position()).normalize();
        cast_data_.cast_time = api->get_riot_clock()->get_time();
    }
} // namespace empyrean
