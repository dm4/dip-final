main:
	g++ `pkg-config --cflags --libs opencv` -D__STDC_LIMIT_MACROS *.cpp -o final && ./final
