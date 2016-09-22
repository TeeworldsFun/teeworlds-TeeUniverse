#ifndef TU_CLIENT_TEXTRENDERER_H
#define TU_CLIENT_TEXTRENDERER_H

#include <tu/client/kernel.h>
#include <tu/client/gui/rect.h>
#include <engine/graphics.h>
#include <base/vmath.h>
#include <base/tl/array.h>
#include <base/tl/sorted_array.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace tu
{
	
class CTextRenderer : public CKernel::CComponent
{
protected:
	struct CFont
	{
		char m_aFilename[512];
		FT_Face m_FtFace;
	};
	
	struct CBlock
	{
	public:
		ivec2 m_Granularity;
		int m_Size;
		int m_MaxSize;
		
		inline bool IsFull() const { return (m_Size >= m_MaxSize); }
	};
	
	struct CGlyph
	{
		int m_CharCode;
		int m_RenderTick;
		int m_Width;
		int m_Height;
		ivec2 m_Granularity;
		int m_Block;
		int m_BlockPos;
		char* m_pData;
		vec2 m_UVMin;
		vec2 m_UVMax;
		
		float m_AdvanceX;
		float m_OffsetX;
		float m_OffsetY;
		
		//For sorting: this speed up *a lot* the time needed to search a glyph
		CGlyph() {}
		CGlyph(int CharCode) : m_CharCode(CharCode) {}
		inline bool operator<(const CGlyph& Glyph) const { return (m_CharCode < Glyph.m_CharCode); }
	};
	
	class CGlyphCache : public CKernel::CGuest
	{
	public:
		int m_FontSize;
		int m_PPG;
		int m_GPB;
		int m_PPB;
		int m_OffsetY;
		int m_RenderTick;
		int m_Width;
		int m_Height;
		char* m_pData;
		IGraphics::CTextureHandle m_Texture;
		int m_MemoryUsage;
		int m_Version;
		
		array<CBlock> m_Blocks;
		sorted_array<CGlyph> m_Glyphs;
		
	private:
		void IncreaseCache();
		int NewBlock(ivec2 Granularity);
		void UpdateGlyph(CTextRenderer::CGlyph* pGlyph);
		
	public:
		CGlyphCache(CKernel* pKernel);
		~CGlyphCache();
		void Init(int FontSize);
		CGlyph* NewGlyph(int CharCode, int Width, int Height);
		CGlyph* FindGlyph(int CharCode);
		void UpdateTexture(CTextRenderer::CGlyph* pGlyph, const char* pData);
		void UpdateVersion();
	};
	
public:
	//Warning: the cache assume that the text is still the same.
	class CTextCache
	{
	public:
		struct CQuad
		{
			int m_TextIter;			//Character in the original string that has created this quad
			vec2 m_AdvancePos;
			vec2 m_QuadPos;
			vec2 m_Size;
			vec2 m_UVMin;
			vec2 m_UVMax;
		};
		
	public:
		ivec2 m_BoxSize;			//To check if the context is different
		float m_FontSize;
		int m_GlyphCacheId;			//Any modification in the GlyphCache can make obsolete the TextCache
		int m_GlyphCacheVersion;				
		array<CQuad> m_Quads;		//Keep quads to redraw without seaching and shape again
		float m_TextWidth;
		
	public:
		CTextCache();
		void Reset();
	};
	
	struct CTextCursor
	{
		int m_TextIter;
		vec2 m_Position;
	};
	
private:
	FT_Library m_FTLibrary;
	array<CFont*> m_Fonts;
	array<CGlyphCache*> m_GlyphCaches;
	int m_RenderTick;

private:
	CGlyph* LoadGlyph(CGlyphCache* pCache, int Character);
	CGlyph* FindGlyph(CGlyphCache* pCache, int Character);
	CGlyph* GetGlyph(CGlyphCache* pCache, int Character);

public:
	CTextRenderer(CKernel* pKernel);
	virtual ~CTextRenderer();
	
	bool Init();
	void Update();
	bool LoadFont(const char* pFilename);
	
	void UpdateTextCache(const char* pText, ivec2 BoxSize, float FontSize, CTextCache* pTextCache);
	float GetTextWidth(const char* pText, gui::CRect Rect, float FontSize, CTextCache* pTextCache);
	CTextCursor GetTextCursorFromPosition(const char* pText, ivec2 MousePosition, gui::CRect Rect, float FontSize, CTextCache* pTextCache);
	CTextCursor GetTextCursorFromTextIter(const char* pText, int TextIter, gui::CRect Rect, float FontSize, CTextCache* pTextCache);
	void DrawText(const char* pText, gui::CRect Rect, float FontSize, vec4 Color, CTextCache* pTextCache);
	void Debug_DrawCaches();
};

}

#endif
