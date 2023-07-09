#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

// Generate candidate itemsets of length k
vector<unordered_set<int>> generateCandidates(vector<unordered_set<int>>& frequentSets, int k) {
    vector<unordered_set<int>> candidates;

    int numSets = frequentSets.size();
    for (int i = 0; i < numSets; ++i) {
        for (int j = i + 1; j < numSets; ++j) {
            unordered_set<int> candidate;
            auto set1 = frequentSets[i];
            auto set2 = frequentSets[j];

            // Take the union of the two sets
            for (int item : set1)
                candidate.insert(item);
            for (int item : set2)
                candidate.insert(item);

            // If the candidate has the desired length, add it to the list
            if (candidate.size() == k)
                candidates.push_back(candidate);
        }
    }

    return candidates;
}

// Prune infrequent itemsets from the candidate set
vector<unordered_set<int>> pruneCandidates(vector<unordered_set<int>>& candidates, vector<vector<int>>& transactions, float minSupport) {
    vector<unordered_set<int>> frequentSets;

    // Count the occurrence of each candidate itemset
    vector<int> counts(candidates.size(), 0);
    for (int i = 0; i < candidates.size(); ++i) {
        for (const vector<int>& transaction : transactions) {
            bool present = true;
            for (int item : candidates[i]) {
                if (find(transaction.begin(), transaction.end(), item) == transaction.end()) {
                    present = false;
                    break;
                }
            }
            if (present)
                counts[i]++;
        }
    }

    // Remove duplicates from the counts vector
    sort(counts.begin(), counts.end());
    counts.erase(unique(counts.begin(), counts.end()), counts.end());

    // Add frequent itemsets to the list
    for (int i = 0; i < candidates.size(); ++i) {
        float support = static_cast<float>(counts[i]) / transactions.size();
        if (support >= minSupport)
            frequentSets.push_back(candidates[i]);
    }

    return frequentSets;
}

// Generate frequent itemsets using the Apriori algorithm
vector<unordered_set<int>> apriori(vector<vector<int>>& transactions, float minSupport) {
    vector<unordered_set<int>> frequentSets;
    int numItems = 0;

    // Find the maximum item value in the transactions
    for (const vector<int>& transaction : transactions) {
        for (int item : transaction) {
            numItems = max(numItems, item);
        }
    }

    // Initialize frequent 1-itemsets
    for (int i = 1; i <= numItems; ++i) {
        unordered_set<int> itemset = { i };
        frequentSets.push_back(itemset);
    }

    int k = 2;
    while (k <= numItems) {
        vector<unordered_set<int>> candidates = generateCandidates(frequentSets, k);
        frequentSets = pruneCandidates(candidates, transactions, minSupport);

        // Print frequent itemsets at this iteration
        cout << "Frequent " << k << "-itemsets:" << endl;
        for (const auto& itemset : frequentSets) {
            for (int item : itemset) {
                cout << item << " ";
            }
            cout << endl;
        }
        cout << endl;

        ++k;
    }

    return frequentSets;
}

int main() {
    // Example usage
    vector<vector<int>> transactions = {
        {1, 2, 3, 4},
        {1, 2, 4},
        {1, 2},
        {2, 3, 4},
        {2, 3},
        {3, 4},
        {2, 4}
    };

    float minSupport = 0.2;

    vector<unordered_set<int>> frequentSets = apriori(transactions, minSupport);

  // Print the frequent itemsets
    for (const auto& itemset : frequentSets) {
    for (int item : itemset) {
        cout << item << " ";
    }
    cout << endl;
}

    std::cout << "Press any key to exit..." << std::endl;
    std::cin.get(); // Wait for a key press before closing the console

    return 0;
}
