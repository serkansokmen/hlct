#pragma once

#define HLCT_LIVES                      3       // how many lives it takes to die
#define HLCT_MAX_CATCH                  10      // how many needed to catch for a win
#define HLCT_HELMET_SECTION_COUNT       4       // screen width is divided into randomize sections to drop helmets


#define HLCT_USER_POSE_DURATION         5       // seconds for posing
#define HLCT_GAME_END_TO_TITLE_DURATION 10      // game ended, how many seconds we need to go back to title screen
#define HLCT_MIN_DURATION               10      // game minimum duration in seconds (used mainly in gui slider)
#define HLCT_MAX_DURATION               200     // game maximum duration in seconds (used mainly in gui slider)

#define HLCT_HERO_BOTTOM                120     // hero position from bottom in pixels
#define HLCT_CLAMP_STAGE                100     // stage left/right clamp in pixels


#define HLCT_OSC_PORT                   7000    // osc receiver port
// endpoints
#define HLCT_OSC_ENDPOINT_DURATION      "/hlct/duration"
#define HLCT_OSC_ENDPOINT_USER          "/hlct/user"
#define HLCT_OSC_ENDPOINT_POSING        "/hlct/posing"
#define HLCT_OSC_ENDPOINT_POSITION      "/hlct/position"
#define HLCT_OSC_ENDPOINT_NEW           "/hlct/new"
#define HLCT_OSC_ENDPOINT_PAUSE         "/hlct/pause"
