#include "Parser.h"

int compareString(const char *A, const char *B, int len)
{
	int same = 1;
	for(int c = 0; c < len; ++c)
	{
		if(A[c] != B[c])
		{
			same = 0;
		}
	}
	return same;
}

int copyString(char *Src, char *Dst, int len)
{
	for(int c = 0; c < len; ++c)
	{
		Dst[c] = Src[c];
	}
	Dst[len] = '\0';

	return 0;
}

int findMatchingBraces(const char* A)
{
	int aIdx = 0,bracketLevel = 0,inputEnd = 0;

	do
	{
		if(A[aIdx] == '}')
		{
			bracketLevel--;
		}
		else if(A[aIdx] == '{')
		{
			bracketLevel++;
		}
		else if(A[aIdx] == '\0')
		{
			inputEnd = 1;
		}

		if(bracketLevel > 0)
		{
			aIdx++;
		}
	}while((bracketLevel > 0)&&(!inputEnd));
	aIdx++;

	if(inputEnd)
	{
		aIdx = 0;
	}
	
	return aIdx;
}

Parser::Parser(const char *input)
{
	inputIdx = 0;
	glyphFound = 0;
	inputEnd = 0;
	sprintf(inputBuffer,"%s",input);
	currentGlyphType = General;
	currentState = Start;
}

void Parser::findGlyph()
{
	glyphFound = 0;
	currentGlyphType = General;
	currentState = Start;

	while((!glyphFound)&&(!inputEnd)&&(currentState != Error))
	{
		updateState();
	}

	if(currentState == Error)
	{
		printf("Parser: FAILED to parse input stream.\n");
	}
}

void Parser::updateStart()
{
	switch(inputBuffer[inputIdx])
	{
	case '\\':
		{
			inputIdx++;
			currentState = CBCommand;
			break;
		}
	case '\0':
		{
			inputEnd = 1;
			break;
		}
	case '^':
		{
			inputIdx++;
			currentState = CBSuperscript;
			break;
		}
	case '_':
		{
			inputIdx++;
			currentState = CBSubscript;
			break;
		}
	case '\n':
		{
			inputIdx++;
			break;
		}
	default:
		{
			copyString(&inputBuffer[inputIdx],buffer1,1);
			inputIdx += 1;
			currentState = IsText;
			break;
		}
	}
}

void Parser::updateCBCommand()
{
	if(compareString(&inputBuffer[inputIdx],"frac",4))
	{
		inputIdx += 4;
		currentState = CBFrac;
	}
	else if(compareString(&inputBuffer[inputIdx],"sqrt",4))
	{
		inputIdx += 4;
		currentState = CBSqrt;
	}
	else if(compareString(&inputBuffer[inputIdx],"root",4))
	{
		inputIdx += 4;
		currentState = CBRoot;
	}
	else if(compareString(&inputBuffer[inputIdx],"pi",2))
	{
		inputIdx += 2;
		currentState = IsPi;
	}
	else
	{
		printf("Parser-Command: Invalid command.\n");
		currentState = Error;
	}
}

void Parser::updateCBFrac()
{
	int lengthOfMatchingBraces;

	lengthOfMatchingBraces = findMatchingBraces(&inputBuffer[inputIdx]);
	if(lengthOfMatchingBraces)
	{
		copyString(&inputBuffer[inputIdx+1],buffer1,lengthOfMatchingBraces-2);
		inputIdx += lengthOfMatchingBraces;
	}
	else
	{
		printf("Parser-CBFrac: Invalid first argument.\n");
		currentState = Error;
	}

	if(currentState != Error)
	{
		lengthOfMatchingBraces = findMatchingBraces(&inputBuffer[inputIdx]);
		if(lengthOfMatchingBraces)
		{
			copyString(&inputBuffer[inputIdx+1],buffer2,lengthOfMatchingBraces-2);
			inputIdx += lengthOfMatchingBraces;
		}
		else
		{
			printf("Parser-CBFrac: Invalid second argument.\n");
			currentState = Error;
		}
	}

	if(currentState != Error)
	{
		currentState = IsFrac;
	}
}

void Parser::updateCBSqrt()
{
	int lengthOfMatchingBraces;

	lengthOfMatchingBraces = findMatchingBraces(&inputBuffer[inputIdx]);
	if(lengthOfMatchingBraces)
	{
		copyString(&inputBuffer[inputIdx+1],buffer1,lengthOfMatchingBraces-2);
		inputIdx += lengthOfMatchingBraces;
	}
	else
	{
		printf("Parser-CBSqrt: Invalid argument.\n");
		currentState = Error;
	}

	if(currentState != Error)
	{
		currentState = IsSqrt;
	}
}

