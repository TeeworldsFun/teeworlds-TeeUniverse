#include <tu/client/textrenderer.h>
#include <engine/storage.h>

namespace tu
{

/* GLYPH CACHE ********************************************************/

CTextRenderer::CGlyphCache::CGlyphCache(CKernel* pKernel) :
	CKernel::CGuest(pKernel),
	m_pData(0),
	m_Version(0)
{
	
}

CTextRenderer::CGlyphCache::~CGlyphCache()
{
	if(m_pData)
		delete[] m_pData;
	
	if(m_Texture.IsValid())
		Graphics()->UnloadTexture(&m_Texture);
}

void CTextRenderer::CGlyphCache::UpdateVersion()
{
	m_Version++;
}

void CTextRenderer::CGlyphCache::UpdateGlyph(CTextRenderer::CGlyph* pGlyph)
{
	int NumBlockX = m_Width/m_PPB;
	int NumGlyphX = m_GPB/pGlyph->m_Granularity.x;
	int BlockPosX = pGlyph->m_BlockPos % NumGlyphX;
	int BlockPosY = pGlyph->m_BlockPos / NumGlyphX;
	
	int DataX = (pGlyph->m_Block % NumBlockX)*m_PPB + (BlockPosX * pGlyph->m_Granularity.x)*m_PPG;
	int DataY = (pGlyph->m_Block / NumBlockX)*m_PPB + (BlockPosY * pGlyph->m_Granularity.y)*m_PPG;
	pGlyph->m_pData = m_pData + DataY * m_Width + DataX;
	
	float UScale = 1.0f/m_Width;
	float VScale = 1.0f/m_Height;
	
	pGlyph->m_UVMin.x = DataX * UScale;
	pGlyph->m_UVMin.y = DataY * VScale;
	pGlyph->m_UVMax.x = (DataX + pGlyph->m_Width) * UScale;
	pGlyph->m_UVMax.y = (DataY + pGlyph->m_Height) * VScale;
}

const int s_Margin = 2;

void CTextRenderer::CGlyphCache::Init(int FontSize)
{
	m_FontSize = FontSize;
	m_OffsetY = m_FontSize;
	m_PPG = nearestPowerOfTwo((FontSize+2*s_Margin)/2);
	m_GPB = 8;
	m_PPB = m_GPB * m_PPG;
	
	//Remove old data
	if(m_pData)
		delete[] m_pData;
	
	if(m_Texture.IsValid())
		Graphics()->UnloadTexture(&m_Texture);
	
	m_Blocks.clear();
	m_Glyphs.clear();
	
	//Init
	m_Width = m_PPB;
	m_Height = m_PPB;
	m_pData = new char[m_Width*m_Height];
	
	mem_zero(m_pData, m_Width*m_Height);
	m_Texture = Graphics()->LoadTextureRaw(m_Width, m_Height, 1, 1, CImageInfo::FORMAT_ALPHA, m_pData, CImageInfo::FORMAT_ALPHA, IGraphics::TEXLOAD_NOMIPMAPS);
	
	m_MemoryUsage = m_Width*m_Height;
	
	UpdateVersion();
}

void CTextRenderer::CGlyphCache::IncreaseCache()
{
	//Compute new sizes
	int NewWidth = m_Width;
	int NewHeight = m_Height;
	
	if(NewWidth > NewHeight)
		NewHeight <<= 1;
	else
		NewWidth <<= 1;
	
	//Allocate a new buffer and copy the old inside
	char* pNewData = new char[NewWidth*NewHeight];
	mem_zero(pNewData, NewWidth*NewHeight);
		
		//Block are arranged differently to be still accessible iterativly
	if(m_pData)
	{
		int OldNumBlockX = m_Width/m_PPB;
		int NewNumBlockX = NewWidth/m_PPB;
		
		for(int b=0; b<m_Blocks.size(); b++)
		{
			for(int j=0; j<m_PPB; j++)
			{
				int OldY = (b / OldNumBlockX)*m_PPB + j;
				int NewY = (b / NewNumBlockX)*m_PPB + j;
				
				for(int i=0; i<m_PPB; i++)
				{
					int OldX = (b % OldNumBlockX)*m_PPB + i;
					int NewX = (b % NewNumBlockX)*m_PPB + i;
					
					pNewData[NewY*NewWidth+NewX] = m_pData[OldY*m_Width+OldX];
				}
			}
		}
		
		delete[] m_pData;
		
		m_MemoryUsage -= m_Width*m_Height;
		
		if(m_Texture.IsValid())
			Graphics()->UnloadTexture(&m_Texture);
	}
	m_pData = pNewData;
	m_Width = NewWidth;
	m_Height = NewHeight;
	
	//Update Glyphs
	for(int i=0; i<m_Glyphs.size(); i++)
	{
		UpdateGlyph(&m_Glyphs[i]);
	}
	
	//Update texture
	
	m_Texture = Graphics()->LoadTextureRaw(m_Width, m_Height, 1, 1, CImageInfo::FORMAT_ALPHA, m_pData, CImageInfo::FORMAT_ALPHA, IGraphics::TEXLOAD_NOMIPMAPS);
	m_MemoryUsage += m_Width*m_Height;
	
	UpdateVersion();
}

int CTextRenderer::CGlyphCache::NewBlock(ivec2 Granularity)
{
	int MaxNumBlock = (m_Width * m_Height) / (m_PPB * m_PPB);
	
	if(MaxNumBlock <= m_Blocks.size())
		IncreaseCache();
	
	int BlockId = m_Blocks.size();
	m_Blocks.add(CBlock());
	CBlock* pBlock = &m_Blocks[BlockId];
	
	pBlock->m_Granularity = Granularity;
	pBlock->m_Size = 0;
	
	int NumGlyphX = m_GPB/Granularity.x;
	int NumGlyphY = m_GPB/Granularity.y;
	pBlock->m_MaxSize = NumGlyphX * NumGlyphY;
	
	return BlockId;
}

CTextRenderer::CGlyph* CTextRenderer::CGlyphCache::NewGlyph(int CharCode, int Width, int Height)
{	
	ivec2 Granularity;
	Granularity.x = (Width%m_PPG == 0 ? (Width / m_PPG) : (Width / m_PPG) + 1);
	Granularity.y = (Height%m_PPG == 0 ? (Height / m_PPG) : (Height / m_PPG) + 1);
	if(Granularity.x < 1)
		Granularity.x = 1;
	if(Granularity.y < 1)
		Granularity.y = 1;
	
	//First pass: find a free slot in blocks
		//We use backward iteration because non-full blockes are at the end
	for(int i=m_Blocks.size()-1; i>=0; i--)
	{
		if(m_Blocks[i].m_Granularity == Granularity)
		{
			if(!m_Blocks[i].IsFull())
			{
				CGlyph* pGlyph = &m_Glyphs[m_Glyphs.add(CGlyph(CharCode))];
				
				pGlyph->m_Width = Width;
				pGlyph->m_Height = Height;
				pGlyph->m_Granularity = Granularity;
				pGlyph->m_Block = i;
				pGlyph->m_BlockPos = m_Blocks[i].m_Size;
				UpdateGlyph(pGlyph);
		
				m_Blocks[pGlyph->m_Block].m_Size++;
				
				return pGlyph;
			}
			else
				break; //Only the last block of this granularity can be non-full
		}
	}
	
	//Second pass: find the oldest glyph with the same granularity
	int OldestTick = m_RenderTick;
	int OldestGlyph = -1;
	for(int i=0; i<m_Glyphs.size(); i++)
	{
		if(m_Glyphs[i].m_Granularity == Granularity && m_Glyphs[i].m_RenderTick < OldestTick)
		{
			OldestTick = m_Glyphs[i].m_RenderTick;
			OldestGlyph = i;
		}
	}
	
	if(OldestGlyph >= 0) //Replace the glyph
	{
		CGlyph Glyph = CGlyph(CharCode);
		Glyph.m_Block = m_Glyphs[OldestGlyph].m_Block;
		Glyph.m_BlockPos = m_Glyphs[OldestGlyph].m_BlockPos;
		
		m_Glyphs.remove_index(OldestGlyph);
		
		int GlyphId = m_Glyphs.add(Glyph);
		CGlyph* pGlyph = &m_Glyphs[GlyphId];
		
		pGlyph->m_Width = Width;
		pGlyph->m_Height = Height;
		pGlyph->m_Granularity = Granularity;
		UpdateGlyph(pGlyph);
		UpdateVersion();
		
		return pGlyph;
	}
	else //Add a new block
	{
		int BlockId = NewBlock(Granularity);

		CGlyph* pGlyph = &m_Glyphs[m_Glyphs.add(CGlyph(CharCode))];
		
		pGlyph->m_Width = Width;
		pGlyph->m_Height = Height;
		pGlyph->m_Granularity = Granularity;
		pGlyph->m_Block = BlockId;
		pGlyph->m_BlockPos = 0;
		
		UpdateGlyph(pGlyph);
		
		m_Blocks[pGlyph->m_Block].m_Size++;
		
		return pGlyph;
	}
}

CTextRenderer::CGlyph* CTextRenderer::CGlyphCache::FindGlyph(int CharCode)
{
	if(m_Glyphs.size() == 0)
		return 0;
	
	int iMin = 0;
	int iMax = m_Glyphs.size()-1;
	
	if(m_Glyphs[iMax].m_CharCode < CharCode)
		return 0;

	while((iMax-iMin) > 1)
	{
		int Pivot = iMin + (iMax-iMin-1)/2;
		if(m_Glyphs[Pivot].m_CharCode < CharCode)
			iMin = Pivot+1;
		else
			iMax = Pivot+1;
	}
	
	if(m_Glyphs[iMin].m_CharCode == CharCode)
		return &m_Glyphs[iMin];
	else
		return 0;
}

void CTextRenderer::CGlyphCache::UpdateTexture(CTextRenderer::CGlyph* pGlyph, const char* pData)
{
	int DataX = (pGlyph->m_pData - m_pData) % m_Width;
	int DataY = (pGlyph->m_pData - m_pData) / m_Width;
	
	Graphics()->LoadTextureRawSub(
		m_Texture,
		DataX, DataY,
		pGlyph->m_Granularity.x * m_PPG,
		pGlyph->m_Granularity.y * m_PPG,
		CImageInfo::FORMAT_ALPHA,
		pData
	);
}

/* TEXT CACHE *********************************************************/

CTextRenderer::CTextCache::CTextCache()
{
	m_GlyphCacheId = -1;
}

void CTextRenderer::CTextCache::Reset()
{
	m_GlyphCacheId = -1;
	m_Quads.clear();
	m_TextWidth = 0.0f;
}

/* TEXT RENDERER ******************************************************/

CTextRenderer::CTextRenderer(CKernel* pKernel) :
	CKernel::CComponent(pKernel)
{
	
}

CTextRenderer::~CTextRenderer()
{
	for(int i=0; i<m_Fonts.size(); i++)
		delete m_Fonts[i];
	
	for(int i=0; i<m_GlyphCaches.size(); i++)
		delete m_GlyphCaches[i];
		
	m_Fonts.clear();
	m_GlyphCaches.clear();
}

static int s_aFontSizes[] = {8,9,10,11,12,13,14,15,16,17,18,19,20,36,64};
//~ static int s_aFontSizes[] = {30};
#define NUM_FONT_SIZES (sizeof(s_aFontSizes)/sizeof(int))

bool CTextRenderer::Init()
{	
	//Cleaning
	for(int i=0; i<m_Fonts.size(); i++)
		delete m_Fonts[i];
	
	for(int i=0; i<m_GlyphCaches.size(); i++)
		delete m_GlyphCaches[i];
	
	m_Fonts.clear();
	m_GlyphCaches.clear();
	
	//Init caches
	m_GlyphCaches.set_size(NUM_FONT_SIZES);
	for(int i=0; i<m_GlyphCaches.size(); i++)
	{
		m_GlyphCaches[i] = new CGlyphCache(Kernel());
		m_GlyphCaches[i]->Init(s_aFontSizes[i]);
	}
	
	//Load FreeType
	if(FT_Init_FreeType(&m_FTLibrary) != FT_Err_Ok)
		return false;
	
	//Load Font
	if(!LoadFont("fonts/DejaVuSans.ttf"))
		return false;
	LoadFont("fonts/NotoSansCJKjp-Medium.ttf");
	
	return true;
}

bool CTextRenderer::LoadFont(const char* pFilename)
{
	char aFullPath[512];
	IOHANDLE File = Storage()->OpenFile(pFilename, IOFLAG_READ, IStorage::TYPE_ALL, aFullPath, sizeof(aFullPath));
	if(!File)
	{
		dbg_msg("TextRenderer", "Can't open %s", pFilename);
		return false;
	}
	
	io_close(File);
	
	CFont* pFont = new CFont();
	str_copy(pFont->m_aFilename, aFullPath, sizeof(pFont->m_aFilename));
	
	if(FT_New_Face(m_FTLibrary, pFont->m_aFilename, 0, &pFont->m_FtFace) != FT_Err_Ok)
	{
		dbg_msg("TextRenderer", "Can't create a regular font from %s", pFilename);
		delete pFont;
		return false;
	}
	
	m_Fonts.add(pFont);
	
	return true;
}

char s_aGlyphBuffer[128*128];

CTextRenderer::CGlyph* CTextRenderer::LoadGlyph(CGlyphCache* pCache, int CharCode)
{
	if(m_Fonts.size() == 0)
	{
		dbg_msg("TextRenderer", "No font loaded");
		return 0;
	}
	
	int GlyphIndex = 0;
	int FontIndex = 0;
	while(FontIndex < m_Fonts.size())
	{
		GlyphIndex = FT_Get_Char_Index(m_Fonts[FontIndex]->m_FtFace, CharCode);
		if(GlyphIndex != 0)
			break;
		FontIndex++;
	}
	
	//No glyph found. Print the ".nodef" glyph of the first font instead
	if(GlyphIndex == 0)
		FontIndex = 0;
	
	if(FT_Set_Pixel_Sizes(m_Fonts[FontIndex]->m_FtFace, 0, pCache->m_FontSize) != FT_Err_Ok)
	{
		dbg_msg("TextRenderer", "Can't set pixel size %d", pCache->m_FontSize);
		return 0;
	}
	if(FT_Load_Glyph(m_Fonts[FontIndex]->m_FtFace, GlyphIndex, FT_LOAD_RENDER|FT_LOAD_NO_BITMAP) != FT_Err_Ok)
	{
		dbg_msg("TextRenderer", "Can't load glyph %d", GlyphIndex);
		return 0;
	}
	
	FT_Bitmap* pBitmap = &m_Fonts[FontIndex]->m_FtFace->glyph->bitmap;
	int BitmapWidth = pBitmap->width;
	int BitmapHeight = pBitmap->rows;
	int BBWidth = BitmapWidth+2*s_Margin;
	int BBHeight = BitmapHeight+2*s_Margin;
	
	CTextRenderer::CGlyph* pGlyph = pCache->NewGlyph(CharCode, BBWidth, BBHeight);
	
	pGlyph->m_CharCode = CharCode;
	pGlyph->m_AdvanceX = (m_Fonts[FontIndex]->m_FtFace->glyph->advance.x >> 6);
	pGlyph->m_OffsetX = m_Fonts[FontIndex]->m_FtFace->glyph->bitmap_left - s_Margin;
	pGlyph->m_OffsetY = m_Fonts[FontIndex]->m_FtFace->glyph->bitmap_top + s_Margin;
	
	if(pBitmap->pixel_mode == FT_PIXEL_MODE_GRAY)
	{
		for(int j = 0; j < BitmapHeight; j++)
		{
			for(int i = 0; i < BitmapWidth; i++)
			{
				int CacheDataIndex = (j+s_Margin)*pCache->m_Width + (i+s_Margin);
				pGlyph->m_pData[CacheDataIndex] = pBitmap->buffer[j*pBitmap->pitch+i];
			}
		}
	}
	else if(pBitmap->pixel_mode == FT_PIXEL_MODE_MONO)
	{
		for(int j = 0; j < BitmapHeight; j++)
		{
			for(int i = 0; i < BitmapWidth; i++)
			{
				int CacheDataIndex = (j+s_Margin)*pCache->m_Width + (i+s_Margin);
				if(pBitmap->buffer[j*pBitmap->pitch+i/8]&(1<<(7-(i%8))))
					pGlyph->m_pData[CacheDataIndex] = 255;
				else
					pGlyph->m_pData[CacheDataIndex] = 0;
			}
		}
	}
	
	mem_zero(s_aGlyphBuffer, (pGlyph->m_Granularity.x * pCache->m_PPG) * (pGlyph->m_Granularity.y * pCache->m_PPG));
	
	for(int j = s_Margin; j < pGlyph->m_Height-s_Margin; j++)
	{
		for(int i = s_Margin; i < pGlyph->m_Width-s_Margin; i++)
		{
			s_aGlyphBuffer[j*(pGlyph->m_Granularity.x * pCache->m_PPG) + i] = pGlyph->m_pData[j*pCache->m_Width + i];
		}
	}
	
	pCache->UpdateTexture(pGlyph, s_aGlyphBuffer);
	
	return pGlyph;
}

CTextRenderer::CGlyph* CTextRenderer::FindGlyph(CGlyphCache* pCache, int CharCode)
{
	return pCache->FindGlyph(CharCode);
}


CTextRenderer::CGlyph* CTextRenderer::GetGlyph(CGlyphCache* pCache, int CharCode)
{
	CTextRenderer::CGlyph* pGlyph = FindGlyph(pCache, CharCode);
	
	//Load Glyph
	if(!pGlyph)
		pGlyph = LoadGlyph(pCache, CharCode);
	
	//Update timer
	if(pGlyph)
		pGlyph->m_RenderTick = m_RenderTick;
	
	return pGlyph;
}

void CTextRenderer::UpdateTextCache(const char* pText, ivec2 BoxSize, float FontSize, CTextCache* pTextCache)
{
	bool NeedUpdate = false;
	
	if(pTextCache->m_GlyphCacheId < 0 || pTextCache->m_GlyphCacheId >= m_GlyphCaches.size())
		NeedUpdate = true;
	else if(pTextCache->m_BoxSize != BoxSize)
		NeedUpdate = true;
	else if(pTextCache->m_FontSize != FontSize)
		NeedUpdate = true;
	else if(pTextCache->m_GlyphCacheVersion != m_GlyphCaches[pTextCache->m_GlyphCacheId]->m_Version)
		NeedUpdate = true;
	
	if(!NeedUpdate)
		return;
	
	pTextCache->Reset();
	
	pTextCache->m_BoxSize = BoxSize;
	pTextCache->m_FontSize = FontSize;
	
	//Search the appropriate cached font size to render the text
	CGlyphCache* pGlyphCache = 0;
	for(int i=0; i<m_GlyphCaches.size(); i++)
	{
		pGlyphCache = m_GlyphCaches[i];
		pTextCache->m_GlyphCacheId = i;
		
		if(m_GlyphCaches[i]->m_FontSize >= FontSize)
			break;
	}
	if(!pGlyphCache)
		return;
	
	float RelativeSize = FontSize / pGlyphCache->m_FontSize;
	
	int Length = str_length(pText);	
	
	//TODO: add text shaping and text layout here
	
	//Here, we use two passes in order to make sure that the glyph cache is in the same state for all letters
		//First pass: Update the glyph cache
	const char* pEnd = pText + Length;
	const char* pIter = pText;
	while(pIter < pEnd)
	{
		int CharCode = str_utf8_decode(&pIter);
		GetGlyph(pGlyphCache, CharCode);
	}
	
		//Second pass: Generate the list of quads
	pIter = pText;
	float PosY = BoxSize.y/2 + static_cast<int>(FontSize*0.4f); //TODO: find a way to not hard code the generic glyph height.
	float PosX = 0.0f;
	int TextIter = 0;
	while(pIter < pEnd)
	{
		int CharCode = str_utf8_decode(&pIter);
		
		CGlyph* pGlyph = GetGlyph(pGlyphCache, CharCode);
		if(pGlyph)
		{
			CTextCache::CQuad* pQuad = &pTextCache->m_Quads[pTextCache->m_Quads.add(CTextCache::CQuad())];
			pQuad->m_AdvancePos = vec2(PosX, PosY);
			pQuad->m_QuadPos = vec2(PosX + pGlyph->m_OffsetX*RelativeSize, PosY - pGlyph->m_OffsetY*RelativeSize);
			pQuad->m_Size = vec2(pGlyph->m_Width*RelativeSize, pGlyph->m_Height*RelativeSize);
			pQuad->m_UVMin = pGlyph->m_UVMin;
			pQuad->m_UVMax = pGlyph->m_UVMax;
			pQuad->m_TextIter = TextIter;
			
			PosX += pGlyph->m_AdvanceX*RelativeSize;
		}
		
		TextIter++;
	}
	
	pTextCache->m_TextWidth = PosX;
	pTextCache->m_GlyphCacheVersion = pGlyphCache->m_Version;
}

CTextRenderer::CTextCursor CTextRenderer::GetTextCursorFromPosition(const char* pText, ivec2 MousePosition, gui::CRect Rect, float FontSize, CTextCache* pTextCache)
{
	UpdateTextCache(pText, ivec2(Rect.w, Rect.h), FontSize, pTextCache);
	
	CTextCursor Cursor;
	Cursor.m_TextIter = 0;
	Cursor.m_Position.x = Rect.x;
	Cursor.m_Position.y = Rect.y;
	
	if(pTextCache->m_Quads.size() <= 0)
		return Cursor;
	
	int NearestQuad = 0;
	float CharDistance = 999999.0f;
	vec2 TestPos = vec2(MousePosition.x - Rect.x, MousePosition.y - Rect.y);
	for(int i=0; i<pTextCache->m_Quads.size(); i++)
	{
		float Dist = fabs(pTextCache->m_Quads[i].m_QuadPos.x + pTextCache->m_Quads[i].m_Size.x/2.0f - TestPos.x);
		if(Dist < CharDistance)
		{
			NearestQuad = i;
			CharDistance = Dist;
		}
	}
	
	Cursor.m_TextIter = pTextCache->m_Quads[NearestQuad].m_TextIter;
	Cursor.m_Position.x = Rect.x + pTextCache->m_Quads[NearestQuad].m_AdvancePos.x;
	if(TestPos.x > pTextCache->m_Quads[NearestQuad].m_QuadPos.x + pTextCache->m_Quads[NearestQuad].m_Size.x/2.0f)
	{
		Cursor.m_TextIter++;
		if(NearestQuad == pTextCache->m_Quads.size()-1)
			Cursor.m_Position.x = Rect.x + pTextCache->m_TextWidth;
	}
	
	return Cursor;
}

CTextRenderer::CTextCursor CTextRenderer::GetTextCursorFromTextIter(const char* pText, int TextIter, gui::CRect Rect, float FontSize, CTextCache* pTextCache)
{
	UpdateTextCache(pText, ivec2(Rect.w, Rect.h), FontSize, pTextCache);
	
	CTextCursor Cursor;
	Cursor.m_TextIter = TextIter;
	Cursor.m_Position.x = Rect.x + pTextCache->m_TextWidth;
	Cursor.m_Position.y = Rect.y;
	
	for(int i=0; i<pTextCache->m_Quads.size(); i++)
	{
		if(pTextCache->m_Quads[i].m_TextIter == TextIter)
		{
			Cursor.m_Position.x = Rect.x + pTextCache->m_Quads[i].m_AdvancePos.x;
			break;
		}
	}
	
	return Cursor;
}

float CTextRenderer::GetTextWidth(const char* pText, gui::CRect Rect, float FontSize, CTextCache* pTextCache)
{
	UpdateTextCache(pText, ivec2(Rect.w, Rect.h), FontSize, pTextCache);
	
	return pTextCache->m_TextWidth;
}

void CTextRenderer::DrawText(const char* pText, gui::CRect Rect, float FontSize, vec4 Color, CTextCache* pTextCache)
{
	UpdateTextCache(pText, ivec2(Rect.w, Rect.h), FontSize, pTextCache);
	
	Graphics()->TextureSet(m_GlyphCaches[pTextCache->m_GlyphCacheId]->m_Texture);
	Graphics()->QuadsBegin();
	for(int p=0; p<2; p++)
	{
		vec2 Offset = 0.0f;
		if(p==0)
		{
			Graphics()->SetColor(0.0f, 0.0f, 0.0f, 0.5f*Color.a);
			Offset = vec2(0.0f, 1.0f);
		}
		else
			Graphics()->SetColor(Color, true);
		
		for(int i=0; i<pTextCache->m_Quads.size(); i++)
		{
			Graphics()->QuadsSetSubset(
				pTextCache->m_Quads[i].m_UVMin.x,
				pTextCache->m_Quads[i].m_UVMin.y,
				pTextCache->m_Quads[i].m_UVMax.x,
				pTextCache->m_Quads[i].m_UVMax.y
			);
			IGraphics::CQuadItem QuadItem(
				Rect.x + pTextCache->m_Quads[i].m_QuadPos.x + Offset.x,
				Rect.y + pTextCache->m_Quads[i].m_QuadPos.y + Offset.y,
				pTextCache->m_Quads[i].m_Size.x,
				pTextCache->m_Quads[i].m_Size.y
			);
			Graphics()->QuadsDrawTL(&QuadItem, 1);
		}
	}
	Graphics()->QuadsEnd();
}

void CTextRenderer::Update()
{
	m_RenderTick++;
	
	for(int i=0; i<m_GlyphCaches.size(); i++)
		m_GlyphCaches[i]->m_RenderTick = m_RenderTick;
}

void CTextRenderer::Debug_DrawCaches()
{
	int Scale = 2;
	int PosX = 0;
	int PosY = 0;
	int MaxX = 0;
	
	for(int i=0; i<m_GlyphCaches.size(); i++)
	{
		if(PosY + m_GlyphCaches[i]->m_Height/Scale > Graphics()->ScreenHeight())
		{
			PosY = 0;
			PosX += MaxX;
			MaxX = 0;
		}
		else if(MaxX < m_GlyphCaches[i]->m_Width/Scale)
		{
			MaxX = m_GlyphCaches[i]->m_Width/Scale;
		}
		
		{
			Graphics()->TextureClear();
			Graphics()->QuadsBegin();
			Graphics()->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
			
			Graphics()->QuadsSetSubset(0.0f, 0.0f, 1.0f, 1.0f);
			IGraphics::CQuadItem QuadItem(PosX, PosY, m_GlyphCaches[i]->m_Width/Scale, m_GlyphCaches[i]->m_Height/Scale);
			Graphics()->QuadsDrawTL(&QuadItem, 1);
			
			Graphics()->QuadsEnd();
		}
		{
			Graphics()->TextureSet(m_GlyphCaches[i]->m_Texture);
			Graphics()->QuadsBegin();
			Graphics()->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
			
			Graphics()->QuadsSetSubset(0.0f, 0.0f, 1.0f, 1.0f);
			IGraphics::CQuadItem QuadItem(PosX, PosY, m_GlyphCaches[i]->m_Width/Scale, m_GlyphCaches[i]->m_Height/Scale);
			Graphics()->QuadsDrawTL(&QuadItem, 1);
			
			Graphics()->QuadsEnd();
		}
		{
			Graphics()->TextureClear();
			Graphics()->LinesBegin();
			Graphics()->SetColor(1.0f, 1.0f, 0.0f, 1.0f);
			
			vec2 minCorner = vec2(PosX-0.5f, PosY-0.5f);
			vec2 maxCorner = vec2(PosX+m_GlyphCaches[i]->m_Width/Scale+0.5f, PosY+m_GlyphCaches[i]->m_Height/Scale+0.5f);

			{
				IGraphics::CLineItem Lines[4];
				Lines[0] = IGraphics::CLineItem(minCorner.x, minCorner.y, maxCorner.x, minCorner.y);
				Lines[1] = IGraphics::CLineItem(maxCorner.x, minCorner.y, maxCorner.x, maxCorner.y);
				Lines[2] = IGraphics::CLineItem(maxCorner.x, maxCorner.y, minCorner.x, maxCorner.y);
				Lines[3] = IGraphics::CLineItem(minCorner.x, maxCorner.y, minCorner.x, minCorner.y);
				
				Graphics()->LinesDraw(Lines, 4);
			}
			
			Graphics()->LinesEnd();
		}
		
		PosY += m_GlyphCaches[i]->m_Height/Scale;
	}
}

}
