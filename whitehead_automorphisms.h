#include <vector>

using namespace std;
// Type alias for a word in the free group
typedef vector<int> Word;

// Structure to represent a Whitehead automorphism
struct Automorphism {
    // images[1..r] contains the image of each generator under the automorphism
    // images[0] is unused to make it 1-based indexing
    vector<Word> images;
};
Word get_minimal_word(const Word& input_word, int r);
bool is_minimal(const Word& word, int r, const vector<Automorphism>& automorphisms);
vector<Automorphism> generate_typeII(int r);