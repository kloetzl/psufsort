#include <string>
#include <vector>
#include <utility>
#include <divsufsort.h>
#include <iostream>

#include "flags.h"

std::vector<int> psufsort(std::string);

using std::string;

void str_replace( string &s, const string &search, const string &replace ) {
	for( size_t pos = 0; ; pos += replace.length() ) {
		pos = s.find( search, pos );
		if( pos == string::npos ) break;

		s.erase( pos, search.length() );
		s.insert( pos, replace );
	}
}


void saca_wrapper(std::string str){
	auto SA = psufsort(str);

	if( FLAGS & F_VERBOSE){
		std::cout << "i T[SA[i]]" << std::endl;
		for (auto i = SA.begin(); i != SA.end(); ++i){
			auto suff = std::string(str.data()+*i);
			str_replace(suff, "\n", "\\n");

			std::cout << *i << " " << suff << std::endl;
		}
	}

	sufcheck((const unsigned char*)str.data(), SA.data(), str.size()+1, 1);
}
