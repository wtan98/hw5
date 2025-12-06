#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void helper(int curr, string pattern, string possibleWords, string& currWord, set<string>& correctWords, const set<string>& dict); 

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    set<string> correctWords; 

    string currWord = in;

    helper(0, in, floating, currWord, correctWords, dict);

    return correctWords; 
}

// Define any helper functions here
void helper(int curr, string pattern, string possibleLetters, string& currWord, set<string>& correctWords, const set<string>& dict)
{ 
    //check if all the spots have been filled
    if (curr == int(pattern.size())){
      if (possibleLetters.size() == 0){ //all possible letters used 
        //check if word is real 
        if (dict.find(currWord) != dict.end()){
          correctWords.insert(currWord);
        }
      }
      return; //done
    }

    //go next if there is already a letter 
    if(pattern[curr] != '-'){
      helper(curr+1, pattern, possibleLetters, currWord, correctWords, dict);
      return; 
    }

    //get the number of spots that are blank; is a "-"
    int blankCount = 0; 
    for (int i = curr; i < int(pattern.size()); i++){
      if (pattern[i] == '-'){
        blankCount++; 
      }
    }

    //first try the letters from possibleLetters
    for (int i = 0; i < int(possibleLetters.size()); i++){
      
      //add possible letters to the spot 
      char c = possibleLetters[i];
      currWord[curr] = c; 

      string newPossibleLetters = possibleLetters;  //make a new string without that letter 
      newPossibleLetters.erase(i, 1); 

      helper(curr+1, pattern, newPossibleLetters, currWord, correctWords, dict);
    }

    //otherwise, try regular letters iif there is space; blanks > possibleLetters
    if (blankCount > int(possibleLetters.size())){
      for(char a = 'a'; a <= 'z'; a++){
        currWord[curr] = a; 
        
        //go next 
        helper(curr+1, pattern, possibleLetters, currWord, correctWords, dict);
      }
    }
    
}