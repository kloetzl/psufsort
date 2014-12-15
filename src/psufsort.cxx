
#include <string>
#include <vector>
#include <utility>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <cassert>

void mk_sort (std::vector<int>& SA, const std::string& T, size_t l, size_t r, size_t depth);
void insertion_sort (std::vector<int>& SA, const std::string& T, size_t l, size_t r, size_t depth);
void TSQS (std::vector<int>& SA, const std::string& T, size_t l, size_t r, size_t depth);
void mk_buildin (std::vector<int>& SA, const std::string& T, size_t l, size_t r, size_t depth);

class PSufSort
{
	const std::string& T;
	std::vector<int>& SA;
public:
	PSufSort(const std::string& _T, std::vector<int>& _SA) : T(_T), SA(_SA) {
		auto n = T.size();
	}
	~PSufSort() {};

	void sort(size_t l, size_t r, size_t depth, size_t calls);
	void sort_tsqs(size_t l, size_t r, size_t depth, size_t calls);
	void sort_insert(size_t l, size_t r, size_t depth, size_t calls);
	void sort_heap(size_t l, size_t	r, size_t depth, size_t calls);

	void build_heap( int* rSA, size_t n);
	void heapify( int* rSA, size_t heap_size, size_t i);



	void swap_range(size_t a, size_t b, size_t n);
};

std::vector<int> psufsort(std::string T){
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

	auto sorter = PSufSort(T,SA);

	for(i=0; i<256; i++){
		if( bucket_B[i].first > 1){
			int b = bucket_B[i].second;
			int e = b + bucket_B[i].first;

			// sort
			sorter.sort( b, e, 1, 0);
		}
	}

	for(i=0; i<n; i++){
		auto j = SA[i];
		if( j == 0) continue;

		auto a = T[j-1];
		if( a > T[j]){
			SA[bucket_A[a].second] = j -1;
			bucket_A[a].second++;
		}
	}

	return std::move(SA); // move doesnt move
}

void PSufSort::swap_range(size_t a, size_t b, size_t n){
	for(auto i=0; i< n; i++){
		std::swap(this->SA[a++], this->SA[b++]);
	}
}

void PSufSort::sort (size_t l, size_t r, size_t depth, size_t calls) {
	if(l >= r){
		return;
	}

	auto m = r - l;
	if( m < 2 ){
		return;
	}

	if (m <= 4){
		sort_insert(l, r, depth, calls);
		return;
	}

	this->sort_heap(l, r, depth, calls);
}

class sufcmp {
	const std::vector<int>& SA;
	const std::string& T;
public:
	sufcmp(const std::vector<int>& _SA, const std::string& _T) : SA(_SA), T(_T) {};
	~sufcmp() {};

	int cmp_from( size_t a, size_t b, size_t depth){
		auto ta = T.data()+ a +depth;
		auto tb = T.data()+ b +depth;

		return strcmp(ta,tb);
	};

	class Suffix {
		size_t offset;
	public:
		Suffix(int arguments);
		~Suffix();

		/* data */
	};
};



void mk_buildin (std::vector<int>& SA, const std::string& T, size_t l, size_t r, size_t depth){
	auto sc = sufcmp(SA,T);
	auto cmp = [&](int a, int b){
		return sc.cmp_from(a,b,depth) < 0;
	};

	std::sort(SA.begin()+l, SA.begin()+r, cmp);
}

void PSufSort::sort_insert (size_t l, size_t r, size_t depth, size_t unused){
	auto sc = sufcmp(SA,T);
	auto key = [&](size_t i){
		return this->T.data() + SA[i] + depth;
	};

	for(auto j = l+1; j < r; j++){
		auto X = SA[j];

		auto i = j;
		for(; i > l && sc.cmp_from( SA[i-1], X, depth) > 0 ; i--){
			SA[i] = SA[i-1];
		}

		SA[i] = X;
	}
}

void PSufSort::sort_tsqs (size_t l, size_t r, size_t depth, size_t calls){
	auto key = [&](size_t i){
		return (this->T.data() + SA[i])[depth];
	};
	auto SA = this->SA;

	auto K = key(l); // pick K

	auto a = l;
	auto b = l;
	auto c = r-1;
	auto d = r-1;

	while(true) {
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

		if( b > c) break;

		std::swap(SA[b],SA[c]);
		b++, c--;
	}

	auto m = std::min(a-l, b-a);
	swap_range(l, b-m, m);

	m = std::min(d-c, r-d-1);
	swap_range(b, r-m, m);

	auto i = l + b - a;
	auto j = r - d + c;

	this->sort(l, i, depth, calls + 1);
	this->sort(i, j, depth+1, calls + 1);
	this->sort(j, r, depth, calls + 1);
}

size_t LEFT(size_t i){
	return (i << 1) + 1;
}

size_t RIGHT(size_t i){
	return (i << 1) + 2;
}

size_t PARENT( size_t i){
	return (i-1) >> 1;
}

void PSufSort::build_heap( int* rSA, size_t n){
	auto heap_size = n;
	for( ssize_t i= PARENT(n-1); i>=0 ; i--){
		heapify(rSA, heap_size, i);
	}
}

void PSufSort::heapify( int* rSA, size_t heap_size, size_t i){ // aka. siftDown
	auto key = [&](size_t j){
		return T.data() + j + 1; //FIXME: depth
	};

	auto l = LEFT(i);
	auto r = RIGHT(i);
	auto largest = i;

	if( l < heap_size && key(rSA[l]) > key(rSA[i])){
		largest = l;
	}
	if( r < heap_size && key(rSA[r]) > key(rSA[largest])){
		largest = r;
	}
	if( largest != i){
		std::swap(rSA[i], rSA[largest]);
		heapify(rSA, heap_size, largest);
	}
}

void PSufSort::sort_heap(size_t left, size_t right, size_t depth, size_t calls){
	auto key = [&](size_t j){
		return T.data() + j + 1; //FIXME: depth
	};

	auto rSA = SA.data() + left;
	auto n = right - left;
	auto heap_size = n;
	
	build_heap(rSA, n);

	for( auto i = n-1; i ; i--){
		std::swap(rSA[0],rSA[i]);
		assert(key(rSA[i]) > key(rSA[0]));
		heap_size--;
		heapify(rSA, heap_size, 0);
	}
}

