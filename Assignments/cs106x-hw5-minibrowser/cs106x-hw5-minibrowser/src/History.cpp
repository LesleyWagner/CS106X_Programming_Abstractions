#include "History.h"
#include "error.h"
using namespace std;

History::History() {
    current = nullptr;
}

History::~History() {
    if (current) {
        Node* node = current->next;
        while (node) {
            Node* trash = node;
            node = node->next;
            delete trash;
        }
        node = current;
        while (node) {
            Node* trash = node;
            node = node->prev;
            delete trash;
        }
    }
}

bool History::hasNext() const {
    if (current) {
        if (current->next) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool History::hasPrevious() const {
    if (current) {
        if (current->prev) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

string History::next() {
    if (!(current && current->next)) {
        error("No next page.");
    }
    else {
        Node* temp = current;
        current = current->next;
        current->prev = temp;
    }
    return current->title;
}

string History::previous() {
    if (!(current && current->prev)) {
        error("No prev page.");
    }
    else {
        Node* temp = current;
        current = current->prev;
        current->next = temp;
    }
    return current->title;
}

void History::goTo(const string& page) {
    if (current) {
        Node* node = current;
        current = new Node{ page, node, nullptr };
        while (node->next) {
            Node* trash = node->next;
            node = node->next;
            delete trash;
        }
    }
    else {
        current = new Node{ page, nullptr, nullptr };
    }
}
