#include "lucian.h"
#include "../../components.h"
#include "r.h"

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
        
        if (menu->get<bool>(kMenuIdSemiR) && Lucian::RHandler::GetInstance()->InvokeSemiR())
        {
            return;
        }
    }
}
