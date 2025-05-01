#include "nearest_enemy_tracker.h"

namespace empyrean
{
    std::unique_ptr<NearestEnemyTracker> NearestEnemyTracker::instance = nullptr;
    AIHeroPlayer *NearestEnemyTracker::target_ = nullptr;

    NearestEnemyTracker *NearestEnemyTracker::GetInstance()
    {
        if (!instance)
        {
            instance.reset(new NearestEnemyTracker());
        }
        return instance.get();
    }

    NearestEnemyTracker::NearestEnemyTracker()
    {
        event_manager->add_on_game_update(OnGameUpdate);
    }

    void NearestEnemyTracker::OnGameUpdate()
    {
        SDK::Structs::Math::Vector3 player_position = local_player->get_position();

        const auto local_team = local_player->get_team();

        float closest_distance = FLT_MAX;
        target_ = nullptr;

        for (const auto &hero : api->get_game_object_manager()->get_heroes())
        {
            // If enemy is not targetable currently, that's fine because they will be in the near future
            if (!hero->is_valid() || hero->get_team() == local_team ||
                hero->is_dead() || !hero->is_visible())
            {
                continue;
            }

            float distance = player_position.Distance2D(hero->get_position());

            if (distance < closest_distance)
            {
                closest_distance = distance;
                target_ = hero.get();
            }
        }
    }

    AIHeroPlayer *NearestEnemyTracker::GetTarget()
    {
        return target_;
    }

    bool NearestEnemyTracker::IsTarget(AIHeroPlayer *target)
    {
        return target_ == target;
    }
}