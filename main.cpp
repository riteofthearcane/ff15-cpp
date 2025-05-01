// Standard library headers (alphabetical order)
#include <unordered_map>

// Project headers (alphabetical order)
#include "common/utils.h"
#include "champions/lucian/lucian.h"
#include "components.h"

namespace empyrean
{

    std::unordered_map<std::string, void (*)()> kSupportedChampions = {
        {"Lucian", Lucian::Init}};

    bool OnLoad(NS SurrenderAt15 *ff15)
    {
        api = ff15;

        plugin_loader = api->get_api<PluginLoader::PluginLoaderApi>("PluginLoader");

        plugin_loader->wait_for_dependencies(
            "Empyrean",
            {{6, "TargetSelector"}, {7, "Orbwalker"}, {22, "Prediction"}},
            [&]()
            {
                menu_api = api->get_api<Menu::AMenuConstructor>("Menu");
                orbwalker = api->get_api<Orbwalker::OrbwalkerApi>("Orbwalker");
                prediction = api->get_api<Prediction::PredictionAPI>("Prediction");
                event_manager = EventManager::GetInstance();

                local_player = std::move(api->get_game_object_manager()->get_local_player());

                if (kSupportedChampions.find(local_player->get_char_name()) ==
                    kSupportedChampions.end())
                {
                    api->get_chat_console()->print_chat("[-] Empyrean is not supported for this champion!");
                    return false;
                }

                menu = menu_api->create_menu(GetMenuId(), GetMenuName(), local_player->get_square_texture());
                ts = api->get_api<TargetSelector::TargetSelectorConstructor>("TargetSelector")->create(menu);

                kSupportedChampions[local_player->get_char_name()]();
                api->get_chat_console()->print_chat("[+] Empyrean loaded!");
            });

        return true;
    }

    bool OnUnload()
    {
        if (kSupportedChampions.find(local_player->get_char_name()) !=
            kSupportedChampions.end())
        {
            menu_api->remove_menu(GetMenuId());
            api->get_chat_console()->print_chat("[+] Empyrean unloaded!");
        }

        return true;
    }

} // namespace empyrean

plugin_t PLUGIN = {
    "1.0.0",
    "Empyrean",
    empyrean::OnLoad,
    empyrean::OnUnload,
};

BOOL APIENTRY DllMain(const HMODULE hModule, const DWORD dwReason, const LPVOID lpReserved)
{
    return true;
}