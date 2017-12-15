#include "decrypt.hpp"

decrypt::decrypt(int num_rounds, int num_words, int num_keys)
{
}

void decrypt::key_expansion()
{
	int row;
	int col;

	unsigned char temp[4];
	unsigned char overflow;

	for(row = 0; row < num_words; ++row)
	{
		
		round_key[row * 4] = key[row * 4];
		round_key[row * 4 + 1] = key[row * 4 + 1];
		round_key[row * 4 + 2] = key[row * 4 + 2];
		round_key[row * 4 + 3] = key[row * 4 + 3];
	}

	while (row < (4 * (num_rounds+1)))
	{
		for(col = 0; col < 4; ++col)
		{
			temp[col]=round_key[(row-1) * 4 + col];
		}

		if (row % num_words == 0)
		{

			overflow = temp[0];
			temp[0] = temp[1];
			temp[1] = temp[2];
			temp[2] = temp[3];
			temp[3] = overflow;

			temp[0] = s_box[temp[0]];
			temp[1] = s_box[temp[1]];
			temp[2] = s_box[temp[2]];
			temp[3] = s_box[temp[3]];

			temp[0] =  temp[0] ^ round_constants[row/num_words];
		}
		else if (num_words > 6 && row % num_words == 4)
		{

			temp[0] = s_box[temp[0]];
			temp[1] = s_box[temp[1]];
			temp[2] = s_box[temp[2]];
			temp[3] = s_box[temp[3]];

		}
		round_key[row * 4 + 0] = round_key[(row - num_words) * 4 + 0] ^ temp[0];
		round_key[row * 4 + 1] = round_key[(row - num_words) * 4 + 1] ^ temp[1];
		round_key[row * 4 + 2] = round_key[(row - num_words) * 4 + 2] ^ temp[2];
		round_key[row * 4 + 3] = round_key[(row - num_words) * 4 + 3] ^ temp[3];
		row++;
	}
}