#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#define GAME_VERSION_JP     0
#define GAME_VERSION_US0    1
#define GAME_VERSION_US1    2
#define GAME_VERSION_EU     3

#if GAME_VERSION==GAME_VERSION_JP
#define _bssSegmentSize 0x9BF10
#elif GAME_VERSION==GAME_VERSION_US0
#define _bssSegmentSize 0x9C490
#elif GAME_VERSION==GAME_VERSION_US1
#define _bssSegmentSize 0x9C2D0
#elif GAME_VERSION==GAME_VERSION_EU
#define _bssSegmentSize 0x96AD0
#endif

#endif

