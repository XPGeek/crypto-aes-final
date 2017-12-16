#include "decrypt.hpp"

decrypt::decrypt(int rounds, int words)
{
}

void decrypt::read_key(std::vector<unsigned char>& key_in)
{
}

void decrypt::key_expansion()
{
}

std::array<unsigned char, 16> decrypt::decrypt_block(std::array<unsigned char, 16>& current_input)
{
	return std::array<unsigned char, 16>();
}

void decrypt::r_key(int round)
{
}

void decrypt::inv_sub_bytes()
{
}

void decrypt::inv_shift_rows()
{
}

void decrypt::inv_mix_columns()
{
}

unsigned char decrypt::xtime(unsigned char & x)
{
	return 0;
}

unsigned char decrypt::poly_mult(unsigned char & multiplicand, unsigned char & multipliier)
{
	return 0;
}
