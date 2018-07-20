#pragma once

#ifdef ELEMENT_PLAYER_STATUS_BAR_ENABLED

#ifdef __cplusplus
extern "C" {
#endif

#include "makise_e.h"

#define PLAYER_STATUS_STOP		0
#define PLAYER_STATUS_PLAY		1
#define PLAYER_STATUS_PAUSE		2

typedef struct {
	const MakiseFont*			const fontErcent;
	const uint32_t				fontErcentCol;
	const uint32_t				bgColor;
	const uint32_t				borderColor;
} MakiseStyle_SMPlayerStatusBar;

typedef struct {
	uint32_t ( *getStatePlay )			( void );
	uint32_t ( *getPercentBattery )		( void );
} MPlayerStatusBar_CallbackFunc;

typedef struct {
	MElement								e;
	const MPlayerStatusBar_CallbackFunc*	f;
	const MakiseStyle_SMPlayerStatusBar*	s;
} MPlayerStatusBar;

void m_create_player_status_bar( MPlayerStatusBar*						b,
								 MContainer*							c,
								 MPosition								pos,
								 const MakiseStyle_SMPlayerStatusBar*	s,
								 const MPlayerStatusBar_CallbackFunc*	f );


#ifdef __cplusplus
}
#endif

#endif
