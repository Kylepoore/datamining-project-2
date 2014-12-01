#include <iostream>
#include <unordered_map>
#include <array>
#include <stdlib.h>
#include <fstream>
#include "Ngram.h"

#define REVIEW_DELIMITER "-------------------------"


int main (int argc, char ** argv){
  int vocab_size, data_size, n;
  if(argc < 3){
    std::cout << "usage: " << argv[0] << " vocab_file data_file" << std::endl;
    exit(EXIT_FAILURE);
  }


  std::unordered_map<std::string, int> word_map;

  std::unordered_map<Ngram,std::array<int, 10>,NgramHash,NgramEqual> ngram_map;


  std::ifstream vocab_file (argv[1]);

  if( ! vocab_file.is_open()){
    std::cout << "unable to open file: " << argv[1] << std::endl;
    exit(EXIT_FAILURE);
  }
  vocab_file >> vocab_size;
  for(int i=0;i<vocab_size;i++){
    std::string key;
    int value;
    vocab_file >> value >> key;
    if(i%100==0)
      std::cout << "\033[2k\r" << value << ": " << key;
    word_map.insert({key, value});
  }

  std::cout << "\033[2k\rdone loading vocab...                                            " << std::endl;

  std::ifstream data_file (argv[2]);

  if( ! data_file.is_open()){
    std::cout << "unable to open file: " << argv[1] << std::endl;
    exit(EXIT_FAILURE);
  }
  data_file >> data_size >> n;
  Ngram key(n);
  int r0,r1,r2,r3,r4,r5,r6,r7,r8,r9;
  for(int i=0;i<vocab_size;i++){
    for(int j=0;j<n;j++){
      int tmp;
      data_file >> tmp;
      key.add(tmp);
    }
    data_file >> r0 >> r1 >> r2 >> r3 >> r4 >> r6 >> r7 >> r8 >> r9;
    std::array<int, 10> value = {{r0,r1,r2,r3,r4,r5,r6,r7,r8,r9}};
    if(i%100 == 0)
      std::cout << "\033[2k\r" << key << "          "; 
    ngram_map.insert({key, value});
  }
  std::cout << "\033[2k\rdone loading ngrams..." << std::endl;

}
