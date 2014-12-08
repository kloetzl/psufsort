#include <string>
#include <vector>
#include <utility>
#include <divsufsort.h>
#include <iostream>

#include "flags.h"

std::vector<int> psufsort(std::string);


void saca_wrapper(std::string str){
	auto SA = psufsort(str);

	if( FLAGS & F_VERBOSE){
		for (auto i = SA.begin(); i != SA.end(); ++i)
		{
			std::cout << *i << " " << str.data()+*i << std::endl;
		}
	}

	sufcheck((const unsigned char*)str.data(), SA.data(), str.size()+1, 1);
}
