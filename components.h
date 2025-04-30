#pragma once

#include <API/api_public.h>
#include <API/plugins/public_plugin_loader_api.h>
#include <API/plugins/public_orbwalker_api.h>
#include <API/plugins/public_prediction_api.h>
#include <API/plugins/public_menu_api.h>

inline SurrenderAt15* api;
inline PluginLoader::PluginLoaderApi* plugin_loader;
inline Orbwalker::OrbwalkerApi* orbwalker;
inline Prediction::PredictionAPI* prediction;
inline Menu::AMenuConstructor* menu_api;
inline std::unique_ptr<AIHeroPlayer> local_player;

inline Menu::AMenu* menu;