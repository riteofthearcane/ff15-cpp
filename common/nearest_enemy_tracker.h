#ifndef EMPYREAN_COMMON_NEAREST_ENEMY_TRACKER_H_
#define EMPYREAN_COMMON_NEAREST_ENEMY_TRACKER_H_

#include "../components.h"
#include <memory>

namespace empyrean
{
    class NearestEnemyTracker
    {
    public:
        static NearestEnemyTracker *GetInstance();

        NearestEnemyTracker(const NearestEnemyTracker &) = delete;
        NearestEnemyTracker &operator=(const NearestEnemyTracker &) = delete;

        static AIHeroPlayer *GetTarget();
        static bool IsTarget(AIHeroPlayer *target);

    private:
        NearestEnemyTracker();
        static void OnGameUpdate();

        static AIHeroPlayer *target_;
        static std::unique_ptr<NearestEnemyTracker> instance;
    };

} // namespace empyrean

#endif // EMPYREAN_CDOMMON_NEAREST_ENEMY_TRACKER_H_