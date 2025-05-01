#ifndef EMPYREAN_CHAMPIONS_LUCIAN_R_H_
#define EMPYREAN_CHAMPIONS_LUCIAN_R_H_

#include "../../components.h"
#include "lucian.h"

namespace empyrean
{
    class Lucian::RHandler
    {
    public:
        static Lucian::RHandler *GetInstance();

        RHandler(const RHandler &) = delete;
        RHandler &operator=(const RHandler &) = delete;

        static bool InvokeSemiR();

        // Returns true if the auto-follow was invoked
        static bool InvokeAutoFollow();

    private:
        struct CastData
        {
            SDK::Structs::Math::Vector3 direction;
            float cast_time;
        };

        RHandler();

        static bool HasBuff()
        {
            const auto buff = local_player->get_buff_manager()->has_buff("LucianR");
            return buff && buff->get_remaining_time() > 0.0f;
        }

        static void OnCreateObject(GameObject *object);

        static CastData cast_data_;
        static std::unique_ptr<RHandler> instance;

        static constexpr float kRange = 1150.0f;
        static constexpr float kWidth = 285.0f;

        static Prediction::PredictionInput GetInitialPredInput(AIBaseCommon *target)
        {
            Prediction::PredictionInput input;
            input.target_object = target;
            input.spell_type = Prediction::SpellType::Linear;
            input.width = -150.0f;
            input.range = kRange;
            input.speed = 2800.0f;
            input.delay = 0.0f;
            input.collision_flags = Prediction::CollisionFlag::Heroes | Prediction::CollisionFlag::Minions | Prediction::CollisionFlag::WindWall;
            return input;
        }

        static Prediction::PredictionInput GetCheckPredInput(AIBaseCommon *target)
        {
            Prediction::PredictionInput input;
            input.target_object = target;
            input.spell_type = Prediction::SpellType::Linear;
            input.width = kWidth;
            input.range = kRange;
            input.speed = 2800.0f;
            input.delay = 0.0f;
            input.collision_flags = Prediction::CollisionFlag::Heroes | Prediction::CollisionFlag::Minions | Prediction::CollisionFlag::WindWall;
            return input;
        }

        static constexpr std::string_view kMissileName = "LucianRMissile";
    };
} // namespace empyrean

#endif // EMPYREAN_CHAMPIONS_LUCIAN_R_H_