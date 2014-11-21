#include "ngram.h"

NGram::NGram(int num_words){
  n = num_words;
  grams = new long unsigned int [n];
}
NGram::~NGram(){
  delete [] grams;
}

long unsigned int NGram::hash(){
  long unsigned int h = 0;
  for(int i = 0; i < n; i++){
    h ^= grams[i] << i;
  }
  return h;
}

bool NGram::equal(NGram& other){
  for(int i=0;i<n;i++){
    if(grams[i] != other.grams[i])
      return false;
  }
  return true;
}
