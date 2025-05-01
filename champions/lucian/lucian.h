#ifndef EMPYREAN_CHAMPIONS_LUCIAN_LUCIAN_H_
#define EMPYREAN_CHAMPIONS_LUCIAN_LUCIAN_H_

#include "../../components.h"
#include <memory>
#include <string>

namespace empyrean
{
    class Lucian
    {
    public:
        static void Init();

        Lucian(const Lucian &) = delete;
        Lucian &operator=(const Lucian &) = delete;

        static void OnDraw();
        static void OnGameUpdate();
        static bool SemiR();

    private:
        static constexpr char kMenuIdSemiR[] = "SemiRKey";

        Lucian();
        static void CreateMenu();

        static std::unique_ptr<Lucian> instance;

        class RHandler;
    };

} // namespace empyrean

#endif // EMPYREAN_CHAMPIONS_LUCIAN_LUCIAN_H_
