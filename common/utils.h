#ifndef EMPYREAN_COMMON_UTILS_H_
#define EMPYREAN_COMMON_UTILS_H_

#include "../components.h"
#include <string>

namespace empyrean
{
    inline std::string GetMenuId()
    {
        if (!local_player)
            return "EmpyreanErrorId";
        return std::string(local_player->get_char_name()) + "Empyrean";
    }

    inline std::string GetMenuName()
    {
        if (!local_player)
            return "Empyrean Error";
        return std::string(local_player->get_char_name()) + " Empyrean";
    }
}

#endif // EMPYREAN_COMMON_UTILS_H_
