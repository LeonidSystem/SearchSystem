#include "SearchSystem.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Too many arguments" << endl;
        return -1;
    }
    string config, query, stop_words;
    set<string> stop_words_split;

    config = argv[1];

    ifstream fin(config);

    getline(fin, stop_words);
    getline(fin, query);

    SplitIntoWords(stop_words, stop_words_split);

    SearchServer *server = new SearchServer(config, query, stop_words_split);
    server->Search(move(fin));
}