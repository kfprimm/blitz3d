// AUTOGENERATED. DO NOT EDIT.
// RUN `bin/blitz3d config` TO UPDATE.

#include <bb/stub/stub.h>


#include "../../stdutil/stdutil.h"

BBMODULE_DECL( blitz );
BBMODULE_DECL( hook );
BBMODULE_DECL( event );
BBMODULE_DECL( math );
BBMODULE_DECL( string );
BBMODULE_DECL( stdio );
BBMODULE_DECL( stream );
BBMODULE_DECL( sockets );
BBMODULE_DECL( enet );
BBMODULE_DECL( filesystem );
BBMODULE_DECL( filesystem_windows );
BBMODULE_DECL( runtime );
BBMODULE_DECL( timer );
BBMODULE_DECL( timer_windows );
BBMODULE_DECL( system );
BBMODULE_DECL( input );
BBMODULE_DECL( input_directinput8 );
BBMODULE_DECL( audio );
BBMODULE_DECL( audio_fmod );
BBMODULE_DECL( userlibs );
BBMODULE_DECL( bank );
BBMODULE_DECL( system_windows );
BBMODULE_DECL( pixmap );
BBMODULE_DECL( blitz2d );
BBMODULE_DECL( blitz3d );
BBMODULE_DECL( blitz2d_d3d7 );
BBMODULE_DECL( graphics );
BBMODULE_DECL( graphics_d3d7 );
BBMODULE_DECL( blitz3d_d3d7 );
BBMODULE_DECL( frame );
BBMODULE_DECL( runtime_windows );

class BBRuntime;
BBRuntime *bbCreateD3D7Runtime();
BBRuntime *bbCreateRuntime(){
	return bbCreateD3D7Runtime();
}

#ifdef WIN32
void bbruntime_link( void (*link)( const char *sym,void *pc ) ){
	blitz_link( link );
	hook_link( link );
	event_link( link );
	math_link( link );
	string_link( link );
	stdio_link( link );
	stream_link( link );
	sockets_link( link );
	enet_link( link );
	filesystem_link( link );
	filesystem_windows_link( link );
	runtime_link( link );
	timer_link( link );
	timer_windows_link( link );
	system_link( link );
	input_link( link );
	input_directinput8_link( link );
	audio_link( link );
	audio_fmod_link( link );
	userlibs_link( link );
	bank_link( link );
	system_windows_link( link );
	pixmap_link( link );
	blitz2d_link( link );
	blitz3d_link( link );
	blitz2d_d3d7_link( link );
	graphics_link( link );
	graphics_d3d7_link( link );
	blitz3d_d3d7_link( link );
	frame_link( link );
	runtime_windows_link( link );
}
#endif
bool bbruntime_create(){
	if( blitz_create() ){
				if( hook_create() ){
						if( event_create() ){
								if( math_create() ){
										if( string_create() ){
												if( stdio_create() ){
														if( stream_create() ){
																if( sockets_create() ){
																		if( enet_create() ){
																				if( filesystem_create() ){
																						if( filesystem_windows_create() ){
																								if( runtime_create() ){
																										if( timer_create() ){
																												if( timer_windows_create() ){
																														if( system_create() ){
																																if( input_create() ){
																																		if( input_directinput8_create() ){
																																				if( audio_create() ){
																																						if( audio_fmod_create() ){
																																								if( userlibs_create() ){
																																										if( bank_create() ){
																																												if( system_windows_create() ){
																																														if( pixmap_create() ){
																																																if( blitz2d_create() ){
																																																		if( blitz3d_create() ){
																																																				if( blitz2d_d3d7_create() ){
																																																						if( graphics_create() ){
																																																								if( graphics_d3d7_create() ){
																																																										if( blitz3d_d3d7_create() ){
																																																												if( frame_create() ){
																																																														if( runtime_windows_create() ){
																																																																return true;
																															}else sue( "runtime_windows_create failed" );
																															frame_destroy();
																														}else sue( "frame_create failed" );
																														blitz3d_d3d7_destroy();
																													}else sue( "blitz3d_d3d7_create failed" );
																													graphics_d3d7_destroy();
																												}else sue( "graphics_d3d7_create failed" );
																												graphics_destroy();
																											}else sue( "graphics_create failed" );
																											blitz2d_d3d7_destroy();
																										}else sue( "blitz2d_d3d7_create failed" );
																										blitz3d_destroy();
																									}else sue( "blitz3d_create failed" );
																									blitz2d_destroy();
																								}else sue( "blitz2d_create failed" );
																								pixmap_destroy();
																							}else sue( "pixmap_create failed" );
																							system_windows_destroy();
																						}else sue( "system_windows_create failed" );
																						bank_destroy();
																					}else sue( "bank_create failed" );
																					userlibs_destroy();
																				}else sue( "userlibs_create failed" );
																				audio_fmod_destroy();
																			}else sue( "audio_fmod_create failed" );
																			audio_destroy();
																		}else sue( "audio_create failed" );
																		input_directinput8_destroy();
																	}else sue( "input_directinput8_create failed" );
																	input_destroy();
																}else sue( "input_create failed" );
																system_destroy();
															}else sue( "system_create failed" );
															timer_windows_destroy();
														}else sue( "timer_windows_create failed" );
														timer_destroy();
													}else sue( "timer_create failed" );
													runtime_destroy();
												}else sue( "runtime_create failed" );
												filesystem_windows_destroy();
											}else sue( "filesystem_windows_create failed" );
											filesystem_destroy();
										}else sue( "filesystem_create failed" );
										enet_destroy();
									}else sue( "enet_create failed" );
									sockets_destroy();
								}else sue( "sockets_create failed" );
								stream_destroy();
							}else sue( "stream_create failed" );
							stdio_destroy();
						}else sue( "stdio_create failed" );
						string_destroy();
					}else sue( "string_create failed" );
					math_destroy();
				}else sue( "math_create failed" );
				event_destroy();
			}else sue( "event_create failed" );
			hook_destroy();
		}else sue( "hook_create failed" );
		blitz_destroy();
	}else sue( "blitz_create failed" );
	return false;
}

bool bbruntime_destroy(){
	runtime_windows_destroy();
	frame_destroy();
	blitz3d_d3d7_destroy();
	graphics_d3d7_destroy();
	graphics_destroy();
	blitz2d_d3d7_destroy();
	blitz3d_destroy();
	blitz2d_destroy();
	pixmap_destroy();
	system_windows_destroy();
	bank_destroy();
	userlibs_destroy();
	audio_fmod_destroy();
	audio_destroy();
	input_directinput8_destroy();
	input_destroy();
	system_destroy();
	timer_windows_destroy();
	timer_destroy();
	runtime_destroy();
	filesystem_windows_destroy();
	filesystem_destroy();
	enet_destroy();
	sockets_destroy();
	stream_destroy();
	stdio_destroy();
	string_destroy();
	math_destroy();
	event_destroy();
	hook_destroy();
	blitz_destroy();
	return true;
}
