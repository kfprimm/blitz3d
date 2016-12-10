
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include "cachedtexture.h"

#include "../graphics/canvas.h"

class Texture{
public:
	Texture();
	Texture( const std::string &file,int flags );
	Texture( const std::string &file,int flags,int w,int h,int first,int cnt );
	Texture( int width,int height,int flags,int cnt );
	Texture( const Texture &texture );
	~Texture();

	Texture &operator=( const Texture &texture );

	void setScale( float u_scale,float v_scale );
	void setRotation( float rot );
	void setPosition( float u_pos,float v_pos );
	void setBlend( int blend );
	void setFlags( int flags );

	int getCanvasFlags()const;
	BBCanvas *getCanvas( int frame )const;
	const BBScene::Matrix *getMatrix()const;
	int getBlend()const;
	int getFlags()const;
	CachedTexture *getCachedTexture()const;

	bool isTransparent()const;
	bool operator<( const Texture &t )const;

	static void clearFilters();
	static void addFilter( const std::string &filter,int flags );

private:
	struct Rep;
	Rep *rep;
};

#endif
