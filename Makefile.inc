UCI_DIR?=./uci
INCLUDES += -I$(UCI_DIR)/include

UCI_OBJS:= \
	objs/uci-util.o \
	objs/uci-option.o \
	objs/uci-section.o \
	objs/uci-category.o \
	objs/uci-package.o \
	objs/uci-parse.o \

objs/uci-util.o: $(UCI_DIR)/src/util.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

objs/uci-option.o: $(UCI_DIR)/src/option.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

objs/uci-section.o: $(UCI_DIR)/src/section.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

objs/uci-category.o: $(UCI_DIR)/src/category.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

objs/uci-package.o: $(UCI_DIR)/src/package.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

objs/uci-parse.o: $(UCI_DIR)/src/parse.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;
