/**
*
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Kamen Ivanov
* @idnumber 61894
* @task 3
* @compiler VC
*
*/

#include"stdafx.h"
#include<iostream>
#include<fstream>
#include<cctype>	//isalpha, islower 
#include<exception>
#include "phrase.h"
#include "binarySearchTreeForPhrases.h"

using namespace std;
using namespace fmi::sdp;

//recognizes the phrases in the dictionary 
//and adds them to the list of phrases
static bool initializeListOfPhrases
(Phrase *&listOfPhrasesInDictionary
, int &numberOfPhrasesInDictionary
, const char* nameOfDictionaryFile
);

//sorts the phrases in the dictionary in acsending order
static void quickSortListOfPhrasesAlphabetically
(Phrase *listOfPhrasesInDictionary
, int beginPos
, int endPos
);

static bool canBeConsideredAsSpace(char symbol);

static void makeAllLettersLowerCaseAndMakeWhiteCharactersSpace(char *str);

//the words in str must be devided by exactly one space
static int getNumberOfWordsInPhrase(const char* str);

//adds coefficientOfCurrentPhrase to sumOfAllCoefficientsInFile
//and moves the reading position of the file one position after
//the last position of the first word of the phrase
static void addCoefficientOfPhraseToTotalSumAndGoToNextPhrase
(double &sumOfAllCoefficientsInFile
, double &coefficientOfCurrentPhrase
, ifstream& textFile
, int lastPosOfFirstWordInPhrase
, bool &insidePhrase
, bool &firstWordOfPhraseIsRead
, bool &whiteSymbolFound
, int &numberOfWordsInFile
, int &numberOfWordsInPhrase
);

static void readTheCurrentPhraseAndSearchForItInTheDictionary
(ifstream &textFile
, int firstPosOfPhrase
, int lastPosOfPhrase
, char* phraseBuffer
, SearchResult &searchResult
, const BinarySearchTreeForPhrases &dictionaryTree
, double &coefficientOfCurrentPhrase
);


//returns the coefficient of the file
//(i.e. the sum of all coefficients of the phrases
//in the file, which are also found in the dictionary,
//devided by the number of words in the file)
//A word is a sequence of letters!
static double scanTextFileAndGetCoefficient(const char* textFileName
	, const BinarySearchTreeForPhrases& dictionaryTree);

//prints out the name of every text file in argv and it's coefficient
static void phraseSearching(int argc, char *argv[], const BinarySearchTreeForPhrases& dictionaryTree);

//copies the phrases from the list to the tree.
//The list should be sorted alphabetically 
//in order for the tree to have minimum height
//(that way the search operation in the tree 
//will be as fast as possible)
static void copyPhrasesFromListToTree
(const Phrase *listOfPhrasesInDictionary
, int beginPos
, int endPos
, BinarySearchTreeForPhrases& dictionaryTree
);

bool textFileAnalizing(int argc, char *argv[]);



