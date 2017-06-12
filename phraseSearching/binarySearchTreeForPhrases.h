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

#ifndef BST_FOR_PHRASES_H
#define BST_FOR_PHRASES_H

#include <cstddef>	//NULL
#include "phrase.h"

namespace fmi
{
namespace sdp
{
	//represents the type, returned by function findString(const char*)
	enum SearchResult
	{
		PHRASE_NOT_FOUND,

		//the string matches the first words of a phrase in the dictionary 
		FIRST_WORDS_OF_PHRASE,

		PHRASE_FOUND
	};



	//The left successor is less than or equal to the root
	//The root is less than the right successor!!!
	//
	//Because of the specifics concerning the data type "Phrase"
	//and the particular problem that we have to solve
	//we won't make the binary search tree a template class
	class BinarySearchTreeForPhrases
	{
	public:
		BinarySearchTreeForPhrases();
		BinarySearchTreeForPhrases(const BinarySearchTreeForPhrases& other);
		BinarySearchTreeForPhrases& operator=(const BinarySearchTreeForPhrases& other);
		~BinarySearchTreeForPhrases();

		//adds a new phrase to the tree of phrases
		void insert(const Phrase& newPhrase);
		
		//Searches for the string str in the tree.
		//If found, thecoefficient of the phrase is
		//assigned to coefficientBuffer.
		SearchResult findString(const char* str, double &coefficientBuffer) const;

		//prints out the elements of the tree in preorder
		void printTree() const;

	private:
		struct Node
		{
			Node(const Phrase& phrase, Node *left = NULL, Node *right = NULL)
				: phrase(phrase)
				, left(left)
				, right(right)
			{}

			Phrase phrase;
			Node *left;
			Node *right;
		};

	private:
		Node* root;
	
	private:
		//copies the content of the tree with root pointed by sourceRoot
		//to the tree with root pointed by destinationRoot
		static void copy(Node *&destinantionRoot, const Node *sourceRoot);

		//clears the dinamic memory pointed by root
		static void clear(Node* root);

		//adds a new phrase to the tree with root pointed by "root"
		static void insert(Node *&root, const Phrase& newPhrase);

		//searches for the string str in the tree with root pointed by "root"
		static SearchResult search(const Node* root, const char *str
			, const int lengthOfStr, double &coefficientBuffer);

		//prints out the elements of the tree with root pointed by "root"
		//in preorder
		static void preorderPrint(const Node* root);
	};

}	//sdp
}	//fmi

#endif // BST_FOR_PHRASES_H

