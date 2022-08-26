#define GOD_GRAPHICS
#include "Graphics.h"

void Graphics::BeginFrame()
{
	// clear the sysbuffer
	memset( pSysBuffer,0u,sizeof( Color ) * Graphics::ScreenHeight * Graphics::ScreenWidth );
}

void Graphics::PutPixel( int x,int y,Color c )
{
	assert( x >= 0 );
	assert( x < int( Graphics::ScreenWidth ) );
	assert( y >= 0 );
	assert( y < int( Graphics::ScreenHeight ) );
	pSysBuffer[Graphics::ScreenWidth * y + x] = c;
}

void Graphics::DrawBorder(const RectI& s, Color borderColor, int borderWidth)
{
	RectI outer = s.GetExpanded(borderWidth);
	for (int y = outer.top; y <= outer.bottom; y++)
	{
		for (int x = outer.left; x <= outer.right; x++)
		{
			if (outer.Contains({ x,y }) && !s.Contains({ x,y }) && Graphics::GetScreenRect().Contains({ x,y }))
			{
				PutPixel(x, y, borderColor);
			}
		}
	}
}

Color Graphics::GetPixel( int x,int y ) const
{
	assert( x >= 0 );
	assert( x < int( Graphics::ScreenWidth ) );
	assert( y >= 0 );
	assert( y < int( Graphics::ScreenHeight ) );
	return pSysBuffer[Graphics::ScreenWidth * y + x];
}

void dummy( Graphics& gfx )
{
	gfx.DrawSprite( 0,0,RectI{ 0,0,0,0 },RectI{ 0,0,0,0 },Surface{},SpriteEffect::Copy{} );
	gfx.DrawSprite( 0,0,RectI{ 0,0,0,0 },RectI{ 0,0,0,0 },Surface{},SpriteEffect::Chroma{ Colors::Black } );
	gfx.DrawSprite( 0,0,RectI{ 0,0,0,0 },RectI{ 0,0,0,0 },Surface{},SpriteEffect::Substitution{ Colors::Black,Colors::Black } );
}
