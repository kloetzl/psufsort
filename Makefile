
CPPFLAGS += -std=c++1y -g -ggdb -O0 -fopenmp -Wall -W -Icommon
LIBS += -ldivsufsort -fopenmp

psufsort: common/driver.cxx common/psufsort_wrapper.cxx src/psufsort.cxx
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	-rm -f psufsort