//recognizes the phrases in the dictionary 
//and adds them to the list of phrases
static bool initializeListOfPhrases
(Phrase *&listOfPhrasesInDictionary
, int &numberOfPhrasesInDictionary
, const char* nameOfDictionaryFile
)
{
	//in the particular task, that we have to solve
	//there is only one dictionary file,
	//so the list of phrases should be empty
	listOfPhrasesInDictionary = NULL;
	numberOfPhrasesInDictionary = 0;

	ifstream dictionaryFile(nameOfDictionaryFile, ios::in | ios::binary);
	if (!dictionaryFile)
	{
		cerr << "could not open " << nameOfDictionaryFile << " for reading\n";
		return false;
	}

	//tells if the current reading position of the file
	//is inside a number
	bool insideNumber = false;
	
	//the symbols of the file will be read one by one and
	//stored in this variable
	char symbol;

	//Each phrase in the file has a coefficient(a real number),
	//so the number of coefficients should be equal to the number
	//of phrases. We'll scan through the file to find the number 
	//coefficients.
	while (dictionaryFile.get(symbol))
	{
		
		if (isdigit(symbol))
		{
			if (insideNumber == false)
			{
				insideNumber = true;

				//each time we encounter a new number, we have
				//also read a new phrase
				++numberOfPhrasesInDictionary;
			}
		}
		else
		{
			if (symbol != '.')
			{
				if (insideNumber == true)
				{
					insideNumber = false;
				}
			}
			else //symbol is '.'
			{
				//if there are digits before '.'
				if (insideNumber == true)
					continue;
			}
		}
	}

	if (numberOfPhrasesInDictionary == 0)
	{
		cerr << "There are no phrases in the dictionary"
			" (each phrase must have a coefficient)\n";
		return false;
	}

	listOfPhrasesInDictionary = new Phrase[numberOfPhrasesInDictionary];
	int indexOfCurrentPhrase=0;
	
	//tells if the current reading position of the file
	//is inside a phrase
	bool insidePhrase = false;

	insideNumber = false;

	int firstPosOfPhrase=0;
	int lastPosOfPhrase=0;

	int currentPositionOfSymbol = 0;

	dictionaryFile.clear();
	dictionaryFile.seekg(0, ios::beg);

	//we'll find each phrase in the dictionary and
	//add it to the list of phrases
	while (dictionaryFile.get(symbol))
	{
		//we assign the position of the current symbol,
		//and not the position of the next symbol in file
		currentPositionOfSymbol = -1 + dictionaryFile.tellg();

		if (islower(symbol))
		{
			if (insideNumber == true)
			{
				insideNumber = false;
			}

			//if this is the first letter of the phrase
			if (insidePhrase == false)
			{
				firstPosOfPhrase = lastPosOfPhrase  = currentPositionOfSymbol;
				insidePhrase = true;
			}
			else
			{
				lastPosOfPhrase = currentPositionOfSymbol;
			}
		}
		else
		{
			if ( ! isspace(symbol) )
			{
				//if the symbol is invalid for the dictionary
				if ( ( ! isdigit(symbol) ) && (symbol != '+' ) && (symbol != '-') && (symbol != '.') )
				{
					cerr << "there is an invalid symbol on position " << currentPositionOfSymbol << '\n';
					
					delete[] listOfPhrasesInDictionary;
					listOfPhrasesInDictionary = NULL;
					dictionaryFile.close();

					return false;
				}

				else // symbol is a digit or a '-' or a '+' or a '.'
				{
					if (insidePhrase == false)
					{
						//if there are two numbers and no word between them
						if (insideNumber == false)
						{
							cerr << "there shouldn't be a number on position " 
								<< currentPositionOfSymbol
								<< " there should be a word\n";
						}
						else
						{
							cerr << "this message shouldn't appear"
								"if the logic of the program is correct\n";
						}

						delete[] listOfPhrasesInDictionary;
						listOfPhrasesInDictionary = NULL;
						dictionaryFile.close();

						return false;
					}
					else //this is the end of the phrase and the beginning of its coefficient
					{
						insidePhrase = false;
						insideNumber = true;

						//we need (lastPosOfPhrase + 1 - firstPosOfPhrase) symbols for the 
						//letters of the phrase and 1 symbol for the '\0'
						listOfPhrasesInDictionary[indexOfCurrentPhrase].notation = 
							new char[(lastPosOfPhrase + 1) - firstPosOfPhrase + 1]	{};

						dictionaryFile.seekg(firstPosOfPhrase);

						dictionaryFile.read
							(listOfPhrasesInDictionary[indexOfCurrentPhrase].notation
							, (lastPosOfPhrase + 1) - firstPosOfPhrase);

						dictionaryFile >> listOfPhrasesInDictionary[indexOfCurrentPhrase].coefficient;

						++indexOfCurrentPhrase;
					}
				}
			}
			else //symbol is a whitespace character
			{
				if (insideNumber)
					insideNumber = false;
			}
		}
	}

	dictionaryFile.close();

	return true;
}


