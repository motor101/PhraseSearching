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
#include <iostream>	//ostream
#include <cstddef>	//NULL
#include "phrase.h"

namespace fmi
{
namespace sdp
{

Phrase::Phrase() 
		:notation(NULL)
{}

Phrase::Phrase(const Phrase& other)
{
	copy(other);
}

Phrase& Phrase::operator=(const Phrase& other)
{
	if (this != &other)
	{
		clear();
		copy(other);
	}
	return *this;
}

Phrase::~Phrase()
{
	clear();
}

//clears the dinamically allocated memory
//pointed to by *notation
void Phrase::clear()
{
	delete[] notation;
	notation = NULL;
}

void Phrase::copy(const Phrase& other)
{
	if (other.notation == NULL)
	{
		notation = NULL;
	}
	else
	{
		int size = strlen(other.notation);
		
		notation = new char[size + 1];
		
		strcpy(notation, other.notation);
	}

	coefficient = other.coefficient;
}

//compares two phrases alphabetically by their notations
bool operator<(const Phrase& first, const Phrase& second)
{
	return strcmp(first.notation, second.notation) < 0;
}

std::ostream& operator<<(std::ostream& out, const Phrase& phrase)
{
	out << phrase.notation << ' '
		<< phrase.coefficient;

	return out;
}

void swapPhrases(Phrase &first, Phrase& second)
{
	if (&first != &second)
	{
		double tempCoefficient = first.coefficient;
		first.coefficient = second.coefficient;
		second.coefficient = tempCoefficient;

		char* tempNotation = first.notation;
		first.notation = second.notation;
		second.notation = tempNotation;
	}
}

}//sdp
}//fmi