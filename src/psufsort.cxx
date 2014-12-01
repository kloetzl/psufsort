
#include <string>
#include <vector>
#include <utility>
#include <cstring>
#include <algorithm>

void TSQS (std::vector<int>& SA, const std::string& T, size_t l, size_t r, size_t depth);

std::vector<int> psufsort(std::string T){
	auto n = T.size();
	auto SA = std::vector<int>(n+1);

	for(auto i=0; i<n+1;i++){
		SA[i] = i;
	}

	TSQS(SA, T, 0, n, 0);

	return SA;
}

/*std::vector<int> psufsort(std::string T){
	auto n = T.size();
	auto SA = std::vector<int>(n+1);
	auto suff = [&](size_t i){
		return T.data() + i;
	};

	auto bucket_A = std::vector<std::pair<int,int>>(256,std::make_pair(0,0));
	auto bucket_B = std::vector<std::pair<int,int>>(256,std::make_pair(0,0));
	//auto bucket_S = std::vector<std::pair<int,int>>(256,std::make_pair(0,0)); // B*

	size_t i;
	for(i=0; i<n; i++){ // TODO: n-1?
		auto c = T[i];
		auto d = T[i+1];
		if( c > d){
			bucket_A[c].first++;
		} else {
			bucket_B[c].first++;
		}
	}
	bucket_B[0].first = 1;

	int c = 0;
	for(i=0; i<256; i++){
		bucket_A[i].second = c;
		c += bucket_A[i].first + bucket_B[i].first;
		bucket_B[i].second = c;
	}

	for( i=0; i<n; i++){
		char c = T[i];
		if( c <= T[i+1]){
			bucket_B[c].second--;
			SA[bucket_B[c].second] = i;
		}
	}

	SA[0] = n;

	for(i=0; i<256; i++){
		if( bucket_B[i].first > 1){
			int b = bucket_B[i].second;
			int e = b + bucket_B[i].first;
			// sort
			auto cmp = [&](int a, int b){
				auto ta = suff(a);
				auto tb = suff(b);

				auto res = strcmp(ta, tb) < 0;
				return res;
			};

			std::sort(SA.begin()+b, SA.begin()+e, cmp);
		}
	}

	for(i=0; i<n; i++){
		auto a = T[SA[i]-1];
		if( a > T[SA[i]]){
			SA[bucket_A[a].second] = SA[i] -1;
			bucket_A[a].second++;
		}
	}

	return std::move(SA); // move doesnt move
}*/

void swap_range(int *A, int *B, size_t n){
	for(auto i=0; i< n; i++){
		std::swap(A[i], B[i]);
	}
}

void TSQS (std::vector<int>& SA, const std::string& T, size_t l, size_t r, size_t depth){
	auto key = [&](size_t i){
		return (T.data() + SA[i])[depth];
	};

	auto K = key(l); // pick K

	auto a = l;
	auto b = l;
	auto c = r;
	auto d = r;

	do {
		for(; b <= c && key(b) <= K; b++){
			if( key(b) == K){
				std::swap(SA[a], SA[b]);
				a++;
			}
		}

		for(; b <= c && key(c) >= K; c--){
			if( key(c) == K){
				std::swap(SA[c],SA[d]);
				d--;
			}
		}

		if( b < c){
			std::swap(SA[b],SA[c]);
			b++, c--;
			continue;
		}
	} while(b < c);

	auto m = std::min(a-l, b-a);
	swap_range(SA.data()+l, SA.data()+c-m+1, m);

	m = std::min(d-c, r-d);
	swap_range(SA.data()+b, SA.data()+r-m+1, m);
}
