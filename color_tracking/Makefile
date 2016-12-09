INCLUDE = $(shell pkg-config --cflags opencv)
LIBS = $(shell pkg-config --libs opencv) -lpthread
SOURCES = color_tracking.cpp
# 
OBJECTS = $(SOURCES:.cpp=.o)
# 
TARGET = color_tracking
$(TARGET):$(OBJECTS)
		g++ -o $(TARGET) $(OBJECTS) -I $(INCLUDE) $(LIBS)
$(OBJECTS):$(SOURCES)
		g++ -c $(SOURCES)
clean:
		$(OBJECTS) $(TARGET)
    # 
%.o:%.cpp
		g++ -I $(INCLUDE) -o $[url=home.php?mod=space&uid=72445]@[/url] -c [        DISCUZ_CODE_272        ]lt;
