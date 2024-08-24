#include <bits/stdc++.h>
using namespace std;
const int N = 14855;
const int M = 2309;
const int K = 243;
const int L = 5;
const int T = 10;
const int G = 6;

vector<int> children[N][K];
string allowed_words[N];
map<string, int> allowed_words_map;
vector<string> secret_words;
int vis[26];
double entropy[N];
int idx[N];

int get_colors(int a, int b) {
    memset(vis, 0, sizeof(vis));
    for (int i = 0; i < L; ++i)
        vis[secret_words[b][i] - 'a']++;
    int code = 0;
    for (int i = 0; i < L; ++i) {
        int c = allowed_words[a][i] - 'a';
        if (allowed_words[a][i] == secret_words[b][i]) {
            code *= 3;
            --vis[c];
        } else if (vis[c] == 0) {
            code = 3 * code + 1;
        } else {
            code = 3 * code + 2;
            --vis[c];
        }
    }
    return code;
}

int get_code(string pattern) {
    int code = 0;
    for (int i = 0; i < L; ++i) {
        if (pattern[i] == 'g') {
            code = code * 3;
        } else if (pattern[i] == 'o') {
            code = code * 3 + 2;
        } else {
            code = code * 3 + 1;
        }
    }
    return code;
}

int main() {

    string allowed_words_filepath = "./lib/allowed-words.txt";
    ifstream allowed_words_file(allowed_words_filepath);
    if (!allowed_words_file.is_open()) {
        std::cerr << "Error: Could not open file " << allowed_words_filepath << std::endl;
        return 0;
    }

    for (int i = 0; i < N; ++i) {
        getline(allowed_words_file, allowed_words[i]);
        allowed_words_map[allowed_words[i]] = i;
    }
    allowed_words_file.close();

    string secret_words_filepath = "./lib/secret-words.txt";
    ifstream secret_words_file(secret_words_filepath);
    if (!secret_words_file.is_open()) {
        std::cerr << "Error: Could not open file " << secret_words_filepath << std::endl;
        return 0;
    }
    
    secret_words.resize(M);
    for (int i = 0; i < M; ++i) {
        getline(secret_words_file, secret_words[i]);
    }
    secret_words_file.close();

    for (int g = 1; g <= G; ++g) {
        int possible_secret_words = secret_words.size();
        cout << "==========================================================" << endl;
        if (possible_secret_words == 1) {
            cout << "Your secret word is: " << secret_words[0] << endl;
            cout << "==========================================================" << endl;
            break;
        }

        if (possible_secret_words < 1) {
            cout << "The secret word could not be found; please add it to the dictionary." << endl;
            cout << "==========================================================" << endl;
            break;
        }

        cout << "Number of possible secret words: " << possible_secret_words << endl;
        cout << "----------------------------------------------------------" << endl;
        
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < K; ++j) {
                children[i][j].clear();
            }
            for (int j = 0; j < possible_secret_words; ++j) {
                int color = get_colors(i, j);
                children[i][color].push_back(j);
            }
        }

        for (int i = 0; i < N; ++i) {
            entropy[i] = 0;
            for (int j = 0; j < K; ++j) {
                int count = children[i][j].size();
                if (count != 0) {
                    double p = (1.0 * count) / possible_secret_words;
                    entropy[i] += p * log2(1 / p);
                }
            }
        }

        for (int i = 0; i < N; ++i) idx[i] = i;
        sort(idx, idx + N, [](const int &a, const int &b){
            return entropy[a] > entropy[b];
        });

        
        
        cout << "Example secret words: " << "\n";
        for (int i = 0; i < T && i < possible_secret_words; ++i) {
            cout << secret_words[i] << "\n";
        }
        cout << "----------------------------------------------------------" << endl;

        cout << "Top guesses: " << "\n";
        for (int i = 0; i < T; ++i) {
            cout << allowed_words[idx[i]] << " " << entropy[idx[i]] << endl;
        }
        cout << "==========================================================" << endl;

        string guess;
        do {
            cout << "Enter guess..." << endl;
            cin >> guess;
        } while (allowed_words_map.find(guess) == allowed_words_map.end());
        cout << "Your guess: " << guess << endl;

        string pattern;
        cout << "Enter pattern..." << endl;
        cin >> pattern;
        cout << "Your pattern: " << pattern << endl;
        
        int code = get_code(pattern);
        int guess_id = allowed_words_map[guess];
        
        vector<string> new_secret_words;
        for (int sid : children[guess_id][code]) {
            new_secret_words.push_back(secret_words[sid]);
        }
        secret_words = new_secret_words;
    }
}
