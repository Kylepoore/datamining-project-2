#include <stdlib.h>
#include <string>

class NGram {
  private:
    int n;
    long unsigned int *grams;
  public:
    NGram(int num_words);
    ~NGram();

    long unsigned int hash();
    bool equal(NGram& other);
};
