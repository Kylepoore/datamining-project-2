#include <unordered_map>
#include <array>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include "Ngram.h"

#define DELIMITER "-------------------------"

int s2rating(std::string s){
  if(s == "10/100"){
    return 1;
  }else if(s == "20/100"){
    return 2;
  }else if(s == "30/100"){
    return 3;
  }else if(s == "40/100"){
    return 4;
  }else if(s == "50/100"){
    return 5;
  }else if(s == "60/100"){
    return 6;
  }else if(s == "70/100"){
    return 7;
  }else if(s == "80/100"){
    return 8;
  }else if(s == "90/100"){
    return 9;
  }else{
    return 10;
  }
}

int main(int argc, char **argv){
  int i, n = 2;
  int rating;
  std::string word;
  int index = 0;
  if(argc < 5){
    std::cout << "Not enough filenames specified (n,input,vocab_out,result_out)" << std::endl;
  }
  n = atoi(argv[1]);
  std::ifstream in_file(argv[2]);
  
  //vocab index
  std::unordered_map < std::string, int > word_map;
  std::unordered_map < std::string, int >::iterator it;


  //ngram counter
  std::unordered_map < Ngram, std::array<int,10>, NgramHash, NgramEqual > ngram_map; 
  std::unordered_map < Ngram, std::array<int,10>, NgramHash, NgramEqual >::iterator ngram_it;

  Ngram ngram(n);

  while (in_file >> word){
    if(word == DELIMITER){
//      std::cout << "DELIMITER" << std::endl;
      in_file >> word;
      rating = s2rating(word);
//      std::cout << "rating: " << rating << std::endl;
      ngram.reset();
      continue;
    }
    //it = word_map.find(word);
    //if(it == word_map.end()){
    if(word_map.count(word) < 1){
//      std::cout << "new word(" << (index+1) << ")";
      //word_map.insert(std::make_pair<const std::string,int>(word,++index));
      word_map[word] = ++index;
      i = index;
    }else{
//      std::cout << "found word: " << word << std::endl;
      //i = it->second;
      i = word_map[word];
    }
    
//    std::cout << "{ ";
//    for (auto iter = word_map.begin(); iter != word_map.end(); ++iter)
//    {
//        std::cout << iter->first << ": " << iter->second << ", ";
//    }
//    std::cout << " }" << std::endl;

    ngram.add(i);
    ngram_it = ngram_map.find(ngram);
    if(ngram_it == ngram_map.end()){
//      std::cout << "new ngram: " << ngram << std::endl;
      std::array<int,10> init = {{0,0,0,0,0,0,0,0,0,0}};
      ngram_map.insert(std::make_pair<const Ngram, std::array<int,10 > >(ngram,init));
    }
//    std::cout << "foud ngram!" << std::endl;
    ngram_map.at(ngram)[rating-1] += 1;
//    std::array<int,10> v = ngram_map.at(ngram);
//    for(int idx=0;idx<10;idx++){
//      std::cout << v[idx] << " ";
//    }
//    std::cout << std::endl;
  }

  std::ofstream vocab_file(argv[3],std::ofstream::out);

  vocab_file << word_map.size() << std::endl;

  for(auto iter = word_map.begin(); iter != word_map.end(); iter++){
    vocab_file << iter->second << " " << iter->first << std::endl;
  }
  vocab_file.close();

  std::ofstream result_file(argv[4],std::ofstream::out);

  result_file << ngram_map.size() << " "<< n << std::endl;

  for(auto iter = ngram_map.begin(); iter != ngram_map.end(); iter++){
    result_file << iter->first;
    for(int i=0;i<10;i++){
      result_file << " " << iter->second[i];
    }
    result_file << std::endl;
  }
  result_file.close();
  return EXIT_SUCCESS;
}
