#include "whitehead_automorphisms.h"

// Function to compute the inverse of a word
Word invert_word(const Word& w) {
    Word inv;
    for(auto it = w.rbegin(); it != w.rend(); ++it) {
        inv.push_back(-(*it));
    }
    return inv;
}

// Function to reduce a word by canceling adjacent inverse elements
Word reduce_word(const Word& w) {
    Word reduced;
    for(auto c : w){
        if(!reduced.empty() && reduced.back() == -c){
            reduced.pop_back();
        }
        else{
            reduced.push_back(c);
        }
    }
    return reduced;
}

// Function to apply an automorphism to a word and return the reduced word
Word apply_automorphism(const Automorphism& phi, const Word& w, int r){
    Word transformed;
    for(auto c : w){
        if(c > 0){
            // c is generator x_i
            if(c > r){
                // Invalid generator index
                continue;
            }
            for(auto x : phi.images[c]){
                transformed.push_back(x);
            }
        }
        else{
            // c is x_i^{-1}, replace with inverse of phi(x_i)
            int gen = -c;
            if(gen > r){
                // Invalid generator index
                continue;
            }
            Word inv = invert_word(phi.images[gen]);
            for(auto x : inv){
                transformed.push_back(x);
            }
        }
    }
    // Reduce the transformed word
    return reduce_word(transformed);
}

// Function to generate all Type II Whitehead automorphisms
vector<Automorphism> generate_typeII(int r){
    vector<Automorphism> typeII;
    // Iterate over all possible base generators a
    for(int a = 1; a <= r; ++a){
        // Iterate over all possible choices for each generator to multiply by a or not
        // For each generator b != a, decide whether to map b to b, b * a, or b * a^{-1}
        // This leads to 3^{r-1} automorphisms for each base a
        int num_other = r - 1;
        long long total = pow(3, num_other); // Using long long to prevent overflow for larger r
        for(long long mask = 0; mask < total; ++mask){
            Automorphism phi;
            phi.images.resize(r+1, Word()); // 1-based indexing
            phi.images[a].push_back(a); // Base generator maps to itself
            long long temp = mask;
            for(int b = 1; b <= r; ++b){
                if(b == a) continue;
                int choice = temp % 3;
                temp /= 3;
                if(choice == 0){
                    // b maps to itself
                    phi.images[b].push_back(b);
                }
                else if(choice == 1){
                    // b maps to b * a
                    phi.images[b].push_back(b);
                    phi.images[b].push_back(a);
                }
                else{
                    // b maps to b * a^{-1}
                    phi.images[b].push_back(b);
                    phi.images[b].push_back(-a);
                }
            }
            typeII.push_back(phi);
        }
    }
    return typeII;
}

// Function to determine if a word is minimal in its automorphism orbit
bool is_minimal(const Word& word, int r, const vector<Automorphism>& automorphisms) {
    Word reduced_original = reduce_word(word);
    int original_length = reduced_original.size();
    
    // Iterate through all automorphisms and check if any reduces the word length
    for(auto& phi : automorphisms){
        Word transformed = apply_automorphism(phi, reduced_original, r);
        int transformed_length = transformed.size();
        if(transformed_length < original_length){
            // Found an automorphism that reduces the length
            return false;
        }
    }
    // No automorphism reduces the length
    return true;
}

// Function to get a minimal word in the automorphism orbit
Word get_minimal_word(const Word& input_word, int r){
    // Generate all Type II Whitehead automorphisms
    vector<Automorphism> automorphisms = generate_typeII(r);
    
    // Initialize the current word as the reduced input word
    Word current = reduce_word(input_word);
    
    bool changed = true;
    while(changed){
        changed = false;
        Word best = current;
        int best_length = current.size();
        
        // Iterate through all automorphisms to find the one that minimizes the word length
        for(auto& phi : automorphisms){
            Word transformed = apply_automorphism(phi, current, r);
            int transformed_length = transformed.size();
            if(transformed_length < best_length){
                best = transformed;
                best_length = transformed_length;
                changed = true;
                // Early exit for the current iteration if a better automorphism is found
                // Uncomment the following line to prioritize the first found reduction
                // break;
            }
        }
        
        if(changed){
            current = best;
        }
    }
    
    return current;
}
