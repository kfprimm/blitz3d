bb_start_module(blitz3d)
set(DEPENDS_ON bb.blitz2d)
set(SOURCES animation.cpp animator.cpp blitz3d.h blitz3d.cpp brush.cpp cachedtexture.cpp camera.cpp collision.cpp entity.cpp frustum.cpp geom.cpp light.cpp listener.cpp loader_3ds.cpp loader_b3d.cpp md2model.cpp md2norms.cpp md2rep.cpp mesh.cpp meshcollider.cpp meshloader.cpp meshmodel.cpp meshutil.cpp mirror.cpp model.cpp object.cpp pivot.cpp planemodel.cpp q3bspmodel.cpp q3bsprep.cpp scene.cpp sprite.cpp std.cpp surface.cpp terrain.cpp terrainrep.cpp texture.cpp world.cpp animation.h animator.h blitz3d.h brush.h cachedtexture.h camera.h collision.h entity.h frustum.h geom.h light.h listener.h loader_3ds.h loader_b3d.h md2model.h md2norms.h md2rep.h meshcollider.h meshloader.h meshmodel.h meshutil.h mirror.h model.h object.h pivot.h planemodel.h q3bspmodel.h q3bsprep.h rendercontext.h scene.h sprite.h std.h surface.h terrain.h terrainrep.h texture.h world.h module.link.cpp)

IF(NOT BB_WINDOWS AND NOT BB_MINGW32)
  set(SOURCES ${SOURCES} loader_assimp.cpp loader_assimp.h)
  set(LIBS ${LIBS} assimp)
ELSE()
  set(SOURCES ${SOURCES} loader_x.cpp loader_x.h)
  set(LIBS ${LIBS} d3dxof)
ENDIF()

bb_end_module()

IF(BB_WINDOWS)
  target_link_libraries(bb.blitz3d dxsdk)
ENDIF()

IF(NOT BB_WINDOWS AND NOT BB_MINGW32)
  target_compile_definitions (bb.blitz3d PRIVATE USE_ASSIMP)
ENDIF()
