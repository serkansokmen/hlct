#include "GameAsset.h"



void hlct::GameAsset::setup(){
    switchImgTitle.setup("game/title_0.png", "game/title_1.png");
    switchImgWin.setup("game/win_0.png", "game/win_1.png");
    switchImgLoose.setup("game/loose_0.png", "game/loose_1.png");
}


void hlct::GameAsset::draw(const hlct::GameState& state){
    switch (state) {
        case GAME_STATE_TITLE:
            switchImgTitle.draw(state);
            break;
        case GAME_STATE_END_WIN:
            switchImgWin.draw(state);
            break;
        case GAME_STATE_END_LOOSE:
            switchImgLoose.draw(state);
            break;
        default:
            break;
    }
}