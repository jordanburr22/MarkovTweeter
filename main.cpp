// Jordan Burroughs Spring 2020, CPEG476
// Markov Chain Tweet Generator

#define TWEETSIZE 250

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <string>
#include <set>
#include <map>
#include <iterator>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;

template<typename S>
auto select_random(const S &s, size_t n) {
  auto it = std::begin(s);
  // 'advance' the iterator n times
  std::advance(it,n);
  return it;
}

class Word {
	private:
		string word; 
		map<Word*, int> pairs;

		auto random_0_to_n() {
			auto r = rand() % pairs.size();
  			auto n = *select_random(pairs, r);
			return n;
		};
	public:
		// constructor
		Word(string newWord) {
			word = newWord;
		}
		
		void addWord(Word* word) {
			pairs[word] ++;
		};
		string getWord() {
			return word;
		};
		void printMap() {
			for (auto itr = pairs.begin(); itr != pairs.end(); ++itr) { 
        		cout << itr->first->getWord() << '\t' << itr->first << '\t' << itr->second << '\n'; 
    		} 
		};
		Word* getRandNext() {
			if( strcmp(word.c_str(), ".") == 0 ) { 
				return NULL;
			}
			auto r = rand() % pairs.size();
  			auto n = *select_random(pairs, r);
			return n.first;
		};
		int mapSize() {
			return pairs.size();
		};
};



// find word in vector given
Word* findInChain(vector<Word>* v, string word) {
	for (std::vector<Word>::iterator i = v->begin(); i != v->end(); i++) {
		if(strcmp(i->getWord().c_str(), word.c_str()) == 0) {
			return &(*i);
		}
	}
	return NULL;
}

int main(){

	srand(unsigned(time(0)));

	// declare word space and vector for all word objects
	string word, result;
	vector<string> text; 
	vector<Word> wordChain;
	set<string> uniqueWords;

	// Create period Word and add to word pool
	Word PERIOD(".");
	wordChain.push_back(PERIOD);


	// read input file and evaluate each word 
	ifstream myfile ("input.txt");
	if(myfile.is_open())
	{
		while( myfile >> word ) 
		{ 
			bool isPeriod = false;

			// make all words lowercase
			transform(word.begin(), word.end(), word.begin(), ::tolower);
		

			// check if a period is in the word, then add to words
			if( word[word.size() - 1] == '.') {
				isPeriod = true;
				word.pop_back();
			}

			// add word to text
			text.push_back(word);

			// add period if applicable
			if(isPeriod) {
				isPeriod = false;
				text.push_back(".");	
			}

		}
		myfile.close();
	}
	else 
		cout << "Unable to open file";


	// loop through text words (except '.') and add links to words

	// add unique words to word chain structure
	for (std::vector<string>::iterator i = text.begin(); i != text.end(); i++) {
		
		if( strcmp(i->c_str(), ".") != 0 && uniqueWords.count(*i) == 0 ) {	
			Word temp(*i);
			wordChain.push_back(temp);
			uniqueWords.insert(*i);
		}
		
	}

	// loop through to link words and fill dictionaries
	for (std::vector<string>::iterator i = text.begin(), i2 = ++text.begin(); i2 != text.end(); i++, i2++) {
		if( strcmp(i2->c_str(), ".") == 0 ) {
			findInChain(&wordChain, *i)->addWord(&PERIOD);
		} else {
			findInChain(&wordChain, *i)->addWord(findInChain(&wordChain, *i2));
		}
	}
	
	// get random starting word
	auto r = rand() % uniqueWords.size();
  	auto n = *select_random(uniqueWords, r);
	string temp = n;

	// trace words and add to buffer
	Word* currentWord = findInChain(&wordChain, temp);
	while(strcmp(currentWord->getWord().c_str(), ".") != 0 && (result.size() < TWEETSIZE || currentWord->mapSize() != 0)) {
		result += currentWord->getWord();
		result += " ";
		currentWord = currentWord->getRandNext();
	}

	// capitalize first letter and add period at the end
	result[0] = toupper(result[0]);
	result += ".";
	cout << result << endl;
	cout << "Characters: " << result.size() << endl;
	

/*
	// DEBUG!!!!!!!
	for (std::vector<Word>::iterator i = wordChain.begin(); i != wordChain.end(); i++) {
		cout << "Word: " << i->getWord() << endl; 
		i->printMap();
	}
	///////////

*/

	return 0;
}


