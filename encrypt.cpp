#include "encrypt.hpp"

encrypt::encrypt(int rounds, int words)
{
	num_rounds = rounds;
	num_words = words;
}

void encrypt::read_key(std::vector<unsigned char> & key_in)
{
	for (int index = 0; index < key_in.size(); ++index) {
		key[index] = key_in[index];
	}
}

void encrypt::key_expansion()
{
	int row, col;

	unsigned char temp[4], t;

	//Note that 2D arrays are can be represented by 1D arrays with a length of Row * Col
	for (row = 0; row < num_words; ++row)
	{
		for (int temp_col = 0; temp_col < 4; ++temp_col)
		{
			round_key[row * 4 + temp_col] = key[row * 4 + temp_col];
		}
	}

	while (row < 4 * (num_rounds + 1))
	{
		for (col = 0; col < 4; ++col)
		{
			temp[col] = round_key[(row - 1) * 4 + col];
		}

		if (row % num_words == 0)
		{
			//Rotate Left
			t = temp[0];
			temp[0] = temp[1];
			temp[1] = temp[2];
			temp[2] = temp[3];
			temp[3] = t;
			
			//pass values through S box
			for (int index = 0; index < 4; ++index)
			{
				temp[index] = s_box[temp[index]];
			}

			temp[0] = temp[0] ^ round_constant[row / num_words];
		}
		else if (num_words > 6 && row % num_words == 4)
		{
			//pass values through S box
			for (int index = 0; index < 4; ++index)
			{
				temp[index] = s_box[temp[index]];
			}
		}

		//pass values through S box
		for (int index = 0; index < 4; ++index)
		{
			round_key[row * 4 + index] = round_key[(row - num_words) * 4 + index] ^ temp[index];
		}
		
		//increment rows
		++row;
	}
}

std::array<unsigned char, 16> encrypt::encrypt_block(std::array<unsigned char, 16> & current_input)
{
	std::array<unsigned char, 16> current_output;

	int row, col, round = 0;

	//load input
	for (row = 0; row < 4; ++row)
	{
		for (col = 0; col < 4; ++col)
		{
			current_chunk[col][row] = current_input[row * 4 + col];
		}
	}

	//Initialize key
	r_key(0);

	//first n-1 rounds are the same
	for (round = 1; round < num_rounds; ++round)
	{
		sub_bytes();
		shift_rows();
		mix_columns();
		r_key(round);
	}

	//the last round doesnt have the mix col
	sub_bytes();
	shift_rows();
	r_key(num_rounds);

	//write everthing into the output array
	for (row = 0; row < 4; ++row)
	{
		for (col = 0; col < 4; ++col)
		{
			current_output[row * 4 + col] = current_chunk[col][row];
		}
	}

	return current_output;
}


void encrypt::r_key(int round)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			current_chunk[col][row] ^= round_key[round * 16 + 4 * row + col];
		}
	}
}

void encrypt::sub_bytes()
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			current_chunk[row][col] = s_box[current_chunk[row][col]];
		}
	}
}

void encrypt::shift_rows()
{
	unsigned char temp;

	//1st row rotates 1 column left
	temp = current_chunk[1][0];
	current_chunk[1][0] = current_chunk[1][1];
	current_chunk[1][1] = current_chunk[1][2];
	current_chunk[1][2] = current_chunk[1][3];
	current_chunk[1][3] = temp;

	//2nd row rotates 2 columns left
	temp = current_chunk[2][0];
	current_chunk[2][0] = current_chunk[2][2];
	current_chunk[2][2] = temp;

	temp = current_chunk[2][1];
	current_chunk[2][1] = current_chunk[2][3];
	current_chunk[2][3] = temp;

	//3rd row rotates 3 columns left
	temp = current_chunk[3][0];
	current_chunk[3][0] = current_chunk[3][3];
	current_chunk[3][3] = current_chunk[3][2];
	current_chunk[3][2] = current_chunk[3][1];
	current_chunk[3][1] = temp;
}

void encrypt::mix_columns()
{
	//implemented with help of paper here:
	//http://www.angelfire.com/biz7/atleast/mix_columns.pdf
	//and our good friend wikipedia:
	//https://en.wikipedia.org/wiki/Rijndael_MixColumns

	unsigned char temp_a, temp_b, temp_c;

	for (int row = 0; row < 4; ++row)
	{
		temp_a = current_chunk[0][row];
		temp_b = current_chunk[0][row] ^ current_chunk[1][row] ^ current_chunk[2][row] ^ current_chunk[3][row];
		
		temp_c = current_chunk[0][row] ^ current_chunk[1][row];
		temp_c = xtime(temp_c);
		current_chunk[0][row] ^= temp_c ^ temp_b;
		
		temp_c = current_chunk[1][row] ^ current_chunk[2][row];
		temp_c = xtime(temp_c);
		current_chunk[1][row] ^= temp_c ^ temp_b;
		
		temp_c = current_chunk[2][row] ^ current_chunk[3][row];
		temp_c = xtime(temp_c);
		current_chunk[2][row] ^= temp_c ^ temp_b;
		
		temp_c = current_chunk[3][row] ^ temp_a;
		temp_c = xtime(temp_c);
		current_chunk[3][row] ^= temp_c ^ temp_b;
	}
}

unsigned char encrypt::xtime(unsigned char & x)
{
	//using operation defined here:
	// https://www.usenix.org/legacy/event/cardis02/full_papers/valverde/valverde_html/node12.html

	//we shift out A7, then XOR with 0x1B IF A7 is 1 (A7 AND 1)
	return ((x << 1) ^ (((x >> 7) & 1) * 0x1B));
}
