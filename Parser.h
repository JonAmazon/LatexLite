#include <cstdlib>
#include <cstdio>

#define PARSER_INPUT_BUFFER_LEN 1024
#define PARSER_WORK_BBUFFER_LEN 1024

enum ParserGlyphType
{
	General,
	Frac,
	Sqrt,
	Root,
	Superscript,
	Subscript,
	SuperSub,
	Pi,
	Text
};

enum ParserState
{
	Error,
	Start,
	CBCommand,
	CBFrac,
	CBSqrt,
	CBRoot,
	CBPi,
	IsFrac,
	IsSqrt,
	IsRoot,
	IsPi,
	IsText,
	CBSuperscript,
	CBSubscript,
	IsSuperscript,
	IsSubscript
};

class Parser
{

private:
	int glyphFound,inputEnd;
	int inputIdx;
	char inputBuffer[PARSER_INPUT_BUFFER_LEN];
	char buffer1[PARSER_WORK_BBUFFER_LEN];
	char buffer2[PARSER_WORK_BBUFFER_LEN];
	ParserGlyphType currentGlyphType;
	ParserState currentState;

	void updateState();

	void updateStart();
	void updateCBCommand();
	void updateCBFrac();
	void updateCBSqrt();
	void updateCBRoot();
	void updateCBSuperscript();
	void updateCBSubscript();

	void updateIsFrac();
	void updateIsSqrt();
	void updateIsRoot();
	void updateIsPi();
	void updateIsSuperscript();
	void updateIsSubscript();
	void updateText();

public:
	Parser(const char* input);
	void findGlyph();

	int foundValidGlyph(){return glyphFound;}
	ParserGlyphType getGlyphType(){return currentGlyphType;}
	char* getBuffer1(){return buffer1;}
	char* getBuffer2(){return buffer2;}
};