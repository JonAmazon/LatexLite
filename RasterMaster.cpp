#include "RasterMaster.h"

void GlyphRasterMaster::initialize()
{
	for(int y = 0; y < RASTER_AREA_SIZE; ++y)
	{
		for(int x = 0; x < RASTER_AREA_SIZE; ++x)
		{
			rasterArea[x][y] = ' ';
		}
	}
}

void GlyphRasterMaster::clearRasterArea(int xin,int yin,int width,int height)
{
	for(int y = yin; y < yin+height; ++y)
	{
		for(int x = xin; x < xin+width; ++x)
		{
			rasterArea[x][y] = ' ';
		}
	}
}

void GlyphRasterMaster::rasterGlyphFromPosition(Glyph *glyph,int rPosX, int rPosY)
{
	int gSizeX,gSizeY;
	
	gSizeX = glyph->getSizeX();
	gSizeY = glyph->getSizeY();
	clearRasterArea(rPosX,rPosY,gSizeX,gSizeY);
	drawGlyph(glyph,rPosX,rPosY);
}

void GlyphRasterMaster::flushRasterAreaToFile(const char *fileOut)
{
	FILE *fout = fopen(fileOut,"w");

	for(int y = 0; y < RASTER_AREA_SIZE; ++y)
	{
		for(int x = 0; x < RASTER_AREA_SIZE; ++x)
		{
			fprintf(fout,"%c",rasterArea[x][y]);
			if(x == RASTER_AREA_SIZE-1)
			{
				fprintf(fout,"\n");
			}
		}
	}

	fclose(fout);
}

void GlyphRasterMaster::drawGlyph(Glyph *glyph,int rPosX, int rPosY)
{
	switch(glyph->getGlyphType())
	{
		case General:
			{
				drawGeneralGlyph(glyph,rPosX,rPosY);
				break;
			}
		case Frac:
			{
				drawFracGlyph(glyph,rPosX,rPosY);
				break;
			}
		case Sqrt:
			{
				drawSqrtGlyph(glyph,rPosX,rPosY);
				break;
			}
		case Root:
			{
				drawRootGlyph(glyph,rPosX,rPosY);
				break;
			}
		case Subscript:
			{
				drawSubscriptGlyph(glyph,rPosX,rPosY);
				break;
			}
		case Superscript:
			{
				drawSuperscriptGlyph(glyph,rPosX,rPosY);
				break;
			}
		case SuperSub:
			{
				drawSuperSubGlyph(glyph,rPosX,rPosY);
				break;
			}
		case Pi:
			{
				drawPiGlyph(glyph,rPosX,rPosY);
				break;
			}
		case Text:
			{
				drawTextGlyph(glyph,rPosX,rPosY);
				break;
			}
	}
}

void GlyphRasterMaster::drawGeneralGlyph(Glyph *glyph,int rPosX, int rPosY)
{
	GeneralGlyph *genGlyph = dynamic_cast<GeneralGlyph*>(glyph);
	Glyph *child;
	int nChild = genGlyph->getNumberOfChildren();

	int pSizeUp = glyph->getSizeUp();
	int cSizeUp,cSizeDown,cSizeX,cSizeY;
	int cPosX,cPosY;

	cPosX = rPosX;
	cPosY = rPosY;
	for(int c = 0; c < nChild; ++c)
	{
		child = genGlyph->getChildGlyph(c);
		cSizeUp = child->getSizeUp();
		cSizeDown = child->getSizeDown();
		cSizeX = child->getSizeX();
		cSizeY = child->getSizeY();

		cPosY = rPosY + (pSizeUp - cSizeUp);
		drawGlyph(child,cPosX,cPosY);
		cPosX += cSizeX;
	}
}

void GlyphRasterMaster::drawFracGlyph(Glyph *glyph,int rPosX, int rPosY)
{
	FracGlyph *fracGlyph = dynamic_cast<FracGlyph*>(glyph);
	GeneralGlyph *num,*denom;
	int fSizeUp = glyph->getSizeUp();
	int fSizeX = glyph->getSizeX();
	int nSizeX,nOffset;
	int dSizeX,dOffset;

	num = fracGlyph->getNumerator();
	nSizeX = num->getSizeX();
	denom = fracGlyph->getDenominator();
	dSizeX = denom->getSizeX();

	nOffset = (fSizeX - nSizeX)/2;
	dOffset = (fSizeX - dSizeX)/2;

	drawGlyph(num,rPosX+nOffset,rPosY);
	for(int x = 0; x < fSizeX; ++x)
	{
		rasterArea[rPosX+x][rPosY+fSizeUp-1] = '—';
	}
	drawGlyph(denom,rPosX+dOffset,rPosY+fSizeUp);
}

