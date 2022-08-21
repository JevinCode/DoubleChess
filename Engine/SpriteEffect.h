#pragma once

#include "Colors.h"
#include "Graphics.h"

namespace SpriteEffect
{
	class BorderHover
	{
	public:
		void operator()();
	private:
		Color borderColor;
	};
	class Chroma
	{
	public:
		Chroma( Color c )
			:
			chroma( c )
		{}
		void operator()( Color cSrc,int xDest,int yDest,Graphics& gfx ) const
		{
			if( cSrc != chroma )
			{
				gfx.PutPixel( xDest,yDest,cSrc );
			}
		}
	private:
		Color chroma;
	};
	class Substitution
	{
	public:
		Substitution( Color c,Color s )
			:
			chroma( c ),
			sub( s )
		{}
		void operator()( Color cSrc,int xDest,int yDest,Graphics& gfx ) const
		{
			if( cSrc != chroma )
			{
				gfx.PutPixel( xDest,yDest,sub );
			}
		}
	private:
		Color chroma = Colors::Magenta;
		Color sub;
	};
	class Copy
	{
	public:
		void operator()( Color cSrc,int xDest,int yDest,Graphics& gfx ) const
		{
			gfx.PutPixel( xDest,yDest,cSrc );
		}
	};
	class Ghost
	{
	public:
		Ghost( Color c )
			:
			chroma( c )
		{}
		void operator()( Color src,int xDest,int yDest,Graphics& gfx ) const
		{
			if( src != chroma )
			{
				const Color dest = gfx.GetPixel( xDest,yDest );
				const Color blend = {
					unsigned char( (src.GetR() + dest.GetR()) / 2 ),
					unsigned char( (src.GetG() + dest.GetG()) / 2 ),
					unsigned char( (src.GetB() + dest.GetB()) / 2 )
				};
				gfx.PutPixel( xDest,yDest,blend );
			}
		}
	private:
		Color chroma;
	};
	class SolidAlpha
	{
	public:
		SolidAlpha(Color solid, Color chroma, float alpha)
			:
			solid(solid),
			chroma(chroma),
			alpha(alpha)
		{}
		void operator()(Color src, int xDest, int yDest, Graphics& gfx) const
		{
			if (src != chroma)
			{
				const Color blend = {
					unsigned char((1.0f - alpha)*src.GetR() + alpha*solid.GetR()),
					unsigned char((1.0f - alpha)*src.GetG() + alpha*solid.GetG()),
					unsigned char((1.0f - alpha)*src.GetB() + alpha*solid.GetB())
				};
				gfx.PutPixel(xDest, yDest, blend);

			}
		}
	private:
		Color solid;
		Color chroma;
		float alpha;
	};
}