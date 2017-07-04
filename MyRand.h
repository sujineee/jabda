
#ifndef MYRAND_H_
#define MYRAND_H_

double MyRand()
{
	const long a = 16807;
	const long m = 2147483647;
	const long q = 127773;
	const long r = 2836;
	static long x = 1;
	long x_div_q;
	long x_mod_q;
	long t;

	x_div_q = x / q;
	x_mod_q = x % q;

	t = (a * x_mod_q) - (r * x_div_q);

	if(t > 0)
		x = t;
	else
		x = t + m;
	return ((double)x/m);
}


#endif /* MYRAND_H_ */
