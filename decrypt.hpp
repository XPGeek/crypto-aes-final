#ifndef DECRYPT_HPP
#define DECRYPT_HPP

#include <stdio.h>
#include <vector>

//"You get what you get, and don't throw a fit." -Wyatt 2017

class decrypt {
public:

  decrypt();

private:

  int num_rounds;
  int num_words;

  unsigned char round_key[240];

  unsigned char key[32];

  std::vector<unsigned int[16]> input_arrays;
  std::vector<unsigned int[16]> input_arrays;
  std::vector<unsigned int[16]> input_arrays;
};

#endif
