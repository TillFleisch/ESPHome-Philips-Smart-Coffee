#pragma once
#include <string>

namespace esphome
{
    namespace philips_coffee_machine
    {
#if defined(PHILIPS_COFFEE_LANG_en_US)
#define PHILIPS_COFFEE_LANG_DEFAULT
#elif defined(PHILIPS_COFFEE_LANG_de_DE)
        const std::string state_unknown = "Unbekannt";
        const std::string state_off = "Aus";
        const std::string state_idle = "Bereit";
        const std::string state_cleaning = "Spült";
        const std::string state_preparing = "Vorbereitung";
        const std::string state_water_empty = "Wasser leer";
        const std::string state_waste_warning = "Abfallcontainerwarnung";
        const std::string state_error = "Fehler";
        const std::string state_internal_error = "Interner Fehler";

        const std::string state_ground_coffee_selected = "Vorgemahlener Kaffee ausgewählt";
        const std::string state_coffee_programming_mode = "Kaffee Programmiermodus ausgewählt";
        const std::string state_coffee_selected = "Kaffee ausgewählt";
        const std::string state_coffee_2x_selected = "2x Kaffee ausgewählt";
        const std::string state_coffee_brewing = "Bereitet Kaffee zu";
        const std::string state_coffee_2x_brewing = "Bereitet 2x Kaffee zu";

        const std::string state_ground_espresso_selected = "Vorgemahlener Espresso ausgewählt";
        const std::string state_espresso_programming_mode = "Espresso Programmiermodus ausgewählt";
        const std::string state_espresso_selected = "Espresso ausgewählt";
        const std::string state_espresso_2x_selected = "2x Espresso ausgewählt";
        const std::string state_espresso_brewing = "Bereitet Espresso zu";
        const std::string state_espresso_2x_brewing = "Bereitet 2x Espresso zu";

        const std::string state_ground_americano_selected = "Vorgemahlener Americano ausgewählt";
        const std::string state_americano_programming_mode = "Americano Programmiermodus ausgewählt";
        const std::string state_americano_selected = "Americano ausgewählt";
        const std::string state_americano_2x_selected = "2x Americano ausgewählt";
        const std::string state_americano_brewing = "Bereitet Americano zu";
        const std::string state_americano_2x_brewing = "Bereitet 2x Americano zu";

        const std::string state_ground_cappuccino_selected = "Vorgemahlener Cappuccino ausgewählt";
        const std::string state_cappuccino_programming_mode = "Cappuccino Programmiermodus ausgewählt";
        const std::string state_cappuccino_selected = "Cappuccino ausgewählt";
        const std::string state_cappuccino_brewing = "Bereitet Cappuccino zu";

        const std::string state_ground_latte_selected = "Vorgemahlener Latte macchiato ausgewählt";
        const std::string state_latte_programming_mode = "Latte macchiato Programmiermodus ausgewählt";
        const std::string state_latte_selected = "Latte macchiato ausgewählt";
        const std::string state_latte_brewing = "Bereitet Latte macchiato zu";

        const std::string state_hot_water_programming_mode = "Heißes Wasser Programmiermodus ausgewählt";
        const std::string state_hot_water_selected = "Heißes Wasser ausgewählt";
        const std::string state_hot_water_brewing = "Bereitet heißes Wasser zu";

        const std::string state_steam_selected = "Dampf ausgewählt";
        const std::string state_steam_brewing = "Bereitet Dampf zu";

#elif defined(PHILIPS_COFFEE_LANG_it_IT)
        const std::string state_unknown = "Sconosciuto";
        const std::string state_off = "Spento";
        const std::string state_idle = "In Attesa";
        const std::string state_cleaning = "Pulizia";
        const std::string state_preparing = "Preparazione";
        const std::string state_water_empty = "Serbatoio Acqua Vuoto";
        const std::string state_waste_warning = "Attenzione Contenitore Fondi Caffè";
        const std::string state_error = "Errore";
        const std::string state_internal_error = "Errore interno";

        const std::string state_ground_coffee_selected = "Selezionato Caffè Premacinato";
        const std::string state_coffee_programming_mode = "Selezionata Modalità programmazione Caffè";
        const std::string state_coffee_selected = "Selezionato Caffè";
        const std::string state_coffee_2x_selected = "Selezionati 2 Caffè";
        const std::string state_coffee_brewing = "Erogazione Caffè";
        const std::string state_coffee_2x_brewing = "Erogazione 2 Caffè";

