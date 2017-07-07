#include <iostream>
#include <streambuf>

/*
Makes an ostream that writes to cerr. Experiment is half success.
Actual output:
Output> Hello world!
Output> a
Output> 1
Output> 5
Output> Byebyebye

Desired output:
Output> Hello world!
Output> a
Output> 15
Output> Byebyebye
*/

class CerrOutputter : public std::streambuf {
	// override
	std::streamsize xsputn(const char_type* s, std::streamsize n) {
		std::cerr << "Output> " << s << std::endl;
		return n;
	}
	// override
	int_type overflow(int_type _Meta) {
		std::cerr << "Output> " << (char)_Meta << std::endl;
		return _Meta;
	}
} cerr_outputter;

std::ostream cerr_output(&cerr_outputter);

int main(void) {
	cerr_output << "Hello world!";
	cerr_output << 'a' << 15 << "Byebyebye";
}
