#ifndef EMPYREAN_COMMON_SPELL_LOCK_MANAGER_H_
#define EMPYREAN_COMMON_SPELL_LOCK_MANAGER_H_

#include <API/api_public.h>
#include <components.h>
#include <unordered_map>
#include <memory>
#include <vector>
#include <cstdint>
#include <string>

namespace empyrean
{
    class SpellLockManager
    {
    public:
        static SpellLockManager *GetInstance();

        // Delete copy/move constructors and assignment operators
        SpellLockManager(const SpellLockManager &) = delete;
        SpellLockManager &operator=(const SpellLockManager &) = delete;
        SpellLockManager(SpellLockManager &&) = delete;
        SpellLockManager &operator=(SpellLockManager &&) = delete;

        // Check if a specific spell slot can be cast (not locked)
        static bool ShouldCastSpell(int slot);

        // Check if any spell is currently locked
        static bool ShouldCast();

    private:
        // The buffer time to account for latency and other factors.
        static constexpr float DELAY_BUFFER = 0.05f;

        SpellLockManager();

        static void OnProcessSpell(AIBaseCommon *sender, SpellCastInfo *spell_cast_info);
        static void OnExecuteCastFrame(AIBaseCommon *sender, SpellCastInfo *spell_cast_info);

        static std::unique_ptr<SpellLockManager> instance;

        // Map: Spell Hash -> Lock Expiration Time
        static std::unordered_map<std::string, float> invokes_;
    };

} // namespace empyrean

#endif // EMPYREAN_COMMON_SPELL_LOCK_MANAGER_H_