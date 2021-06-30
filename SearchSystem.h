#ifndef SEARCH_SYSTEM
#define SEARCH_SYSTEM

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <typeinfo>

using namespace std;

bool comparator(const pair<string, pair<int, int>>& a, const pair<string, pair<int, int>>& b) {
    if (a.second.second > b.second.second)
        return true;
    else if (a.second.second == b.second.second) {
        if (a.second.first > b.second.first)
            return true;
        else if (a.second.first == b.second.first) 
            return a.first < b.first;
    }
    
    return false;
}

//###splitting a string into words and adding them to a set or vector###
void SplitIntoWords(const string& text, auto& res) {
	string word;
	for (const char c : text) {
		if (c == ' ') {
            if (typeid(res) == typeid(vector<string>&)) {
                vector<string>& words = (vector<string>&)res;
			    words.push_back(word);
            }
            else {
                set<string>& words = (set<string>&)res;
                words.insert(word);
            }
                
			word = "";
		} 
        else 
			word += c;
	}

    if (typeid(res) == typeid(vector<string>&)) {
        vector<string>& words = (vector<string>&)res;
		words.push_back(word);
    }
    else {
        set<string>& words = (set<string>&)res;
        words.insert(word);
    }
}
//######################################################################

class SearchServer {
    private:
        string config;
        set<string> query, stop_words;
        //#####file, <counter of words, relevance#####
        map<string, pair<int, int>> relevance_table;

    public:
        SearchServer(const string& config, const string& query, const set<string>& stop_words) {
            this->config = config;
            this->stop_words = stop_words;

            set<string> query_split;
            SplitIntoWords(query, query_split);
            for (const string& word : query_split) {
                if (!(this->stop_words.count(word))) 
                    this->query.insert(word);
            }
        }

        void Search(ifstream &&fin) {
            string file_name;

            while (fin >> file_name)
                relevance_table[file_name] = make_pair(0, 0);
             
            //###################counting words and relevance by query###################    
            for (auto it = relevance_table.begin(); it != relevance_table.end(); ++it) {
                ifstream file(it->first);

                string str;
                set<string> relevance_words;

                while (getline(file, str)) {
                    vector<string> file_str;
                    
                    SplitIntoWords(str, file_str);

                    for (const string& word : file_str) {
                        if (query.count(word)) {
                            ++relevance_table[it->first].first;
                            if (!relevance_words.count(word)) {
                                relevance_words.insert(word);
                                ++relevance_table[it->first].second;
                            }
                        }
                    }
                }
                
                file.close();
            }
            //###########################################################################

            //#############################sorting by relevance#############################           
            vector<pair<string, pair<int, int>>> sorted_by_relevance;
            for (const auto& it : relevance_table) 
                sorted_by_relevance.push_back(make_pair(it.first, it.second));

            sort(sorted_by_relevance.begin(), sorted_by_relevance.end(), comparator);

            for (const auto& it : sorted_by_relevance) 
                cout << it.first <<" : "<< it.second.first <<" : "<< it.second.second << endl;
            //##############################################################################

            fin.close();

        }
};

#endif