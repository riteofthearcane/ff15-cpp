#ifndef EMPYREAN_COMPONENTS_H_
#define EMPYREAN_COMPONENTS_H_

#include "API/api_public.h"
#include "API/plugins/public_plugin_loader_api.h"
#include "API/plugins/public_orbwalker_api.h"
#include "API/plugins/public_prediction_api.h"
#include "API/plugins/public_menu_api.h"
#include "API/plugins/public_target_selector_api.h"
#include "common/event_manager.h"

inline SurrenderAt15 *api;
inline PluginLoader::PluginLoaderApi *plugin_loader;
inline Orbwalker::OrbwalkerApi *orbwalker;
inline Prediction::PredictionAPI *prediction;
inline TargetSelector::TargetSelectorApi *ts;
inline Menu::AMenuConstructor *menu_api;
inline std::unique_ptr<AIHeroPlayer> local_player;
inline empyrean::EventManager *event_manager;

inline Menu::AMenu *menu;

#endif // EMPYREAN_COMPONENTS_H_