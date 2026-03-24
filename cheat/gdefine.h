#pragma once

//#define CHEAT_DEBUG
#define UE_5_3

#define WINDOW_CLASS_NAME L"UnrealWindow"
#define WINDOW_NAME L"Ready Or Not  "

#define CHEAT_VERSION "26.3.22"
#define CHEAT_HEART "ron"

#define SERVER_IP "62.234.220.74"
#define SERVER_PORT 1314

#define SAVE_PATH "C:\\LHY1339\\ready_or_not\\config.txt"

#ifdef UE_4_27
#define POST_RENDER_INDEX 0x63
#endif
#ifdef UE_5_3
#define POST_RENDER_INDEX 0x70
#endif
#ifdef UE_5_5
#define POST_RENDER_INDEX 0x72
#endif
#ifdef UE_5_6
#define POST_RENDER_INDEX 0x6F
#endif
#ifdef UE_5_7
#define POST_RENDER_INDEX 0x6F
#endif