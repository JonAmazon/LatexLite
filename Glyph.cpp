#include "Glyph.h"

GeneralGlyph::GeneralGlyph()
{
	numberOfChildren = 0;
	type = General;
}

void GeneralGlyph::calculateSize()
{
	sizeX = 0;
	sizeY = 0;
	sizeUp = 0;
	sizeDown = 0;

	for(int g = 0; g < numberOfChildren; ++g)
	{
		children[g]->calculateSize();

		sizeX += children[g]->getSizeX();
		sizeUp = MAX(sizeUp,children[g]->getSizeUp());
		sizeDown = MAX(sizeDown,children[g]->getSizeDown());
	}
	sizeY = sizeUp+sizeDown;
}

void GeneralGlyph::buildText(const char *input)
{
	int Idx = 0;
	while('\0' != input[Idx])
	{
		children[numberOfChildren] = new TextGlyph(input[Idx]);
		numberOfChildren++;
		Idx++;
	}
}

void GeneralGlyph::buildFrac(const char *inputNum, const char* inputDenom)
{
	children[numberOfChildren] = new FracGlyph(inputNum,inputDenom);
	numberOfChildren++;
}

void GeneralGlyph::buildSqrt(const char *inputArg)
{
	children[numberOfChildren] = new SqrtGlyph(inputArg);
	numberOfChildren++;
}

void GeneralGlyph::buildRoot(const char *inputRad, const char* inputArg)
{
	children[numberOfChildren] = new RootGlyph(inputRad,inputArg);
	numberOfChildren++;
}

void GeneralGlyph::buildSubscript(const char *input)
{
	Glyph *prev,*tempArg;
	GeneralGlyph *tempSuper;

	if(numberOfChildren == 0)
	{
		printf("BuildSubscript Error: no argument, ignoring glyph\n");
	}
	else
	{
		numberOfChildren--;
		prev = children[numberOfChildren];	

		if(prev->getGlyphType() == Superscript)
		{
			printf("BuildSubscript: Merging with previous superscript.\n");
			children[numberOfChildren] = new SubSupGlyph;
			tempArg = dynamic_cast<SuperscriptGlyph*>(prev)->getArgument();
			tempSuper = dynamic_cast<SuperscriptGlyph*>(prev)->getSuperscript();
			delete [] prev;

			dynamic_cast<SubSupGlyph*>(children[numberOfChildren])->initializeSuperToSubSup(tempArg,tempSuper,input);
		}
		else
		{
			children[numberOfChildren] = new SubscriptGlyph(prev,input);
		}
		numberOfChildren++;
	}
}

void GeneralGlyph::buildSuperscript(const char *input)
{
	Glyph *prev,*tempArg;
	GeneralGlyph *tempSub;

	if(numberOfChildren == 0)
	{
		printf("BuildSuperscript Error: no argument, ignoring glyph\n");
	}
	else
	{
		numberOfChildren--;
		prev = children[numberOfChildren];	

		if(prev->getGlyphType() == Subscript)
		{
			printf("BuildSuperscript: Merging with previous subscript.\n");
			children[numberOfChildren] = new SubSupGlyph;
			tempArg = dynamic_cast<SubscriptGlyph*>(prev)->getArgument();
			tempSub = dynamic_cast<SubscriptGlyph*>(prev)->getSubscript();
			delete [] prev;

			dynamic_cast<SubSupGlyph*>(children[numberOfChildren])->initializeSubToSubSup(tempArg,tempSub,input);
		}
		else
		{
			children[numberOfChildren] = new SuperscriptGlyph(prev,input);
		}
		numberOfChildren++;
	}
}

void GeneralGlyph::buildPi()
{
	children[numberOfChildren] = new PiGlyph;
	numberOfChildren++;
}

void GeneralGlyph::parseGlyphFromInput(const char *input)
{
	Parser parser(input);

	parser.findGlyph();
	while(parser.foundValidGlyph())
	{
		switch(parser.getGlyphType())
		{
		case Text:
			{
				buildText(parser.getBuffer1());
				break;
			}
		case Frac:
			{
				buildFrac(parser.getBuffer1(),parser.getBuffer2());
				break;
			}
		case Sqrt:
			{
				buildSqrt(parser.getBuffer1());
				break;
			}
		case Root:
			{
				buildRoot(parser.getBuffer1(),parser.getBuffer2());
				break;
			}
		case Subscript:
			{
				buildSubscript(parser.getBuffer1());
				break;
			}
		case Superscript:
			{
				buildSuperscript(parser.getBuffer1());
				break;
			}
		case Pi:
			{
				buildPi();
				break;
			}
		default:
			{
				break;
			}
		}

		parser.findGlyph();
	}
}

