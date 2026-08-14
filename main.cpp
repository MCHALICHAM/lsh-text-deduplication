#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <climits>

using namespace std;

// create k-grams from text
set<string> getShingles(const string& text, int k){
    set<string> shingles;
    if(text.length() < (size_t)k){
        shingles.insert(text);
        return shingles;
    }
    for(size_t i = 0; i <= text.length() - k; i++){
        shingles.insert(text.substr(i, k));
    }
    return shingles;
}

unsigned int hashString(const string& str){
    unsigned int hash = 5381;
    for(char c : str){
        hash = ((hash << 5) + hash) ^ (unsigned int)c;
    }
    return hash;
}

// calculates min-hash signatures for docs
vector<vector<unsigned int>> computeMinHashSignatures(const vector<set<string>>& docsShingles, int numHashes, const vector<unsigned int>& a, const vector<unsigned int>& b){
    int numDocs = docsShingles.size();
    vector<vector<unsigned int>> signatures(numDocs, vector<unsigned int>(numHashes, UINT_MAX));

    for(int d = 0; d < numDocs; d++){
        for(const string& shingle : docsShingles[d]){
            unsigned int sHash = hashString(shingle);
            for(int h = 0; h < numHashes; h++){
                // basic hash combination
                unsigned int val = (a[h] * sHash + b[h]) ^ (a[h] >> 3);
                if(val < signatures[d][h]){
                    signatures[d][h] = val;
                }
            }
        }
    }
    return signatures;
}

// combine band vector into single hash string
string getBandHash(const vector<unsigned int>& sig, int start, int r){
    unsigned int combined = 17;
    for(int i = 0; i < r; i++){
        combined = combined * 31 + sig[start + i];
    }
    return to_string(combined);
}

int main(){
    // input prompts
    vector<string> documents = {
        "locality sensitive hashing is great for similarity search",
        "locality sensitive hashing is great for similarity search",
        "locality sensitive hashing is great for similarity search!",
        "locality sensitive hashing is awesome for similarity search",
        "data structures and algorithms final project",
        "data structures and algorithms final project for cs",
        "the weather today is warm and sunny outside",
        "the weather today is warm and sunny outside."
    };

    int k = 3;
    int numHashes = 20;
    int bands = 5;
    int r = 4; // numHashes = bands * r

    // generate shingles for all docs
    vector<set<string>> docsShingles;
    for(const string& doc : documents){
        docsShingles.push_back(getShingles(doc, k));
    }

    // random coefficients for hash functions
    vector<unsigned int> a = {13, 29, 37, 43, 53, 61, 71, 79, 89, 97, 101, 107, 113, 127, 131, 137, 139, 149, 151, 157};
    vector<unsigned int> b = {7, 11, 19, 23, 31, 41, 47, 59, 67, 73, 83, 103, 109, 137, 163, 173, 179, 181, 191, 193};

    // compute the min-hash signatures
    auto signatures = computeMinHashSignatures(docsShingles, numHashes, a, b);

    // lsh banding
    unordered_map<string, vector<int>> buckets;

    for(int bIdx = 0; bIdx < bands; bIdx++){
        for(int d = 0; d < (int)documents.size(); d++){
            string bandHash = getBandHash(signatures[d], bIdx * r, r);
            string bucketKey = to_string(bIdx) + "_" + bandHash;
            buckets[bucketKey].push_back(d);
        }
    }

    // groups candidate duplicates
    set<set<int>> candidateClusters;
    for(const auto& pair : buckets){
        if(pair.second.size() > 1){
            set<int> cluster(pair.second.begin(), pair.second.end());
            candidateClusters.insert(cluster);
        }
    }

    // prints the output buckets
    cout << "--- LSH Text Deduplication Results ---" << endl;
    int clusterId = 1;
    for(const auto& cluster : candidateClusters){
        cout << "\nBucket " << clusterId++ << ":" << endl;
        for(int docId : cluster){
            cout << "  [Doc " << docId << "]: " << documents[docId] << endl;
        }
    }

    return 0;
}
