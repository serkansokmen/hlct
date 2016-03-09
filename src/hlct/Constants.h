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

#define HLCT_INFO_SCREEN_FONT_PATH      "font/Kollektif-Bold.ttf"   // info screen font path
#define HLCT_INFO_SCREEN_FONT_SIZE      85                          // info screen font size
#define HLCT_INFO_SCREEN_DURATION       2                           // seconds for info screen message switch timer
#define HLCT_INFO_SCREEN_FONT_NAME      "kollektif_bold_85"         // used internally

// Scale min/max
#define HLCT_MIN_HERO_SCALE             .5f
#define HLCT_MAX_HERO_SCALE             2.f
#define HLCT_MIN_BAIT_SCALE             .5f
#define HLCT_MAX_BAIT_SCALE             2.f
#define HLCT_MIN_LIVE_SCALE             .25f
#define HLCT_MAX_LIVE_SCALE             4.f