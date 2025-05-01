#include "lucian.h"
#include "../../components.h"
#include "../../common/utils.h"
#include "../../common/nearest_enemy_tracker.h"
#include "../../common/spell_lock_manager.h"


namespace empyrean
{
    std::unique_ptr<Lucian> Lucian::instance = nullptr;

    void Lucian::Init()
    {
        if (!instance)
        {
            instance.reset(new Lucian());
        }
    }

    Lucian::Lucian()
    {
        CreateMenu();

        NearestEnemyTracker::GetInstance();
        SpellLockManager::GetInstance();

        event_manager->add_on_draw(OnDraw);
        event_manager->add_on_game_update(OnGameUpdate);
    }
} // namespace empyrean