#include "RasterMaster.h"

int main()
{
	char buffer[1024];
	int equationCount = 0;
	int rasterPosY = 0;

	GeneralGlyph* equation[32];
	GlyphRasterMaster renderer;

	FILE *fin;
	fin = fopen("input.txt","r");
	
	renderer.initialize();
	while(fgets(buffer,1024,fin))
	{
		equation[equationCount] = new GeneralGlyph;
		equation[equationCount]->parseGlyphFromInput(buffer);
		equation[equationCount]->calculateSize();

		renderer.rasterGlyphFromPosition(equation[equationCount],2,rasterPosY);

		rasterPosY += equation[equationCount]->getSizeY()+10;
		equationCount++;
	}

	renderer.flushRasterAreaToFile("equation.txt");

	getchar();

	return 0;
}