#include <cstdlib>
#include <cstdio>
#include "Glyph.h"

#define RASTER_AREA_SIZE 256

class GlyphRasterMaster
{
private:
	//INDEXING IS TOP LEFT TO BOTTOM RIGHT [x][y]
	char rasterArea[RASTER_AREA_SIZE][RASTER_AREA_SIZE];

	void clearRasterArea(int xin,int yin,int width,int height);
	void drawGlyph(Glyph *glyph,int rPosX, int rPosY);
	void drawGeneralGlyph(Glyph *glyph,int rPosX, int rPosY);
	void drawFracGlyph(Glyph *glyph,int rPosX, int rPosY);
	void drawSqrtGlyph(Glyph *glyph,int rPosX, int rPosY);
	void drawRootGlyph(Glyph *glyph,int rPosX, int rPosY);
	void drawSubscriptGlyph(Glyph *glyph,int rPosX, int rPosY);
	void drawSuperscriptGlyph(Glyph *glyph,int rPosX, int rPosY);
	void drawSuperSubGlyph(Glyph *glyph,int rPosX, int rPosY);
	void drawPiGlyph(Glyph *glyph,int rPosX, int rPosY);
	void drawTextGlyph(Glyph *glyph,int rPosX, int rPosY);
public:
	void initialize();
	void rasterGlyphFromPosition(Glyph *glyph,int rPosX, int rPosY);
	void flushRasterAreaToFile(const char *fileOut);
};