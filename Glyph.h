#include <cstdio>
#include <cstdlib>
#include "Parser.h"

#define MAX_CHILD_GLYPHS 256
#define MAX(A,B) (A >= B)?A:B

////////////////// Glyph Base Class ////////////////

class Glyph
{
protected:
	int sizeX,sizeY;
	int sizeUp,sizeDown;
	ParserGlyphType type;
public:
	virtual void calculateSize() = 0;
	int getSizeX(){return sizeX;}
	int getSizeY(){return sizeY;}
	int getSizeUp(){return sizeUp;}
	int getSizeDown(){return sizeDown;}
	ParserGlyphType getGlyphType(){return type;}
};

////////////////// General Glyph ////////////////

class GeneralGlyph : public Glyph
{
private:
	int numberOfChildren;
	Glyph* children[MAX_CHILD_GLYPHS];

	void buildText(const char *input);
	void buildFrac(const char *inputNum, const char* inputDenom);
	void buildSqrt(const char *inputArg);
	void buildRoot(const char *inputArg, const char* inputRad);
	void buildSubscript(const char *input);
	void buildSuperscript(const char *input);
	void buildPi();
public:
	GeneralGlyph();
	void calculateSize();
	void parseGlyphFromInput(const char* input);
	int getNumberOfChildren(){return numberOfChildren;}
	Glyph* getChildGlyph(int c){return children[c%numberOfChildren];}
};

////////////////// Text Glyph ////////////////

class TextGlyph : public Glyph
{
private:
	char text;
public:
	TextGlyph(char t);
	char getText(){return text;}
	void calculateSize();
};

////////////////// Frac Glyph ////////////////

class FracGlyph : public Glyph
{
private:
	GeneralGlyph *numerator;
	GeneralGlyph *denominator;
public:
	FracGlyph();
	FracGlyph(const char *num, const char *denom);
	GeneralGlyph* getNumerator(){return numerator;}
	GeneralGlyph* getDenominator(){return denominator;}
	void calculateSize();
};

////////////////// Sqrt Glyph ////////////////

class SqrtGlyph : public Glyph
{
private:
	GeneralGlyph *argument;
public:
	SqrtGlyph();
	SqrtGlyph(const char *arg);
	GeneralGlyph* getArg(){return argument;}
	void calculateSize();
};

////////////////// Root Glyph ////////////////

class RootGlyph : public Glyph
{
private:
	GeneralGlyph *radical;
	GeneralGlyph *argument;
public:
	RootGlyph();
	RootGlyph(const char * arg, const char *rad);
	GeneralGlyph* getRad(){return radical;}
	GeneralGlyph* getArg(){return argument;}
	void calculateSize();
};

////////////////// Subscript Glyph ////////////////

class SubscriptGlyph : public Glyph
{
private:
	Glyph *argument;
	GeneralGlyph *subscript;
public:
	SubscriptGlyph(Glyph *argIn,const char *input);
	Glyph* getArgument(){return argument;}
	GeneralGlyph* getSubscript(){return subscript;}
	void calculateSize();
};

////////////////// Superscript Glyph ////////////////

class SuperscriptGlyph : public Glyph
{
private:
	Glyph *argument;
	GeneralGlyph *superscript;
public:
	SuperscriptGlyph(Glyph *argIn,const char *input);
	Glyph* getArgument(){return argument;}
	GeneralGlyph* getSuperscript(){return superscript;}
	void calculateSize();
};

////////////////// Sub/Superscript Glyph ////////////////

class SubSupGlyph : public Glyph
{
private:
	Glyph *argument;
	GeneralGlyph *superscript;
	GeneralGlyph *subscript;
public:
	SubSupGlyph();
	void initializeSuperToSubSup(Glyph* argIn, GeneralGlyph* superIn,const char *input);
	void initializeSubToSubSup(Glyph* argIn, GeneralGlyph* subIn,const char *input);
	Glyph* getArgument(){return argument;}
	GeneralGlyph* getSuperscript(){return superscript;}
	GeneralGlyph* getSubscript(){return subscript;}
	void calculateSize();
};

////////////////// Sub/Superscript Glyph ////////////////

class PiGlyph : public Glyph
{
private:
public:
	PiGlyph();
	void calculateSize();
};