void GlyphRasterMaster::drawSqrtGlyph(Glyph *glyph,int rPosX, int rPosY)
{
	SqrtGlyph *sqrtGlyph = dynamic_cast<SqrtGlyph*>(glyph);
	GeneralGlyph *arg = sqrtGlyph->getArg();

	int gSizeX = sqrtGlyph->getSizeX();
	int gSizeY = sqrtGlyph->getSizeY();
	int aSizeX = arg->getSizeX();
	int aSizeY = arg->getSizeY();

	int slashStartX = 0;
	int slashStartY = (gSizeY-1);
	int aStartX = slashStartX+aSizeY;
	int aStartY = slashStartY-aSizeY+1;

	rasterArea[rPosX+slashStartX][rPosY+slashStartY] = 'V';
	for(int slashes = 1; slashes < aSizeY; ++slashes)
	{
		rasterArea[rPosX+slashStartX+(slashes)][rPosY+slashStartY-(slashes)] = '/';
	}
	for(int x = 0; x < aSizeX; ++x)
	{
		rasterArea[rPosX+aStartX+x][rPosY+aStartY-1] = '_';
	}
	drawGlyph(arg,rPosX+aStartX,rPosY+aStartY);
}

void GlyphRasterMaster::drawRootGlyph(Glyph *glyph,int rPosX, int rPosY)
{
	RootGlyph *rootGlyph = dynamic_cast<RootGlyph*>(glyph);
	GeneralGlyph *rad = rootGlyph->getRad();
	GeneralGlyph *arg = rootGlyph->getArg();
	
	int gSizeX = rootGlyph->getSizeX();
	int gSizeY = rootGlyph->getSizeY();
	int rSizeX = rad->getSizeX();
	int rSizeY = rad->getSizeY();
	int aSizeX = arg->getSizeX();
	int aSizeY = arg->getSizeY();

	int slashStartX = rSizeX-1;
	int slashStartY = (gSizeY-1);
	int aStartX = slashStartX+aSizeY;
	int aStartY = slashStartY-aSizeY+1;
	int rStartX = 0;
	int rStartY = (rSizeY > aSizeY)?0:(aSizeY+1)-(rSizeY+1);

	rasterArea[rPosX+slashStartX][rPosY+slashStartY] = 'V';
	for(int slashes = 1; slashes < aSizeY; ++slashes)
	{
		rasterArea[rPosX+slashStartX+(slashes)][rPosY+slashStartY-(slashes)] = '/';
	}
	for(int x = 0; x < aSizeX; ++x)
	{
		rasterArea[rPosX+aStartX+x][rPosY+aStartY-1] = '_';
	}

	drawGlyph(rad,rPosX+rStartX,rPosY+rStartY);
	drawGlyph(arg,rPosX+aStartX,rPosY+aStartY);
}

void GlyphRasterMaster::drawSubscriptGlyph(Glyph *glyph,int rPosX, int rPosY)
{
	SubscriptGlyph *subscriptGlyph = dynamic_cast<SubscriptGlyph*>(glyph);
	Glyph *arg = subscriptGlyph->getArgument();
	GeneralGlyph *subscript =subscriptGlyph->getSubscript();

	int aSizeX = arg->getSizeX();
	int aSizeY = arg->getSizeY();

	drawGlyph(arg,rPosX,rPosY);
	drawGlyph(subscript,rPosX+aSizeX,rPosY+aSizeY);
}

void GlyphRasterMaster::drawSuperscriptGlyph(Glyph *glyph,int rPosX, int rPosY)
{
	SuperscriptGlyph *superscriptGlyph = dynamic_cast<SuperscriptGlyph*>(glyph);
	Glyph *arg = superscriptGlyph->getArgument();
	GeneralGlyph *superscript = superscriptGlyph->getSuperscript();

	int aSizeX = arg->getSizeX();
	int superSizeY = superscript->getSizeY();

	drawGlyph(arg,rPosX,rPosY+superSizeY);
	drawGlyph(superscript,rPosX+aSizeX,rPosY);
}

void GlyphRasterMaster::drawSuperSubGlyph(Glyph *glyph,int rPosX, int rPosY)
{
	SubSupGlyph *subsupGlyph = dynamic_cast<SubSupGlyph*>(glyph);
	Glyph *arg = subsupGlyph->getArgument();
	GeneralGlyph *superscript = subsupGlyph->getSuperscript();
	GeneralGlyph *subscript  = subsupGlyph->getSubscript();

	int aSizeX = arg->getSizeX();
	int aSizeY = arg->getSizeY();
	int superSizeY = superscript->getSizeY();

	drawGlyph(superscript,rPosX+aSizeX,rPosY);
	drawGlyph(arg,rPosX,rPosY+superSizeY);
	drawGlyph(subscript,rPosX+aSizeX,rPosY+superSizeY+aSizeY);
}

void GlyphRasterMaster::drawPiGlyph(Glyph *glyph,int rPosX, int rPosY)
{
	rasterArea[rPosX][rPosY] = 227;
}

void GlyphRasterMaster::drawTextGlyph(Glyph *glyph,int rPosX, int rPosY)
{
	TextGlyph *textGlyph = dynamic_cast<TextGlyph*>(glyph);
	char text = textGlyph->getText();

	rasterArea[rPosX][rPosY] = text;
}