// This is the CPP file you will edit and turn in. (TODO: Remove this comment!)

#include <iostream>
#include "console.h"
#include "hashmap.h"
#include "vector.h"
#include <fstream>
#include "simpio.h"
#include "queue.h"
#include "random.h"
#include "filelib.h"

using namespace std;

void write_intro();
void get_parameters(ifstream&, int&);
HashMap<Queue<string>, Vector<string>> build_map(ifstream&, int);
void generate_text(HashMap<Queue<string>, Vector<string>>&, int);

int main() {
    write_intro();
    int n_groups;
    ifstream input;
    get_parameters(input, n_groups);
    HashMap<Queue<string>, Vector<string>> n_grams = build_map(input, n_groups);
    int num_words;
    while (true) {
        while (true) {
            num_words = getInteger("# of random words to generate (0 to quit)? ");
            if (num_words == 0 || num_words >= n_groups) {
                break;
            }
            cout << "Must be at least " << n_groups << " words.\n\n";
        }
        if (num_words == 0) {
            break;
        }
        generate_text(n_grams, num_words);
    }
    cout << "Exiting." << endl;
    return 0;
}

// writes intro message to the console
void write_intro() {
    string intro = "Welcome to CS 106B/X Random Writer ('N-Grams')!\n"
            "This program generates random text based on a document.\n"
            "Give me an input file and an 'N' value for groups\n"
            "of words, and I'll create random text for you.\n\n";
    cout << intro;
}

void get_parameters(ifstream& input, int& n_groups) {
    promptUserForFile(input, "Input file name: ");
    while (true) {
        n_groups = getInteger("Value of N: ");
        if (n_groups > 1) {
            break;
        }
        cout << "N must be 2 or greater.\n";
    }
    cout << "\n";
}

HashMap<Queue<string>, Vector<string>> build_map(ifstream& input, int n_groups) {
    HashMap<Queue<string>, Vector<string>> n_grams;
    Queue<string> group;
    int group_size = n_groups-1;
    string word;
    while (input >> word) {
        if (group.size() == group_size) {
            if (n_grams.containsKey(group)) {
                n_grams[group].add(word);
            }
            else {
                n_grams.put(group, Vector<string> {word});
            }
            group.dequeue();
        }
        group.enqueue(word);
    }
    input.clear();
    input.seekg(0, input.beg);
    for (int i = 0; i < group_size; i++) {
        input >> word;
        if (n_grams.containsKey(group)) {
            n_grams[group].add(word);
        }
        else {
            n_grams.put(group, Vector<string> {word});
        }
        group.dequeue();
        group.enqueue(word);
    }
    input.close();
    return n_grams;
}

void generate_text(HashMap<Queue<string>, Vector<string>>& n_grams, int num_words) {
    ostringstream text;
    Vector<Queue<string>> keys = n_grams.keys();
    Queue<string> prefix = keys[randomInteger(0, keys.size()-1)];
    text << "... ";
    for (string word : prefix) {
        text << word << " ";
    }
    num_words -= 2;
    for (int i = 0; i < num_words; i++) {
        Vector<string> suffixes = n_grams[prefix];
        string word = suffixes[randomInteger(0, suffixes.size()-1)];
        text << word << " ";
        prefix.dequeue();
        prefix.enqueue(word);
    }
    text << "...\n\n";
    cout << text.str();
}
