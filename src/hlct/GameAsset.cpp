#include "GameAsset.h"



void hlct::GameAsset::setup(const ofRectangle& rect){
    title.setup("game/title_0.png", "game/title_1.png", rect);
    posing.setup("game/posing.png", "game/posing.png", rect);
    win.setup("game/win_0.png", "game/win_1.png", rect);
    loose.setup("game/loose_0.png", "game/loose_1.png", rect);
}


void hlct::GameAsset::update(const GameState& state){
    this->state = state;
    if (state != GAME_STATE_GAME){
        title.update(state);
        posing.update(state);
        win.update(state);
        loose.update(state);
    }
}


void hlct::GameAsset::draw(){
    switch (state) {
        case GAME_STATE_TITLE:
            title.draw();
            break;
        case GAME_STATE_POSING:
            posing.draw();
            break;
        case GAME_STATE_END_WIN:
            win.draw();
            break;
        case GAME_STATE_END_LOOSE:
            loose.draw();
            break;
        default:
            break;
    }
}