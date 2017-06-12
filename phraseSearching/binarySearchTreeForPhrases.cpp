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
#include <new>	//std::bad_alloc
#include<cstring>
#include "binarySearchTreeForPhrases.h"


namespace fmi
{
namespace sdp
{

	BinarySearchTreeForPhrases::BinarySearchTreeForPhrases()
	{
		root = NULL;
	}

	BinarySearchTreeForPhrases
	::BinarySearchTreeForPhrases(const BinarySearchTreeForPhrases& other)
	{
		root = NULL;
		copy(root, other.root);
	}

	BinarySearchTreeForPhrases& BinarySearchTreeForPhrases
	::operator=(const BinarySearchTreeForPhrases& other)
	{
		if (this != &other)
		{
			Node* newRoot = NULL;
			
			try
			{
				copy(newRoot, other.root);
			}
			catch (std::bad_alloc& except)
			{
				clear(newRoot);
				throw;
			}

			clear(root);
			root = newRoot;
		}
		return *this;
	}

	BinarySearchTreeForPhrases::~BinarySearchTreeForPhrases()
	{
		clear(root);
	}

	//adds a new phrase to the tree of phrases
	void BinarySearchTreeForPhrases::insert(const Phrase& newPhrase)
	{
		insert(root, newPhrase);
	}

	//Searches for the string str in the tree.
	//If found, thecoefficient of the phrase is
	//assigned to coefficientBuffer.
	SearchResult BinarySearchTreeForPhrases::findString(const char* str, double &coefficientBuffer) const
	{
		return search(root, str, strlen(str), coefficientBuffer);
	}

	//prints out the elements of the tree in preorder
	void BinarySearchTreeForPhrases::printTree() const
	{
		preorderPrint(root);
		std::cout << '\n';
	}

	//copies the content of the tree with root pointed by sourceRoot
	//to the tree with root pointed by destinationRoot
	void BinarySearchTreeForPhrases::copy(Node *&destinantionRoot, const Node *sourceRoot)
	{
		if (sourceRoot)
		{
			destinantionRoot = new Node(sourceRoot->phrase);
			copy(destinantionRoot->left, sourceRoot->left);
			copy(destinantionRoot->right, sourceRoot->right);
		}
		else
		{
			destinantionRoot = NULL;
		}
	}

	//clears the dinamic memory pointed by root
	void BinarySearchTreeForPhrases::clear(Node* root)
	{
		if (root)
		{
			clear(root->left);
			clear(root->right);

			delete root;
		}
	}


	//adds a new phrase to the tree with root pointed by "root"
	void BinarySearchTreeForPhrases::insert(Node *&root, const Phrase& newPhrase)
	{
		if (root == NULL)
		{
			root = new Node(newPhrase);
		}
		else
		{
			//The left successor is less than or equal to the root
			//The root is less than the right successor!!!
			if (root->phrase < newPhrase)
				insert(root->right, newPhrase);
			else
				insert(root->left, newPhrase);
		}
	}

	//searches for the string str in the tree with root pointed by "root"
	SearchResult BinarySearchTreeForPhrases::search(const Node* root, const char *str
		, const int lengthOfStr, double &coefficientBuffer)
	{
		if (root == NULL)
		{
			return PHRASE_NOT_FOUND;
		}

		int comparisonResult = strncmp(root->phrase.notation, str, lengthOfStr);

		//The left successor is less than or equal to the root
		//The root is less than the right successor!!!
		if (comparisonResult < 0)
			return search(root->right, str, lengthOfStr, coefficientBuffer);

		if (comparisonResult > 0)
			return search(root->left, str, lengthOfStr, coefficientBuffer);

		//if the two strings match exactly
		if (root->phrase.notation[lengthOfStr] == '\0')
		{
			coefficientBuffer = root->phrase.coefficient;
			return PHRASE_FOUND;
		}

		//if str is the first words of the phrase
		//Example: str="hello" phrase="hello world"
		if (root->phrase.notation[lengthOfStr] == ' ')
			return FIRST_WORDS_OF_PHRASE;

		//str is the prefix of the phrase
		//Example: str="my friend" phrase="my friends"
		return search(root->left, str, lengthOfStr, coefficientBuffer);
	}

	//prints out the elements of the tree with root pointed by "root"
	//in preorder
	void BinarySearchTreeForPhrases::preorderPrint(const Node* root)
	{
		if (root)
		{
		std::cout << root->phrase<<'\n';
		preorderPrint(root->left);
		preorderPrint(root->right);
		}
	}

}	//sdp
}	//fmi