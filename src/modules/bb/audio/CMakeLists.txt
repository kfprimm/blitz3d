bb_start_module(audio)
set(DEPENDS_ON bb.runtime)
set(SOURCES channel.cpp channel.h sound.cpp sound.h driver.cpp driver.h audio.cpp audio.h stream.cpp stream.h module.link.cpp)

IF(BB_DESKTOP AND NOT BB_WINDOWS)
 set (SOURCES ${SOURCES} ogg_stream.cpp ogg_stream.h wav_stream.cpp wav_stream.h)
 set (LIBS ${LIBS} vorbis ogg)
ENDIF()

bb_end_module()
