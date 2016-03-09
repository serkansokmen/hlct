#pragma once

#include "ofMain.h"

namespace hlct {
    
    enum GameState {
        GAME_STATE_TITLE = 0,
        GAME_STATE_POSING = 1,
        GAME_STATE_GAME = 2,
        GAME_STATE_END_WIN = 3,
        GAME_STATE_END_LOOSE = 4,
    };
}