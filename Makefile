
CPPFLAGS += -std=c++1y -g -O0
LIBS += -ldivsufsort

psufsort: common/driver.cxx common/psufsort_wrapper.cxx src/psufsort.cxx
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	-rm -f psufsort
