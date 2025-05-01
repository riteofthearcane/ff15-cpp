#include "lucian.h"
#include "../../components.h"
#include "r.h"
#include "e.h"
#include "common/spell_lock_manager.h"

namespace empyrean
{
    void Lucian::OnGameUpdate()
    {
        if (menu->get<bool>(kMenuIdQueueE))
        {
            const auto mouse_pos = *api->get_hud_manager()->get_cursor_logic()->get_cursor_position();
            Lucian::EHandler::GetInstance()->Queue(std::move(mouse_pos));
        }

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

        if (Lucian::EHandler::GetInstance()->InvokeQueued())
        {
            return;
        }
    }
}