//sorts the phrases in the dictionary in acsending order
static void quickSortListOfPhrasesAlphabetically
	(Phrase *listOfPhrasesInDictionary
	, int beginPos
	, int endPos
	)
{
	if (beginPos >= endPos)
		return;

	int lastPos = beginPos;
		
	for (int i = beginPos + 1; i <= endPos; ++i)
	if (listOfPhrasesInDictionary[i] < listOfPhrasesInDictionary[beginPos])
	{
		++lastPos;
		swapPhrases(listOfPhrasesInDictionary[i], listOfPhrasesInDictionary[lastPos]);
	}

	swapPhrases(listOfPhrasesInDictionary[beginPos], listOfPhrasesInDictionary[lastPos]);

	quickSortListOfPhrasesAlphabetically(listOfPhrasesInDictionary, beginPos, lastPos - 1);

	quickSortListOfPhrasesAlphabetically(listOfPhrasesInDictionary, lastPos + 1, endPos);
}

static bool canBeConsideredAsSpace(char symbol)
{
	switch (symbol)
	{
	case ' ' :
	case '\n':
	case '\t':
	case '\r':
		return true;

	default: return false;
	}
}

static void makeAllLettersLowerCaseAndMakeWhiteCharactersSpace(char *str)
{
	int pos=0;
	while (str[pos] != '\0')
	{
		if (isupper(str[pos]))
		{
			str[pos] = tolower(str[pos]);
		}
		else
		{
			//the new line in binary files can
			//appear as '\r''\n'.
			//That's why we must consider this two
			//symbols as one.
			if (str[pos] == '\r')
			{
				int i = pos;
				while (str[i] != '\0')
				{
					str[i] = str[i+1];
					++i;
				}
			}
			if (canBeConsideredAsSpace(str[pos]))
			{
				str[pos] = ' ';
			}
		}
		++pos;
	}
}

//the words in str must be devided by exactly one space
static int getNumberOfWordsInPhrase(const char* str)
{
	int pos = 0;
	int numberOfWordsInPhrase = 1;

	while (str[pos] != '\0')
	{
		if (str[pos] == ' ')
			++numberOfWordsInPhrase;

		++pos;
	}
	return numberOfWordsInPhrase;
}

//adds coefficientOfCurrentPhrase to sumOfAllCoefficientsInFile
//and moves the reading position of the file one position after
//the last position of the first word of the phrase
static void addCoefficientOfPhraseToTotalSumAndGoToNextPhrase
( double &sumOfAllCoefficientsInFile
, double &coefficientOfCurrentPhrase
, ifstream& textFile
, int lastPosOfFirstWordInPhrase
, bool &insidePhrase
, bool &firstWordOfPhraseIsRead
, bool &whiteSymbolFound
, int &numberOfWordsInFile
, int &numberOfWordsInPhrase
)
{

	sumOfAllCoefficientsInFile += coefficientOfCurrentPhrase;
	coefficientOfCurrentPhrase = 0.0;

	if (numberOfWordsInPhrase != 0)
		numberOfWordsInFile += numberOfWordsInPhrase;
	else
		++numberOfWordsInFile;

	numberOfWordsInPhrase = 0;

	textFile.seekg(lastPosOfFirstWordInPhrase + 1, ios::beg);
	insidePhrase = false;
	firstWordOfPhraseIsRead = false;
	whiteSymbolFound = false;
}

static void readTheCurrentPhraseAndSearchForItInTheDictionary
( ifstream &textFile
, int firstPosOfPhrase
, int lastPosOfPhrase
, char* phraseBuffer
, SearchResult &searchResult
, const BinarySearchTreeForPhrases &dictionaryTree
, double &coefficientOfCurrentPhrase
)
{
	int currentValueOfTellg = textFile.tellg();

	textFile.seekg(firstPosOfPhrase, ios::beg);
	textFile.read(phraseBuffer, lastPosOfPhrase + 1 - firstPosOfPhrase);
	textFile.seekg(currentValueOfTellg, ios::beg);

	phraseBuffer[lastPosOfPhrase + 1 - firstPosOfPhrase] = '\0';

	//change the symbols of phraseBuffer to fit the requierments
	//for the words in the dictionaryTree
	makeAllLettersLowerCaseAndMakeWhiteCharactersSpace(phraseBuffer);

	searchResult = dictionaryTree.findString(phraseBuffer, coefficientOfCurrentPhrase);
}

