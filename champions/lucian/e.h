#ifndef EMPYREAN_CHAMPIONS_LUCIAN_E_H_
#define EMPYREAN_CHAMPIONS_LUCIAN_E_H_

#include "../../components.h"
#include "lucian.h"
#include <optional>

namespace empyrean
{
    class Lucian::EHandler
    {
    public:
        static EHandler *GetInstance();

        EHandler(const EHandler &) = delete;
        EHandler &operator=(const EHandler &) = delete;

        static void Queue(const SDK::Structs::Math::Vector3 &position);
        static bool IsQueued() { return queued_data_.has_value(); }
        static bool InvokeQueued();

    private:
        EHandler();

        static void OnGameUpdate();
        static void OnDraw();
        static void OnBasicAttack(AIBaseCommon *sender, SpellCastInfo *spell_cast_info);

        struct QueueData
        {
            SDK::Structs::Math::Vector3 direction;
            float expire_time;
        };

        static std::optional<QueueData> queued_data_;
        static std::float_t last_basic_attack_time_;
        static std::unique_ptr<EHandler> instance;

        static constexpr float kQueueExpireTime = 0.5f;
        static constexpr float kMinimumDashDist = 200.0f;
    };
}

#endif