#include "Ngram.h"

Ngram::Ngram(int num_words) :
  n(num_words), grams(new int[n]) 
{
}

Ngram::Ngram(const Ngram &obj) :
  n(obj.n), grams(new int[obj.n])
{
  for(int i=0;i<(*this).n;i++){
    grams[i] = obj.grams[i];
  }
}

Ngram::Ngram(Ngram&& other) :
  n(other.n), grams(other.grams)
{
  other.grams = NULL;  
}

Ngram::~Ngram() {
  delete [] grams;
}

Ngram& Ngram::operator=(const Ngram& other){
  Ngram tmp(other);
  *this = std::move(tmp);
  return *this;
}

Ngram& Ngram::operator=(Ngram&& other){
  std::swap(grams, other.grams);
  std::swap(n, other.n);
  return *this;
}

void Ngram::reset(){
  for(int i=0;i<n;i++){
    grams[i] = 0;
  }
}

std::ostream& operator<< (std::ostream &out, const Ngram ngram){
  for(int i=0;i<ngram.n;i++){
    out << ngram.grams[i] << " ";
  }
  return out;
}

void Ngram::add(int x){
  //Ngram ngram(*this);
  for(int i=0;i<n-1;i++){
    grams[i] = grams[i+1];
  }
  grams[n-1] = x;
}

size_t NgramHash::operator()(const Ngram& k) const{
  long unsigned int h = 0;
  for(int i = 0; i < k.n; i++){
    h ^= k.grams[i] << i;
  }
  return h;
}

bool NgramEqual::operator()(const Ngram& k1, const Ngram& k2) const{
  if(k1.n != k2.n){
    return false;
  }
  for(int i=0;i<k1.n;i++){
    if(k1.grams[i] != k2.grams[i])
      return false;
  }
  return true;
}
