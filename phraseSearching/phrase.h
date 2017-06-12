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

#ifndef PHRASE_H
#define PHRASE_H

#include<iostream>	//ostream

namespace fmi
{
namespace sdp
{

	/*
	This data type represents a word or phrase in a dictionary file.
	Each phrase has a coefficient which shows how important
	it is in a particular asspect.
	*/
	class Phrase
	{
	public:
		Phrase();
		Phrase(const Phrase& other);
		Phrase& operator=(const Phrase& other);
		~Phrase();

	public:
		double coefficient;
		char *notation;
	
	private:
		//clears the dinamically allocated memory
		//pointed to by *notation
		void clear();

		void copy(const Phrase& other);
	};

	//compares two phrases alphabetically by their notations
	bool operator<(const Phrase& first, const Phrase& second);

	std::ostream& operator<<(std::ostream& out, const Phrase& phrase);

	void swapPhrases(Phrase &first, Phrase& second);

}	//sdp
}	//fmi

#endif //PHRASE_H