        const std::string state_ground_espresso_selected = "Selezionate Espresso Premacinato";
        const std::string state_espresso_programming_mode = "Selezionata Modalità programmazione Espresso";
        const std::string state_espresso_selected = "Selezionato Espresso";
        const std::string state_espresso_2x_selected = "Selezionat1 2 Espressi";
        const std::string state_espresso_brewing = "Erogazione Espresso";
        const std::string state_espresso_2x_brewing = "Erogazione 2 Espressi";

        const std::string state_ground_americano_selected = "Selezionato Americano Premacinato";
        const std::string state_americano_programming_mode = "Selezionata Modalità programmazione Americano";
        const std::string state_americano_selected = "Selezionato Americano";
        const std::string state_americano_2x_selected = "Selezionati 2 Americani";
        const std::string state_americano_brewing = "Erogazione Americano";
        const std::string state_americano_2x_brewing = "Erogazione 2 Americani";

        const std::string state_ground_cappuccino_selected = "Selezionato Cappuccino Premacinato";
        const std::string state_cappuccino_programming_mode = "Selezionata Modalità programmazione Cappuccino";
        const std::string state_cappuccino_selected = "Selezionato Cappuccino";
        const std::string state_cappuccino_brewing = "Erogazione Cappuccino";

        const std::string state_ground_latte_selected = "Selezionato Latte Macchiato Premacinato";
        const std::string state_latte_programming_mode = "Selezionata Modalità programmazione Latte Macchiato";
        const std::string state_latte_selected = "Selezionato Latte Macchiato";
        const std::string state_latte_brewing = "Erogazione Latte Macchiato";

        const std::string state_hot_water_programming_mode = "Selezionata Modalità programmazione Acqua Calda";
        const std::string state_hot_water_selected = "Selezionata Acqua Calda";
        const std::string state_hot_water_brewing = "Erogazione Acqua Calda";

        const std::string state_steam_selected = "Vapore Selezionato";
        const std::string state_steam_brewing = "Erogazione Vapore";
#else
#define PHILIPS_COFFEE_LANG_DEFAULT
#endif

#ifdef PHILIPS_COFFEE_LANG_DEFAULT
        const std::string state_unknown = "Unknown";
        const std::string state_off = "Off";
        const std::string state_idle = "Idle";
        const std::string state_cleaning = "Cleaning";
        const std::string state_preparing = "Preparing";
        const std::string state_water_empty = "Water empty";
        const std::string state_waste_warning = "Waste container warning";
        const std::string state_error = "Error";
        const std::string state_internal_error = "Internal Error";

        const std::string state_ground_coffee_selected = "Pre-ground Coffee selected";
        const std::string state_coffee_programming_mode = "Coffee programming mode selected";
        const std::string state_coffee_selected = "Coffee selected";
        const std::string state_coffee_2x_selected = "2x Coffee selected";
        const std::string state_coffee_brewing = "Brewing Coffee";
        const std::string state_coffee_2x_brewing = "Brewing 2x Coffee";

        const std::string state_ground_espresso_selected = "Pre-ground Espresso selected";
        const std::string state_espresso_programming_mode = "Espresso programming mode selected";
        const std::string state_espresso_selected = "Espresso selected";
        const std::string state_espresso_2x_selected = "2x Espresso selected";
        const std::string state_espresso_brewing = "Brewing Espresso";
        const std::string state_espresso_2x_brewing = "Brewing 2x Espresso";

        const std::string state_ground_americano_selected = "Pre-ground Americano selected";
        const std::string state_americano_programming_mode = "Americano programming mode selected";
        const std::string state_americano_selected = "Americano selected";
        const std::string state_americano_2x_selected = "2x Americano selected";
        const std::string state_americano_brewing = "Brewing Americano";
        const std::string state_americano_2x_brewing = "Brewing 2x Americano";

        const std::string state_ground_cappuccino_selected = "Pre-ground Cappuccino selected";
        const std::string state_cappuccino_programming_mode = "Cappuccino programming mode selected";
        const std::string state_cappuccino_selected = "Cappuccino selected";
        const std::string state_cappuccino_brewing = "Brewing Cappuccino";

        const std::string state_ground_latte_selected = "Pre-ground Latte Macchiato selected";
        const std::string state_latte_programming_mode = "Latte Macchiato programming mode selected";
        const std::string state_latte_selected = "Latte Macchiato selected";
        const std::string state_latte_brewing = "Brewing Latte Macchiato";

        const std::string state_hot_water_programming_mode = "Hot water programming mode selected";
        const std::string state_hot_water_selected = "Hot water selected";
        const std::string state_hot_water_brewing = "Making Hot Water";

        const std::string state_steam_selected = "Steam selected";
        const std::string state_steam_brewing = "Making Steam";
#endif
    } // namespace philips_coffee_machine
} // namespace esphome