//returns the coefficient of the file
//(i.e. the sum of all coefficients of the phrases
//in the file, which are also found in the dictionary,
//devided by the number of words in the file)
//A word is a sequence of letters!
static double scanTextFileAndGetCoefficient(const char* textFileName
	, const BinarySearchTreeForPhrases& dictionaryTree)
{
	ifstream textFile(textFileName, ios::in | ios::binary);
	if (!textFile)
	{
		cerr << "could not open " << textFileName << " for reading\n";
		throw(std::exception("file does not exist"));
	}

	double sumOfAllCoefficientsInFile = 0.0;

	int numberOfWordsInFile = 0;

	int numberOfWordsInPhrase=0;

	double coefficientOfCurrentPhrase = 0.0;

	int firstPosOfPhrase;

	int lastPosOfPhrase;

	bool firstWordOfPhraseIsRead = false;

	//this variable stores the bigger value:
	//1) the index of the last letter
	//of the first word of the phrase
	//AND
	//2) the index of the last letter of the first 
	//successfully found subphrase of the current phrase
	int lastPosOfFirstWordInPhrase;

	char symbol;

	int positionOfCurrentSymbolInFile;

	//shows if the current reading position of the file is inside a phrase
	bool insidePhrase = false;

	//shows if the first symbol after the current word is ' ', '\n' or '\t'
	bool whiteSymbolFound = false;

	const int maxLengthOfPhrase = 100;

	char phraseBuffer[maxLengthOfPhrase + 1]  {};

	SearchResult searchResult;

	while (true)
	{
		while (textFile.get(symbol))
		{
			positionOfCurrentSymbolInFile = -1 + textFile.tellg();

			if (isalpha(symbol))
			{
				whiteSymbolFound = false;

				if (firstWordOfPhraseIsRead == false)
					lastPosOfFirstWordInPhrase = positionOfCurrentSymbolInFile;

				if (insidePhrase)
				{
					lastPosOfPhrase = positionOfCurrentSymbolInFile;
				}
				else
				{
					insidePhrase = true;

					  firstPosOfPhrase 
					= lastPosOfPhrase 
					= positionOfCurrentSymbolInFile;
				}
			}
			else
			{
				if (insidePhrase == true)
				{
					if ((lastPosOfPhrase + 1 - firstPosOfPhrase) <= maxLengthOfPhrase)
					{
						readTheCurrentPhraseAndSearchForItInTheDictionary
							(textFile
							, firstPosOfPhrase
							, lastPosOfPhrase
							, phraseBuffer
							, searchResult
							, dictionaryTree
							, coefficientOfCurrentPhrase
							);

						if (searchResult == PHRASE_FOUND)
						{
							lastPosOfFirstWordInPhrase = lastPosOfPhrase;
							numberOfWordsInPhrase = getNumberOfWordsInPhrase(phraseBuffer);
						}

						//if this is the end of the phrase
						if (searchResult == PHRASE_NOT_FOUND
							||
							(whiteSymbolFound == true
							||
							whiteSymbolFound == false && canBeConsideredAsSpace(symbol) == false
							)
							)
						{
							addCoefficientOfPhraseToTotalSumAndGoToNextPhrase
								(sumOfAllCoefficientsInFile
								, coefficientOfCurrentPhrase
								, textFile
								, lastPosOfFirstWordInPhrase
								, insidePhrase
								, firstWordOfPhraseIsRead
								, whiteSymbolFound
								, numberOfWordsInFile
								, numberOfWordsInPhrase
								);
							continue;
						}

						//this is the end of the current word in the phrase
						//but still not the end of the phrase
						else
						{
							firstWordOfPhraseIsRead = true;

							//the new line in binary mode can appear as '\r''\n'
							if (symbol != '\r')
								whiteSymbolFound = true;
						}

					}
					else //the size of the phrase is bigger than the size of the buffer 
					{
						addCoefficientOfPhraseToTotalSumAndGoToNextPhrase
							(sumOfAllCoefficientsInFile
							, coefficientOfCurrentPhrase
							, textFile
							, lastPosOfFirstWordInPhrase
							, insidePhrase
							, firstWordOfPhraseIsRead
							, whiteSymbolFound
							, numberOfWordsInFile
							, numberOfWordsInPhrase
							);
						continue;
					}
				}
			}
		}

		textFile.clear();

		if (insidePhrase == true)
		{
			if ((lastPosOfPhrase + 1 - firstPosOfPhrase) <= maxLengthOfPhrase)
			{
			
				readTheCurrentPhraseAndSearchForItInTheDictionary
					(textFile
					, firstPosOfPhrase
					, lastPosOfPhrase
					, phraseBuffer
					, searchResult
					, dictionaryTree
					, coefficientOfCurrentPhrase
					);
			}

			addCoefficientOfPhraseToTotalSumAndGoToNextPhrase
				(sumOfAllCoefficientsInFile
				, coefficientOfCurrentPhrase
				, textFile
				, lastPosOfFirstWordInPhrase
				, insidePhrase
				, firstWordOfPhraseIsRead
				, whiteSymbolFound
				, numberOfWordsInFile
				, numberOfWordsInPhrase
				);
		}
		else
		{
			break;
		}
	}


	textFile.close();

	if (numberOfWordsInFile == 0)
		return 0.0;
	else
		return sumOfAllCoefficientsInFile / numberOfWordsInFile;
}

