int main(int argc, char** argv){
	int x = 1;
	int y = 2;
	__atomic_compare_exchange_n(&x, &x, y, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return x;
}
