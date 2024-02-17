#pragma once

#include "core/application.h"
#include "core/logger.h"
#include "game_types.h"

extern Boolean create_game(game* out_game);

int main(void) {

    game game_instance;
    if (!create_game(&game_instance)) {
        KFATAL("Could not create game!");
        return -1;
    }
    
    if (!game_instance.render || !game_instance.update || !game_instance.initialize || !game_instance.on_resize) {
        KFATAL("All of the game's function pointers MUST be assigned.");
        return -2;
    }

    if (!application_create(&game_instance)) {
        KINFO("Application failed to create!");
        return 1;
    }
    
    if (!application_run()) {
        KINFO("Application did not shut down gracefully.");
        return 2;
    }

    return 0;
}