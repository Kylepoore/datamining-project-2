#include <iostream>
#include <stdio.h>
#include <unordered_map>
#include <array>
#include <numeric>
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
  for(int i=0;i<data_size;i++){
    for(int j=0;j<n;j++){
      int tmp;
      data_file >> tmp;
      key.add(tmp);
    }
    data_file >> r0;
    data_file >> r1;
    data_file >> r2;
    data_file >> r3;
    data_file >> r4;
    data_file >> r5;
    data_file >> r6;
    data_file >> r7;
    data_file >> r8;
    data_file >> r9;
    std::array<int, 10> value = {{r0,r1,r2,r3,r4,r5,r6,r7,r8,r9}};
    if(i %1000 == 0)
      std::cout << "\r" << key << " " << r0 
                               << " " << r1 
                               << " " << r2 
                               << " " << r3 
                               << " " << r4 
                               << " " << r5 
                               << " " << r6 
                               << " " << r7 
                               << " " << r8 
                               << " " << r9; 
    ngram_map.insert({key, value});
  }
  std::cout << "\033[2k\rdone loading ngrams..." << std::endl;

  std::string word;
  int word_id;
  int review_counter = 0;
  Ngram ngram(n);
  std::array<double, 10> correction = {{22.075, 59.523, 44.052, 42.016, 27.777, 19.607, 11.415, 6.1199, 4.1736, 1.8089}};
  std::array<double, 10> scores = {{1,1,1,1,1,1,1,1,1,1}};
  std::cout << "enter a review: " << std::endl;
  while (std::cin >> word){
    if(word == REVIEW_DELIMITER || word == "endend"){
      //end of review
      int maxidx = -1;
      double max = -1;
      double sum = 0;
      for (int i=0;i<10;i++){
        if (scores[i] > max){
          max = scores[i];
          maxidx = i;
        }
        sum += scores[i];
      }
      
      for (int i=0;i<10;i++){
        printf("[%2d] %*s",i+1,(int)(scores[i] * 100 / sum),"*\n");
      }
      
      std::cout << "review: " << review_counter++ << " score: [" << maxidx+1 << "] = " << max << std::endl << REVIEW_DELIMITER << std::endl;
      ngram.reset();
      for (int i=0;i<10;i++){
        scores[i] = 1.0;
      }
      continue;
    }
    auto word_iter = word_map.find(word);
    if(word_iter == word_map.end()){
      word_id = -1;
    }else{
      word_id = word_iter->second;
    }
    ngram.add(word_id);
    auto ngram_iter = ngram_map.find(ngram);
    if(ngram_iter == ngram_map.end()){
      continue;
    }
    std::array<double, 10> result;
    for (int i=0;i<10;i++){
      result[i] = ngram_iter->second[i];
    }
//    std::cout << "found ngram!" << std::endl;
    double sum = std::accumulate(result.begin(),result.end(),0.0);
    sum += 100.0; //laplace smoothing
    for (int i=0;i<10;i++){
      double score = (correction[i]/196.5424) * (result[i] + 10.0) / sum; //laplace smoothing
      result[i] = score;
      scores[i] *= score;
    }
  }
}
