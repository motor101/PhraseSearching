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

#include "stdafx.h"
#include<iostream>
#include<cstring>
#include<cstdlib>	//for system("cls")
#include "phrase.h"
#include "binarySearchTreeForPhrases.h"


using namespace std;
using namespace fmi::sdp;

bool textFileAnalizing(int argc, char *argv[]);


void printSearchResult (enum SearchResult searchResult)
{
	switch (searchResult)
	{
	case PHRASE_NOT_FOUND: cout << "PHRASE_NOT_FOUND\n"; break;

	case PHRASE_FOUND: cout << "PHRASE_FOUND\n"; break;

	case FIRST_WORDS_OF_PHRASE: cout << "FIRST_WORDS_OF_PHRASE\n"; break;

	default: cerr << "invalid search result\n";
	}
}

void phraseTest()
{
	Phrase phrase1;
	phrase1.coefficient = 3.3;
	int size = 10;
	phrase1.notation = new char[size];
	strcpy(phrase1.notation, "hello");

	cout << phrase1<<endl;

	Phrase phrase2(phrase1);

	cout << phrase2 << endl;
	
	Phrase phrase3;
	phrase3.coefficient = 321.2;
	phrase3.notation = new char[size];
	strcpy(phrase3.notation, "ribata");

	cout << phrase3 << endl;

	phrase2 = phrase3;

	cout << phrase2 << endl;
}

void binarySearchTreeTest()
{
	BinarySearchTreeForPhrases tree1;
	cout << "tree1: ";
	tree1.printTree();

	Phrase phrase;
	phrase.coefficient = 666;

	const int size = 30;
	phrase.notation = new char[size]{};

	phrase.notation[0]='5';
	tree1.insert(phrase);

	cout << "tree1: ";
	tree1.printTree();

	phrase.notation[0] = '2';
	tree1.insert(phrase);

	phrase.notation[0] = '8';
	tree1.insert(phrase);

	cout << "tree1: ";
	tree1.printTree();

	phrase.notation[0] = '3';
	tree1.insert(phrase);

	phrase.notation[0] = '4';
	tree1.insert(phrase);

	phrase.notation[0] = '1';
	tree1.insert(phrase);

	phrase.notation[0] = '6';
	tree1.insert(phrase);

	phrase.notation[0] = '9';
	tree1.insert(phrase);

	phrase.notation[0] = '8';
	tree1.insert(phrase);

	cout << "tree1: ";
	tree1.printTree();

	BinarySearchTreeForPhrases tree2(tree1);

	cout << "tree2: ";
	tree2.printTree();

	BinarySearchTreeForPhrases tree3;

	strcpy(phrase.notation, "hello");
	tree3.insert(phrase);
	
	strcpy(phrase.notation, "mate");
	tree3.insert(phrase);
	
	strcpy(phrase.notation, "how");
	tree3.insert(phrase);
	
	strcpy(phrase.notation, "are");
	tree3.insert(phrase);
	
	strcpy(phrase.notation, "you");
	tree3.insert(phrase);

	strcpy(phrase.notation, "today");
	tree3.insert(phrase);

	cout << "tree3: ";
	tree3.printTree();

	tree2 = tree3;

	cout << "tree2: ";
	tree2.printTree();

	tree2 = tree1;

	cout << "tree2: ";
	tree2.printTree();

	strcpy(phrase.notation, "");
	tree3.insert(phrase);

	strcpy(phrase.notation, "how are you today");
	tree3.insert(phrase);

	strcpy(phrase.notation, "mate how are you today");
	tree3.insert(phrase);

	strcpy(phrase.notation, "are you today");
	tree3.insert(phrase);

	strcpy(phrase.notation, "hello mate how are you today");
	tree3.insert(phrase);


	system("cls");

	cout << "tree3: ";
	tree3.printTree();

	char stringForSearching[size];
	
	double coefficientBuffer;

	strcpy(stringForSearching, "");
	cout << stringForSearching << ": ";
	printSearchResult(tree3.findString(stringForSearching, coefficientBuffer));

	strcpy(stringForSearching, "hello");
	cout << stringForSearching << ": ";
	printSearchResult(tree3.findString(stringForSearching, coefficientBuffer));

	strcpy(stringForSearching, "mate");
	cout << stringForSearching << ": ";
	printSearchResult(tree3.findString(stringForSearching, coefficientBuffer));

	strcpy(stringForSearching, "hello mate");
	cout << stringForSearching << ": ";
	printSearchResult(tree3.findString(stringForSearching, coefficientBuffer));

	strcpy(stringForSearching, "hello ma");
	cout << stringForSearching << ": ";
	printSearchResult(tree3.findString(stringForSearching, coefficientBuffer));

	strcpy(stringForSearching, "are you");
	cout << stringForSearching << ": ";
	printSearchResult(tree3.findString(stringForSearching, coefficientBuffer));

	strcpy(stringForSearching, "hell");
	cout << stringForSearching << ": ";
	printSearchResult(tree3.findString(stringForSearching, coefficientBuffer));

	strcpy(stringForSearching, "hello mate how are you tod");
	cout << stringForSearching << ": ";
	printSearchResult(tree3.findString(stringForSearching, coefficientBuffer));

	strcpy(stringForSearching, "hello mate how are you today");
	cout << stringForSearching << ": ";
	printSearchResult(tree3.findString(stringForSearching, coefficientBuffer));
}



int main(int argc, char* argv[])
{
	//the command line must call the program with at least
	//two arguments: the name of the dictionary and at least
	//one name of a text file, which is to be scanned
	if (argc < 3)
	{
		cerr << "there aren't enough command line arguments\n";
		return 1;
	}
	else
		textFileAnalizing(argc, argv);

	return 0;
}

