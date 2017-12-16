#include "decrypt.hpp"

decrypt::decrypt(int rounds, int words)
{
	num_rounds = rounds;
	num_words = words;
}

void decrypt::read_key(std::vector<unsigned char>& key_in)
{
	for (int index = 0; index < key_in.size(); ++index) {
		key[index] = key_in[index];
	}
}

void decrypt::key_expansion()
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

std::array<unsigned char, 16> decrypt::decrypt_block(std::array<unsigned char, 16>& current_input)
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
	r_key(num_rounds);

	//first n-1 rounds are the same
	for (round = num_rounds - 1; round > 0; --round)
	{
		inv_shift_rows();
		inv_sub_bytes();
		r_key(round);
		inv_mix_columns();
	}

	//the last round doesnt have the mix col
	inv_shift_rows();
	inv_sub_bytes();
	r_key(0);

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

void decrypt::r_key(int round)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			current_chunk[col][row] ^= round_key[round * 16 + 4 * row + col];
		}
	}
}

void decrypt::inv_sub_bytes()
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			current_chunk[row][col] = inv_s_box[current_chunk[row][col]];
		}
	}
}

void decrypt::inv_shift_rows()
{
	unsigned char temp;

	//1st row rotates 1 column right
	temp = current_chunk[1][3];
	current_chunk[1][3] = current_chunk[1][2];
	current_chunk[1][2] = current_chunk[1][1];
	current_chunk[1][1] = current_chunk[1][0];
	current_chunk[1][0] = temp;

	//2nd row rotates 2 columns right
	temp = current_chunk[2][0];
	current_chunk[2][0] = current_chunk[2][2];
	current_chunk[2][2] = temp;

	temp = current_chunk[2][1];
	current_chunk[2][1] = current_chunk[2][3];
	current_chunk[2][3] = temp;

	//3rd row rotates 3 columns right
	temp = current_chunk[3][0];
	current_chunk[3][0] = current_chunk[3][1];
	current_chunk[3][1] = current_chunk[3][2];
	current_chunk[3][2] = current_chunk[3][3];
	current_chunk[3][3] = temp;
}

void decrypt::inv_mix_columns()
{
	unsigned char row_a, row_b, row_c, row_d;

	for (int col = 0; col < 4; ++col)
	{

		row_a = current_chunk[0][col];
		row_b = current_chunk[1][col];
		row_c = current_chunk[2][col];
		row_d = current_chunk[3][col];


		current_chunk[0][col] = poly_mult(row_a, 0x0E) ^ poly_mult(row_b, 0x0B) ^ poly_mult(row_c, 0x0d) ^ poly_mult(row_d, 0x09);
		current_chunk[1][col] = poly_mult(row_a, 0x09) ^ poly_mult(row_b, 0x0E) ^ poly_mult(row_c, 0x0b) ^ poly_mult(row_d, 0x0d);
		current_chunk[2][col] = poly_mult(row_a, 0x0D) ^ poly_mult(row_b, 0x09) ^ poly_mult(row_c, 0x0e) ^ poly_mult(row_d, 0x0b);
		current_chunk[3][col] = poly_mult(row_a, 0x0B) ^ poly_mult(row_b, 0x0D) ^ poly_mult(row_c, 0x09) ^ poly_mult(row_d, 0x0e);
	}
}

unsigned char decrypt::xtime(unsigned char & x)
{
	//using operation defined here:
	// https://www.usenix.org/legacy/event/cardis02/full_papers/valverde/valverde_html/node12.html

	//we shift out A7, then XOR with 0x1B IF A7 is 1 (A7 AND 1)
	return ((x << 1) ^ (((x >> 7) & 1) * 0x1B));
}

unsigned char decrypt::poly_mult(unsigned char multiplicand, unsigned char multipliier)
{
	unsigned char int_product_a = xtime(multiplicand);
	unsigned char int_product_b = xtime(int_product_a);
	unsigned char int_product_c = xtime(int_product_b);
	unsigned char int_product_d = xtime(int_product_c);

	return (((multipliier & 1) * multiplicand) ^ ((multipliier >> 1 & 1) * int_product_a) ^ ((multipliier >> 2 & 1) * int_product_b) ^ ((multipliier >> 3 & 1) * int_product_c) ^ ((multipliier >> 4 & 1) * int_product_d));
}
