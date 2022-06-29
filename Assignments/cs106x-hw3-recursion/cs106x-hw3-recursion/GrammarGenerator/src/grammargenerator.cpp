/*
 * File: grammargenerator.cpp
 * --------------------------
 * Name:
 * Section leader:
 * This file contains grammar generating code for CS106X HW3.
 */

#include "grammargenerator.h";
#include "hashmap.h";
#include "vector.h";
#include "random.h";

using namespace std;

HashMap<string, Vector<Vector<string>>> generateMap(istream& input) {
    HashMap<string, Vector<Vector<string>>> grammar;
    string line;
    while (getline(input, line)) {
        Vector<string> definition = stringSplit(line, "::=");
        string nonterminal = trim(definition[0]);
        if (grammar.containsKey(nonterminal)) {
            throw "nonterminal key is defined twice";
        }
        Vector<Vector<string>> rules;
        Vector<string> symbols = stringSplit(trim(definition[1]), "|");
        for (string rule : symbols) {
            Vector<string> tokens = stringSplit(trim(rule), " ");
            rules.add(tokens);
        }
        grammar.put(nonterminal, rules);
    }
    return grammar;
}

// generates grammar for a given symbol recursively
void grammarHelper(const string& symbol, string& sentence, HashMap<string, Vector<Vector<string>>>& grammar) {
    if (!grammar.containsKey(symbol)) {
        sentence += symbol + " ";
    }
    else {
        Vector<string> rule = randomElement(grammar[symbol]);
        for (string symbol : rule) {
            grammarHelper(symbol, sentence, grammar);
        }
    }
}

/**
 * Generates grammar for a given symbol a certain number of times given
 * a BNF input file.
 *
 * This will be called by grammarmain.cpp.
 *
 * @param input - Input stream of BNF file.
 * @param symbol - Symbol to generate
 * @param times - Number of times grammar is generated
 * @return Vector of strings of size times with random generations of symbol
 */
Vector<string> generateGrammar(istream& input, const string& symbol, int times) {
    if (symbol.empty()) {
        throw "symbol must be non-empty";
    }
    Vector<string> sentences;
    HashMap<string, Vector<Vector<string>>> grammar = generateMap(input);
    for (int i = 0; i < times; i++) {
        string sentence = "";
        grammarHelper(symbol, sentence, grammar);
        sentence = trim(sentence);
        sentences.add(sentence);
    }
    return sentences;
}
