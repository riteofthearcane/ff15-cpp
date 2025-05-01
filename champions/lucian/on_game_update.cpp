#include "lucian.h"
#include "../../components.h"
#include "r.h"
#include "common/spell_lock_manager.h"

namespace empyrean
{
    void Lucian::OnGameUpdate()
    {
        if (Lucian::RHandler::GetInstance()->InvokeAutoFollow())
        {
            orbwalker->block_move(true);
            return;
        }

        orbwalker->block_move(false);

        if (!SpellLockManager::GetInstance()->ShouldCast())
        {
            return;
        }

        if (menu->get<bool>(kMenuIdSemiR) && Lucian::RHandler::GetInstance()->InvokeSemiR())
        {
            return;
        }
    }
}
