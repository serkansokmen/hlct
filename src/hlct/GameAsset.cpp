#include "GameAsset.h"



void hlct::GameAsset::setup(){
    title.setup("game/title_0.png", "game/title_1.png");
    win.setup("game/win_0.png", "game/win_1.png");
    loose.setup("game/loose_0.png", "game/loose_1.png");
}


void hlct::GameAsset::update(const GameState& state){
    
    this->state = state;
    
    title.update();
    win.update();
    loose.update();
}


void hlct::GameAsset::draw(){
    switch (state) {
        case GAME_STATE_TITLE:
            title.draw(state);
            break;
        case GAME_STATE_END_WIN:
            win.draw(state);
            break;
        case GAME_STATE_END_LOOSE:
            loose.draw(state);
            break;
        default:
            break;
    }
}