TextGlyph::TextGlyph(char t)
{
	text = t;
	type = Text;
}

void TextGlyph::calculateSize()
{
	sizeX = 1;
	sizeY = 1;
	sizeUp = 1;
	sizeDown = 0;
}

FracGlyph::FracGlyph()
{}

FracGlyph::FracGlyph(const char *num, const char *denom)
{
	numerator = new GeneralGlyph;
	denominator = new GeneralGlyph;

	numerator->parseGlyphFromInput(num);
	denominator->parseGlyphFromInput(denom);

	type = Frac;
}

void FracGlyph::calculateSize()
{
	numerator->calculateSize();
	denominator->calculateSize();

	sizeDown = denominator->getSizeY();
	sizeUp = numerator->getSizeY() + 1;
	sizeX = MAX(numerator->getSizeX(),denominator->getSizeX());
	sizeY = sizeUp + sizeDown;
}

SqrtGlyph::SqrtGlyph()
{}

SqrtGlyph::SqrtGlyph(const char *arg)
{
	argument = new GeneralGlyph;

	argument->parseGlyphFromInput(arg);

	type = Sqrt;
}

void SqrtGlyph::calculateSize()
{
	argument->calculateSize();

	sizeUp = argument->getSizeY() + 1;
	sizeDown = 0;
	sizeX = argument->getSizeX() + argument->getSizeY();
	sizeY = sizeUp + sizeDown;
}

RootGlyph::RootGlyph()
{}

RootGlyph::RootGlyph(const char *rad, const char *arg)
{
	radical = new GeneralGlyph;
	argument = new GeneralGlyph;

	radical->parseGlyphFromInput(rad);
	argument->parseGlyphFromInput(arg);

	type = Root;
}

void RootGlyph::calculateSize()
{
	radical->calculateSize();
	argument->calculateSize();

	sizeUp = MAX(argument->getSizeY() + 1,radical->getSizeY() + 1);
	sizeDown = 0;
	sizeX = argument->getSizeX() + argument->getSizeY() + radical->getSizeX() - 1;
	sizeY = sizeUp + sizeDown;
}

SubscriptGlyph::SubscriptGlyph(Glyph *argIn,const char *input)
{
	argument = argIn;
	subscript = new GeneralGlyph;

	subscript->parseGlyphFromInput(input);

	type = Subscript;
};

void SubscriptGlyph::calculateSize()
{
	argument->calculateSize();
	subscript->calculateSize();

	sizeUp = argument->getSizeY();
	sizeDown = subscript->getSizeY();
	sizeX = argument->getSizeX() + subscript->getSizeX();
	sizeY = sizeUp + sizeDown;
}

SuperscriptGlyph::SuperscriptGlyph(Glyph *argIn,const char *input)
{
	argument = argIn;
	superscript = new GeneralGlyph;

	superscript->parseGlyphFromInput(input);

	type = Superscript;
};

void SuperscriptGlyph::calculateSize()
{
	argument->calculateSize();
	superscript->calculateSize();

	sizeUp = argument->getSizeY() + superscript->getSizeY();
	sizeDown = 0;
	sizeX = argument->getSizeX() + superscript->getSizeX();
	sizeY = sizeUp + sizeDown;
}

SubSupGlyph::SubSupGlyph()
{
	type = SuperSub;
}

void SubSupGlyph::initializeSuperToSubSup(Glyph* argIn, GeneralGlyph* superIn,const char *input)
{
	argument = argIn;
	superscript = superIn;
	subscript = new GeneralGlyph;

	subscript->parseGlyphFromInput(input);

	type = SuperSub;
}

void SubSupGlyph::initializeSubToSubSup(Glyph* argIn, GeneralGlyph* subIn,const char *input)
{
	argument = argIn;
	subscript = subIn;
	superscript = new GeneralGlyph;

	superscript->parseGlyphFromInput(input);

	type = SuperSub;
}

void SubSupGlyph::calculateSize()
{
	argument->calculateSize();
	superscript->calculateSize();
	subscript->calculateSize();

	sizeUp = argument->getSizeY() + superscript->getSizeY();
	sizeDown = subscript->getSizeY();

	sizeX = MAX(superscript->getSizeX(),subscript->getSizeX());
	sizeX += argument->getSizeX();
	sizeY = sizeUp + sizeDown;
}

PiGlyph::PiGlyph()
{
	type = Pi;
};

void PiGlyph::calculateSize()
{
	sizeUp = 1;
	sizeDown = 0;
	sizeX = 1;
	sizeY = 1;
}