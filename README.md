# Locality-Sensitive Hashing (LSH) for Text Deduplication

LSH is the data structure implemented in this project. This project implements an efficient approximate nearest-neighbor search to identify similar text prompts. Instead of O(N^2) exact string matching, LSH reduces the searcg space by hashing similar items in the same buckets. My program breaks the text down into what we call k-grams, condenses them into numerical signatures, then divides these signature into bands. If the two documents share an identical hash within a band, they're clustered as a candidate pair for deduplication.

# How to run the code:

1. In the terminal, cd to the project directory.
2. Compile with: `g++ -std=c++11 main.cpp -o lsh_program`
3. Then run: `./lsh_program`

The biggest problem I ran into coding this project scratch was with the hyperparameter tuning. With balancing the k-gram size (k=3), the total number of hash functions (numHashes=20), and the number of bands (bands=5) needed  experimentation. Finding the optimal threshold to minimize both false positives (dissimilar strings sharing a bucket) and false negatives (missing actual duplicates) took a lot of trial and error. Using the unordered_map to ensure efficient memory management also threw me off since I didn't use external libraries.

# Walkthrough
1. The Input Data
Here is my hardcoded dataset for the algorithm. Look at the slight variations in punctuation designed specifically to trick standard exact-string matching:

<img width="794" height="389" alt="image" src="https://github.com/user-attachments/assets/bb86342b-fa63-408d-9f70-1c22cce229ca" />


2. The Final Output
After running the data through the Shingling and MinHashing, the algorithm will bypass the typos and group the clusters into their respective buckets:

<img width="892" height="343" alt="image" src="https://github.com/user-attachments/assets/a2eba718-45b0-4d45-83ce-e94ee30d4a3d" />
