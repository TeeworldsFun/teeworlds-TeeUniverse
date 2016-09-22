#ifndef TU_CLIENT_TEXTRENDERER_H
#define TU_CLIENT_TEXTRENDERER_H

#include <tu/client/kernel.h>
#include <tu/client/gui/rect.h>
#include <engine/graphics.h>
#include <base/vmath.h>
#include <base/tl/array.h>
#include <base/tl/sorted_array.h>
#include <string>

//FreeType to load font and rasterization
#include <ft2build.h>
#include FT_FREETYPE_H

//ICU to apply BiDi algorithm
#include <unicode/unistr.h> //To convert utf8 to utf16

//HarfBuff to shape the text (arabic, ...)
#include <harfbuzz/hb.h>

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
		int m_FontId;
		int m_GlyphCode;
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
		CGlyph(int FontId, int GlyphCode) : m_FontId(FontId), m_GlyphCode(GlyphCode) {}
		inline int Comp(int FontId, int GlyphCode) const { return ((m_FontId == FontId) ? comp(m_GlyphCode, GlyphCode) : comp(m_FontId, FontId)); }
		inline bool operator<(const CGlyph& Glyph) const { return (Comp(Glyph.m_FontId, Glyph.m_GlyphCode) < 0); }
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
		CGlyph* NewGlyph(int FontId, int GlyphCode, int Width, int Height);
		CGlyph* FindGlyph(int FontId, int GlyphCode);
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
		bool m_Rendered;
		
		std::string m_Text;				//Store the text to detect if the cache must be updated
		ivec2 m_BoxSize;			//To check if the context is different
		float m_FontSize;
		
		int m_GlyphCacheId;			//Any modification in the GlyphCache can make obsolete the TextCache
		int m_GlyphCacheVersion;
					
		array<CQuad> m_Quads;		//Keep quads to redraw quickly
		float m_TextWidth;
		
	public:
		CTextCache();
		void ResetRendering();
		void SetText(const char* pText);
		void SetBoxSize(ivec2 BoxSize);
		void SetFontSize(float FontSize);
	};
	
	struct CTextCursor
	{
		int m_TextIter;
		vec2 m_Position;
	};
	
private:
	FT_Library m_FTLibrary;
	hb_font_t* m_pHBFont;
	array<CFont*> m_Fonts;
	array<CGlyphCache*> m_GlyphCaches;
	int m_RenderTick;

private:
	CGlyph* LoadGlyph(CGlyphCache* pCache, int FontId, int GlyphCode);
	CGlyph* FindGlyph(CGlyphCache* pCache, int FontId, int GlyphCode);
	CGlyph* GetGlyph(CGlyphCache* pCache, int FontId, int GlyphCode);
	
	void UpdateTextCache_GenerateGlyphChain(array<int>* pGlyphChain, const UChar* pTextUTF16, int Start, int Length, bool IsRTL);

public:
	CTextRenderer(CKernel* pKernel);
	virtual ~CTextRenderer();
	
	bool Init();
	void Update();
	bool LoadFont(const char* pFilename);
	
	void UpdateTextCache(CTextCache* pTextCache);
	float GetTextWidth(CTextCache* pTextCache);
	CTextCursor GetTextCursorFromPosition(CTextCache* pTextCache, ivec2 TextPosition, ivec2 MousePosition);
	CTextCursor GetTextCursorFromTextIter(CTextCache* pTextCache, ivec2 TextPosition, int TextIter);
	void DrawText(CTextCache* pTextCache, ivec2 Position, vec4 Color);
	void Debug_DrawCaches();
};

}

#endif
