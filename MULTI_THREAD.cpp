#include <iostream>
#include <cmath>
#include<complex>
#include<chrono>
#include<thread>

//MULTI-THREAD IMPLEMENTATION OF FFT8

#define MAXFIFO 1024
#define MAXIO 8

const std::complex<double> W0_2(1, 0);
const std::complex<double> W0_4(1, 0);
const std::complex<double> W1_4(0, -1);
const std::complex<double> W0_8(1, 0);
const std::complex<double> W1_8((sqrt(2) / 2), -(sqrt(2) / 2));
const std::complex<double> W2_8(0, -1);
const std::complex<double> W3_8(-(sqrt(2) / 2), -(sqrt(2) / 2));

std::chrono::steady_clock::time_point start;
std::chrono::steady_clock::time_point stop;
long long processed_time;
struct fifo_t {
public:
	std::complex<double> data[MAXFIFO];
	unsigned wptr;
	unsigned rptr;
	fifo_t() {
		init_fifo();
	}
	void put_fifo(std::complex<double> dat) {
		if (((this->wptr + 1) % MAXFIFO) != this->rptr) {
			this->data[this->wptr] = dat;
			this->wptr = (this->wptr + 1) % MAXFIFO;
		}
	}
	std::complex<double> get_fifo(void) {
		std::complex<double> r;
		if (this->rptr != this->wptr) {
			r = this->data[this->rptr];
			this->rptr = (this->rptr + 1) % MAXFIFO;
			return r;
		}
		return -1;
	}
	unsigned fifo_size(void) {
		if (wptr >= rptr)
			return wptr - rptr;
		else
			return MAXFIFO - (rptr - wptr) + 1;
	}
private:
	void init_fifo(void) {
		this->wptr = this->rptr = 0;
	}
};

struct actorio_t {
public:
	fifo_t* in[MAXIO];
	fifo_t* out[MAXIO];
};

void Reorder(actorio_t* g) {
	std::complex<double> v0, v1, v2, v3, v4, v5, v6, v7;
	while (1) {
		while (g->in[0]->fifo_size() >= 8) {
			v0 = g->in[0]->get_fifo();
			v1 = g->in[0]->get_fifo();
			v2 = g->in[0]->get_fifo();
			v3 = g->in[0]->get_fifo();
			v4 = g->in[0]->get_fifo();
			v5 = g->in[0]->get_fifo();
			v6 = g->in[0]->get_fifo();
			v7 = g->in[0]->get_fifo();
			g->out[0]->put_fifo(v0);
			g->out[0]->put_fifo(v4);
			g->out[0]->put_fifo(v2);
			g->out[0]->put_fifo(v6);
			g->out[0]->put_fifo(v1);
			g->out[0]->put_fifo(v5);
			g->out[0]->put_fifo(v3);
			g->out[0]->put_fifo(v7);
		}
	}
}
void FFT2(actorio_t* g) {
	std::complex<double> a, b;
	while (1) {
		while (g->in[0]->fifo_size() >= 2) {
			a = g->in[0]->get_fifo();
			b = g->in[0]->get_fifo();
			g->out[0]->put_fifo(a + W0_2 * b);
			g->out[0]->put_fifo(a - b);
		}
	}
}
void FFT4(actorio_t* g) {
	std::complex<double> a, b, c, d;
	while (1) {
		while (g->in[0]->fifo_size() >= 4) {
			a = g->in[0]->get_fifo();
			b = g->in[0]->get_fifo();
			c = g->in[0]->get_fifo();
			d = g->in[0]->get_fifo();
			g->out[0]->put_fifo(a + W0_4 * c);
			g->out[0]->put_fifo(b + W1_4 * d);
			g->out[0]->put_fifo(-W0_4 * c + a);
			g->out[0]->put_fifo(-W1_4 * d + b);
		}
	}
}

void FFT8(actorio_t* g) {
	std::complex<double> a, b, c, d, e, f, l, h;
	static int count = 0;
	while (1) {
		while (g->in[0]->fifo_size() >= 8) {
			a = g->in[0]->get_fifo();
			b = g->in[0]->get_fifo();
			c = g->in[0]->get_fifo();
			d = g->in[0]->get_fifo();
			e = g->in[0]->get_fifo();
			f = g->in[0]->get_fifo();
			l = g->in[0]->get_fifo();
			h = g->in[0]->get_fifo();
			g->out[0]->put_fifo(a + W0_8 * e);
			g->out[0]->put_fifo(b + W1_8 * f);
			g->out[0]->put_fifo(c + W2_8 * l);
			g->out[0]->put_fifo(d + W3_8 * h);
			g->out[0]->put_fifo(-W0_8 * e + a);
			g->out[0]->put_fifo(-W1_8 * f + b);
			g->out[0]->put_fifo(-W2_8 * l + c);
			g->out[0]->put_fifo(-W3_8 * h + d);
			count++;
			if (count >= 100) {
				stop = std::chrono::high_resolution_clock::now();
				processed_time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()/100;
				std::cout << " Processing Time MULTITHREAD : " << processed_time << " (microsecond)\n";
				exit(0);
			}
		}
	}
}

void Data_Creator(fifo_t* q1) {
	static int i = 0;
	static std::complex<double> in0(100, 0), in1(170, 0), in2(200, 0), in3(170, 0), in4(100, 0), in5(29, 0), in6(0, 0), in7(29, 0);
	start = std::chrono::high_resolution_clock::now();
	while (1) {
		if (i >100)
			break;
		q1->put_fifo(in0);
		q1->put_fifo(in1);
		q1->put_fifo(in2);
		q1->put_fifo(in3);
		q1->put_fifo(in4);
		q1->put_fifo(in5);
		q1->put_fifo(in6);
		q1->put_fifo(in7);
		i++;
	}

}
int main()
{
	fifo_t q1, q2, q3, q4, q5;

	actorio_t reorder_io = { {&q1},{&q2} };
	actorio_t fft2_io = { {&q2},{&q3} };
	actorio_t fft4_io = { {&q3},{&q4} };
	actorio_t fft8_io = { {&q4},{&q5} };

	std::thread t1(Reorder, &reorder_io);
	std::thread t2(FFT2, &fft2_io);
	std::thread t3(FFT4, &fft4_io);
	std::thread t4(FFT8, &fft8_io);
	std::thread t5(Data_Creator, &q1);

	//C++ HAS AUTOMATIC SCHEUDLER . RANDOM JOINING .
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();

	

}
//WITH MY CPU ABOUT 4 MIRCOSECOND