//prints out the name of every text file in argv and it's coefficient
static void phraseSearching(int argc, char *argv[], const BinarySearchTreeForPhrases& dictionaryTree)
{
	const int indexOfFirstTextFile = 2;

	double textFileCoefficient;

	for (int i = indexOfFirstTextFile; i < argc; ++i)
	{
		textFileCoefficient = scanTextFileAndGetCoefficient(argv[i], dictionaryTree);

			cout << argv[i] << ' ' << textFileCoefficient << '\n';
	}
}

//copies the phrases from the list to the tree.
//The list should be sorted alphabetically 
//in order for the tree to have minimum height
//(that way the search operation in the tree 
//will be as fast as possible)
static void copyPhrasesFromListToTree
	(const Phrase *listOfPhrasesInDictionary
	, int beginPos
	, int endPos
	, BinarySearchTreeForPhrases& dictionaryTree
	)
{
	if (beginPos > endPos)
		return;

	int middlePos = (beginPos + endPos) / 2;

	dictionaryTree.insert(listOfPhrasesInDictionary[middlePos]);

	copyPhrasesFromListToTree(listOfPhrasesInDictionary, beginPos, middlePos - 1, dictionaryTree);
	copyPhrasesFromListToTree(listOfPhrasesInDictionary, middlePos + 1, endPos, dictionaryTree);
}

bool textFileAnalizing(int argc, char *argv[])
{
	//this is the list of all the phrases which
	//will be found in the dictionary file
	Phrase *listOfPhrasesInDictionary = NULL;

	int numberOfPhrasesInDictionary=0;

	//the first command line argument is the name of the dictionary file
	bool result = initializeListOfPhrases(listOfPhrasesInDictionary
		, numberOfPhrasesInDictionary
		, argv[1]);

	if (!result)
		return false;

	BinarySearchTreeForPhrases dictionaryTree;
	
	copyPhrasesFromListToTree(listOfPhrasesInDictionary, 0
		, numberOfPhrasesInDictionary - 1, dictionaryTree);

	delete[] listOfPhrasesInDictionary;

	phraseSearching(argc, argv, dictionaryTree);

	return true;
}