/*
 * Word ladder
 * by Lesley Wagner
 * 28-10-2018
 *
 * Takes two words and bridges from one word to another by changing one letter at a time,
 * where every word in the bridging sequence is also a valid word.
 */

#include <iostream>
#include "console.h"
#include "vector.h"
#include "simpio.h"
#include "lexicon.h"
#include "filelib.h"
#include "queue.h"
#include "stack.h"
#include "hashset.h"

using namespace std;

void print_intro();
Lexicon get_dictionary();
void get_words(string&, string&, const Lexicon&);
void get_ladder(string, string, const Lexicon&, ostringstream&);
Set<string> get_neighbors(string, const Lexicon&);

int main() {
    print_intro();
    const Lexicon lexicon = get_dictionary();
    string word1 = "";
    string word2 = "";
    while (true) {
        get_words(word1, word2, lexicon);
        if (word1 == "" || word2 == "") {
            break;
        }
        ostringstream ladder;
        get_ladder(word1, word2, lexicon, ladder);
        if (ladder.str().empty()) {
            cout << "No word ladder found from " << word1 << " back to " << word2 << ".\n\n";
        }
        else {
            cout << "A ladder from " << word1 << " back to " << word2 << ":\n";
            cout << ladder.str() << "\n\n";
        }
    }
    cout << "Have a nice day." << endl;
    return 0;
}

/*
 * Prints intro message to console.
 */
void print_intro() {
    string intro = "Welcome to CS 106B/X Word Ladder!\n"
            "Please give me two English words, and I will convert the\n"
            "first into the second by modifying one letter at a time.\n\n";
    cout << intro;
}

/*
 * Asks user for dictionary input file and returns lexicon of words in that file.
 */
Lexicon get_dictionary() {
    string file_name;
    while (true) {
        file_name = getLine("Dictionary file name: ");
        if (fileExists(file_name)) {
            break;
        }
        cout << "Unable to open that file. Try again.\n";
    }
    return Lexicon(file_name);
}

/*
 * Gets two words from the user that must be found in the dictionary,
 * must be of the same length and must be different.
 */
void get_words(string& word1, string& word2, const Lexicon& lexicon) {
    while (true) {
        word1 = getLine("Word 1 (or Enter to quit): ");
        // user pressed enter to quit
        if (word1 == "") {
            break;
        }

        word2 = getLine("Word 2 (or Enter to quit): ");
        // user pressed enter to quit
        if (word2 == "") {
            break;
        }
        // the words are the same
        else if (word1 == word2) {
            cout << "The two words must be different.\n\n";
            continue;
        }
        // the words are of different length
        else if (word1.length() != word2.length()) {
            cout << "The two words must be the same length\n\n";
            continue;
        }
        // one or both of the words are not found in the dictionary
        else if (!(lexicon.contains(word1) && lexicon.contains(word2))) {
            cout << "The two words must be found in the dictionary.\n\n";
            continue;
        }
        // the words are valid
        break;
    }
}

/*
 * Get a ladder between word1 and word2 by computing the shortest path between the words;
 * where every word in the path only differs by one letter from its neighbours and
 * every word is a valid word in the lexicon.
 */
void get_ladder(string word1, string word2, const Lexicon& lexicon, ostringstream& ladder) {
    Queue<Stack<string>> partial_paths;
    HashSet<string> words_used {word1};
    bool path_found = false;
    partial_paths.enqueue(Stack<string> {word1});
    while (!partial_paths.isEmpty()) {
        Stack<string> path = partial_paths.dequeue();
        Set<string> neighbors = get_neighbors(path.peek(), lexicon);
        Stack<string> new_path = path;
        for (string neighbor : neighbors) {
            if (neighbor == word2) {
                path_found = true;
                Stack<string> reversed_path;
                while (!path.isEmpty()) {
                    reversed_path.push(path.pop());
                }
                while (!reversed_path.isEmpty()) {
                    ladder << reversed_path.pop() << " ";
                }
                ladder << word2;
                break;
            }
            else if (!words_used.contains(neighbor)) {
                words_used.add(neighbor);
                new_path.push(neighbor);
                partial_paths.enqueue(new_path);
                new_path.pop();
            }
        }
        if (path_found) {
            break;
        }
    }
}

/*
 * Return neighbors of a word, where a neighbor is a word that only differs by one character
 * and it is included in the lexicon.
 */
Set<string> get_neighbors(string word, const Lexicon& lexicon) {
    Set<string> neighbors;
    for (size_t i = 0; i < word.length(); i++) {
        string new_word = word;
        for (int j = 'a'; j < 'a'+26; j++) {
            new_word[i] = char(j);
            if (char(j) != word[i] && lexicon.contains(new_word)) {
                neighbors.add(new_word);
            }
        }
    }
    return neighbors;
}