void Parser::updateCBRoot()
{
	int lengthOfMatchingBraces;

	lengthOfMatchingBraces = findMatchingBraces(&inputBuffer[inputIdx]);
	if(lengthOfMatchingBraces)
	{
		copyString(&inputBuffer[inputIdx+1],buffer1,lengthOfMatchingBraces-2);
		inputIdx += lengthOfMatchingBraces;
	}
	else
	{
		printf("Parser-CBRoot: Invalid first argument.\n");
		currentState = Error;
	}

	if(currentState != Error)
	{
		lengthOfMatchingBraces = findMatchingBraces(&inputBuffer[inputIdx]);
		if(lengthOfMatchingBraces)
		{
			copyString(&inputBuffer[inputIdx+1],buffer2,lengthOfMatchingBraces-2);
			inputIdx += lengthOfMatchingBraces;
		}
		else
		{
			printf("Parser-CBRoot: Invalid second argument.\n");
			currentState = Error;
		}
	}

	if(currentState != Error)
	{
		currentState = IsRoot;
	}
}

void Parser::updateCBSuperscript()
{
	int lengthOfMatchingBraces;

	lengthOfMatchingBraces = findMatchingBraces(&inputBuffer[inputIdx]);
	if(lengthOfMatchingBraces)
	{
		copyString(&inputBuffer[inputIdx+1],buffer1,lengthOfMatchingBraces-2);
		inputIdx += lengthOfMatchingBraces;
	}
	else
	{
		printf("Parser-CBSuperscript: Invalid argument.\n");
		currentState = Error;
	}

	if(currentState != Error)
	{
		currentState = IsSuperscript;
	}
}

void Parser::updateCBSubscript()
{
	int lengthOfMatchingBraces;

	lengthOfMatchingBraces = findMatchingBraces(&inputBuffer[inputIdx]);
	if(lengthOfMatchingBraces)
	{
		copyString(&inputBuffer[inputIdx+1],buffer1,lengthOfMatchingBraces-2);
		inputIdx += lengthOfMatchingBraces;
	}
	else
	{
		printf("Parser-CBSubscript: Invalid argument.\n");
		currentState = Error;
	}

	if(currentState != Error)
	{
		currentState = IsSubscript;
	}
}

void Parser::updateIsFrac()
{
	printf("Frac: num = [%s] denom = [%s]\n",buffer1,buffer2);

	glyphFound = 1;
	currentGlyphType = Frac;
}

void Parser::updateIsSqrt()
{
	printf("Sqrt: arg = [%s]\n",buffer1);

	glyphFound = 1;
	currentGlyphType = Sqrt;
}

void Parser::updateIsRoot()
{
	printf("Root: radical = [%s] arg = [%s]\n",buffer1,buffer2);

	glyphFound = 1;
	currentGlyphType = Root;
}

void Parser::updateIsPi()
{
	printf("Pi: %c\n",227);

	glyphFound = 1;
	currentGlyphType = Pi;
}

void Parser::updateIsSuperscript()
{
	printf("Superscript: arg = [%s]\n",buffer1);

	glyphFound = 1;
	currentGlyphType = Superscript;
}

void Parser::updateIsSubscript()
{
	printf("Subscript: arg = [%s]\n",buffer1);

	glyphFound = 1;
	currentGlyphType = Subscript;
}

void Parser::updateText()
{
	printf("Text: %s\n",buffer1);

	glyphFound = 1;
	currentGlyphType = Text;
}

void Parser::updateState()
{
	switch(currentState)
	{
	case Start:
		{
			updateStart();
			break;
		}
	case CBCommand:
		{
			updateCBCommand();
			break;
		}
	case CBFrac:
		{
			updateCBFrac();
			break;
		}
	case CBSqrt:
		{
			updateCBSqrt();
			break;
		}
	case CBRoot:
		{
			updateCBRoot();
			break;
		}
	case CBSuperscript:
		{
			updateCBSuperscript();
			break;
		}
	case CBSubscript:
		{
			updateCBSubscript();
			break;
		}
	case IsFrac:
		{
			updateIsFrac();
			break;
		}
	case IsSqrt:
		{
			updateIsSqrt();
			break;
		}
	case IsRoot:
		{
			updateIsRoot();
			break;
		}
	case IsPi:
		{
			updateIsPi();
			break;
		}
	case IsSuperscript:
		{
			updateIsSuperscript();
			break;
		}
	case IsSubscript:
		{
			updateIsSubscript();
			break;
		}
	case IsText:
		{
			updateText();
			break;
		}
	}
}