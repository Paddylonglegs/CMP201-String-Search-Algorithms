/*
Author: Patrick Collins
©️license MIT
https://github.com/Paddylonglegs/
*/

#include <cassert>
#include <iostream>
#include <list>
#include <string>
#include <chrono> //to time search algos
#include <fstream>
#include <functional> ///for std::hash
#include "utils.h"

using std::chrono::duration_cast;
using std::chrono::milliseconds; //measurement
using std::cout;
using std::endl;
using std::list;
using std::string;
using std::ofstream; //wrting timings to file
using std::hash;
using std::cin;

//Defining the type of clock
typedef std::chrono::steady_clock the_clock;

//Globals
int found = 0;
list<Position> matchesBM; //creating list for multi search, orders positions
list<Position> matchesRK; //creating list for multi search, orders positions
list<Position> BMtimes; //creating list for multi search, orders positions
list<Position> RKTimes; //creating list for multi search, orders positions
string currentTxt = "";

/** Return first position of pat in text, or -1 if not found. */
Position find_boyer_moore(const string& pat, const string& text) //Taken from the week 7 practical function "bm_multi"
{ 
	found = 0;
	Position pat_len = pat.size();
	Position text_len = text.size();

	for (Position i = 0; i < text_len - pat_len; ++i) 
	{
		//show_context(text, i);

		Position skip[256]; //array used as accessing elements is constant time
		for (int i = 0; i < 256; i++)
		{
			skip[i] = pat_len; //setting length of skip to pat lenth
		}

		for (int i = 0; i < pat_len; i++)
		{
			skip[int(pat[i])] = (pat_len - 1) - i;
		}

		Position s = skip[int(text[i + pat_len - 1])];
		if (s != 0)
		{
			i = i + s - 1; //skip forwards by pattern length
			continue; //going through program after check
		}

		Position j;
		for (j = 0; j < pat_len; j++)
		{
			if (text[i + j] != pat[j]) {
				break; // Doesn't match here.
			}
		}
		
		if (j == pat_len)
		{
			found++; //just to compare if algorithms are finding same amount of pat
			matchesBM.push_back(i); //Match, push onto list
		}
	}
	return -1; // Not found.
}

//Creating a new hash here, by taking current text and subtracting first character, and adding next character with next character of text file
int newHash(const string& pat, const string& text, Position pat_len, size_t txtHash, int i)
{
	
	string first = currentTxt.substr(1, pat_len); //taking first character away from pattern length
	
	i = i + pat_len;
	auto next = text[i]; //getting next letter in text
	currentTxt = first + next; //adding both to get new text

	txtHash = hash<string>{}(currentTxt); //hashing new current text

	i = i - pat_len;

	return i; //return text posision
	return txtHash; //return new hash
}


/** Return first position of pat in text, or -1 if not found. */
Position find_rabin_karp(const string& pat, const string& text) //My own attempt of implementing rabin karp algorithm, solely based on the explanation given in lecture slides and using std::hash
{
	Position pat_len = pat.size();
	size_t text_len = text.size();
	size_t patHash = hash<string>{}(pat); //hashing pattern using std::hash
	
	found = 0;
	//search through text looking for hash match
	for (Position i = 0; i < text_len - pat_len; ++i) 
	{
		//Where am I?
		//show_context(text, i);

		if (currentTxt.size() != pat_len) //current text is not set
		{
			Position r = i;
			for (Position t = 0; t < pat_len; t++)
			{
				auto temp = text[r];
				currentTxt = currentTxt + temp; //creating current text of pattern length
				r++;
			}
		}

		size_t txtHash = hash<string>{}(currentTxt); //hashing current text position of text

		if(patHash == txtHash) //Hashes are the same
		{

			if (currentTxt == pat) //double checking the word matches the pattern
			{
				//definite match

				found++; //just to compare if algorithms are finding same amount of pattern in text

				matchesRK.push_back(i); //Match, push onto list.

				newHash(pat, text, pat_len, txtHash, i); //create new hash

				continue; //going through program after change*/
			}
			else
			{
				newHash(pat, text, pat_len, txtHash, i); //create new hash
				continue; //going through program after change*/
			}
		}
		else if(patHash != txtHash) //hashes don't match, hash next text
		{

			newHash(pat, text, pat_len, txtHash, i); //create new hash

		
			continue; //going through program after change*/
		}

	}
	return -1; // Not found.*/
}

int main(int argc, char *argv[]) 
{
	//Text files to write timings too
	ofstream Boyer("BM_Timings.txt");
	ofstream Rabin("RK_Timings.txt");

	//What will the algorithms be searching?
	string text;
	text = ""; //text to look through
	load_jute_book(text); //reading jute_book as text

	cout << "What would you like to search?: "; //User can define word to search
	string input = "";
	cin >> input;
	string pat = input; //pattern to search

	//BOYER MOORE//
	for (int i = 0; i < 10000; i++) //10000 times to get good data
	{

		// Start timing
		the_clock::time_point start = the_clock::now(); ///////USING CHRONO TO MEASURE TIME////////

		//calling  boyer moore searching to time in ms

		//BOYER MOORE/
		Position pos = find_boyer_moore(pat, text); //scanning through jute_book//


		// Stop timing
		the_clock::time_point end = the_clock::now();

		//Printing out matches//
		/*for (int n : matchesBM)
		{
			cout << "Found '" << pat << "' at position " << n << ":\n";
			//show_context(text, n);
		}*/

		
		auto time_taken = duration_cast<milliseconds>(end - start).count(); // Compute the difference between the two times in milliseconds
		BMtimes.push_back(time_taken); //pushing time onto list
	}
	
	
	cout << "Found " << found << " matches for '" << pat << "' with boyer moore.\n";

	BMtimes.sort(); //sorting list
	for (int b : BMtimes)
	{
		Boyer << b << "\n"; //printing Boyer Moore results to text file, which will be used for scipy
	}


	//RABIN KARP//
	for (int i = 0; i < 10000; i++)  //10000 times to get good data
	{
		// Start timing
		the_clock::time_point start = the_clock::now(); ///////USING CHRONO TO MEASURE TIME///////

		//calling rabin karp searching to time in ms

		//RABIN KARP//
		Position pos = find_rabin_karp(pat, text);  //scanning through jute_book//
		

		// Stop timing
		the_clock::time_point end = the_clock::now();

		//Printing out matches//
		/*for (int n : matchesRK)
		{
			cout << "Found '" << pat << "' at position " << n << ":\n";
			show_context(text, n);
		}*/

		auto time_taken = duration_cast<milliseconds>(end - start).count(); // Compute the difference between the two times in milliseconds
		RKTimes.push_back(time_taken); //pushing time onto list 
	}

	cout << "Found " << found << " matches for '" << pat << "'  with rabin karp.\n";

	RKTimes.sort(); //sorting list
	for (int r : RKTimes)
	{
		Rabin << r << "\n"; //printing Rabin Karp results to text file, which will be used for scipy
	}

	return 0;
}