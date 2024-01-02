#include <iostream>
#include <cmath>
#include<complex>
#include<chrono>

//INLINE IMPLEMENTATION OF FFT8

const std::complex<double> W0_2(1, 0);
const std::complex<double> W0_4(1, 0);
const std::complex<double> W1_4(0, -1);
const std::complex<double> W0_8(1, 0);
const std::complex<double> W1_8((sqrt(2) / 2), -(sqrt(2) / 2));
const std::complex<double> W2_8(0, -1);
const std::complex<double> W3_8(-(sqrt(2) / 2), -(sqrt(2) / 2));

int main()
{
	std::complex<double> input0 = 100, input1 = 170, input2 = 200, input3 = 170, input4 = 100, input5 = 29, input6 = 0, input7 = 29;
	std::complex<double> reorder_out0, reorder_out1, reorder_out2, reorder_out3, reorder_out4, reorder_out5, reorder_out6, reorder_out7;
	std::complex<double> fft2_0_out0, fft2_0_out1, fft2_1_out0, fft2_1_out1, fft2_2_out0, fft2_2_out1, fft2_3_out0, fft2_3_out1;
	std::complex<double> fft4_0_out_0, fft4_0_out_1, fft4_0_out_2, fft4_0_out_3, fft4_1_out_0, fft4_1_out_1, fft4_1_out_2, fft4_1_out_3;
	std::complex<double> fft8_0, fft8_1, fft8_2, fft8_3, fft8_4, fft8_5, fft8_6, fft8_7;
	auto start = std::chrono::high_resolution_clock::now();
	for(int i=0;i<20000;i++){
		//-------------------REORDER
		reorder_out0 = input0;
		reorder_out1 = input4;
		reorder_out2 = input2;
		reorder_out3 = input6;
		reorder_out4 = input1;
		reorder_out5 = input5;
		reorder_out6 = input3;
		reorder_out7 = input7;
		//--------------------STAGE1
		fft2_0_out0 = reorder_out0 + reorder_out1;
		fft2_0_out1 = reorder_out0 - reorder_out1;

		fft2_1_out0 = reorder_out2 + reorder_out3;
		fft2_1_out1 = reorder_out2 - reorder_out3;

		fft2_2_out0 = reorder_out4 + reorder_out5;
		fft2_2_out1 = reorder_out4 - reorder_out5;

		fft2_3_out0 = reorder_out6 + reorder_out7;
		fft2_3_out1 = reorder_out6 - reorder_out7;
		//--------------------STAGE2
		fft4_0_out_0 = fft2_0_out0 + W0_4 * fft2_1_out0;
		fft4_0_out_1 = fft2_0_out1 + W1_4 * fft2_1_out1;
		fft4_0_out_2 = -W0_4 * fft2_1_out0 + fft2_0_out0;
		fft4_0_out_3 = -W1_4 * fft2_1_out1 + fft2_0_out1;

		fft4_1_out_0 = fft2_2_out0 + W0_4 * fft2_3_out0;
		fft4_1_out_1 = fft2_2_out1 + W1_4 * fft2_3_out1;
		fft4_1_out_2 = -W0_4 * fft2_3_out0 + fft2_2_out0;
		fft4_1_out_3 = -W1_4 * fft2_3_out1 + fft2_2_out1;
		//---------------------STAGE3
		fft8_0 = fft4_0_out_0 + W0_8 * fft4_1_out_0;
		fft8_1 = fft4_0_out_1 + W1_8 * fft4_1_out_1;
		fft8_2 = fft4_0_out_2 + W2_8 * fft4_1_out_2;
		fft8_3 = fft4_0_out_3 + W3_8 * fft4_1_out_3;
		fft8_4 = -W0_8 * fft4_1_out_0 + fft4_0_out_0;
		fft8_5 = -W1_8 * fft4_1_out_1 + fft4_0_out_1;
		fft8_6 = -W2_8 * fft4_1_out_2 + fft4_0_out_2;
		fft8_7 = -W3_8 * fft4_1_out_3 + fft4_0_out_3;
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto processed_time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()/20000;
	std::cout << "AVERAGE Processing Time INLINE : " << processed_time << " (microsecond)\n";
	//----------------------RESULTS
	std::cout << fft8_0 << std::endl;
	std::cout << fft8_1 << std::endl;
	std::cout << fft8_2 << std::endl;
	std::cout << fft8_3 << std::endl;
	std::cout << fft8_4 << std::endl;
	std::cout << fft8_5 << std::endl;
	std::cout << fft8_6 << std::endl;
	std::cout << fft8_7 << std::endl;

}
//WITH MY CPU : ABOUT 2 MICROSECOND
