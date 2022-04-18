#include <iostream>

#define VERSION "0.00"
void print_usage() {
	std::cout << "Welcome to OmegaScan! VERSION: " << VERSION << std::endl;
	std::cout << "Usage: omegascan [Scan Type(s)] [Target]" << std::endl;
	std::cout << "SCAN TECHNIQUES:" << std::endl;
	std::cout << "-sS/sT/sA/sW/sM: TCP SYN/Connect()/ACK/Window/Maimon scans" << std::endl;
	std::cout << "-sN/sF/sX: TCP Null, FIN, and Xmas scans" << std::endl;
	std::cout << "-sU: UDP Scan" << std::endl;
}

int main(int argc, char *argv[])
{
	print_usage();
	return 0;
}
