#include <stdlib.h>
#include <string>
#include <ostream>
#include <iostream>
#include <cstddef>

class Ngram {
  public:
    int n;
    int *grams;
    Ngram(int num_words);
    Ngram(const Ngram &obj);
    Ngram(Ngram&& other);
    ~Ngram();
    Ngram& operator=(const Ngram& other);
    Ngram& operator=(Ngram&& other);
    void add(int x) ;
    friend std::ostream& operator<< (std::ostream &out, Ngram ngram);
    void reset();
//    long unsigned int hash();
//    bool equal(NGram& other);
};

class NgramHash {
  public:
  size_t operator()(const Ngram& k) const;
};

class NgramEqual {
  public:
  bool operator()(const Ngram& k1, const Ngram& k2) const;
};
