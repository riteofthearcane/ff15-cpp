#ifndef EMPYREAN_CHAMPIONS_LUCIAN_MENU_H_
#define EMPYREAN_CHAMPIONS_LUCIAN_MENU_H_

#include "lucian.h"
#include "../../components.h"
#include "../../common/utils.h"

namespace empyrean
{
    void Lucian::CreateMenu()
    {
        menu->add_key(kMenuIdSemiR, "Semi R", 'R');
    }
}

#endif // EMPYREAN_CHAMPIONS_LUCIAN_MENU_H_