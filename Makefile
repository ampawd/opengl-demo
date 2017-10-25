#PROGNAME	= openglDemo
#
#CXX			= g++
#
## SRC			= main.cpp \
## 		model.cpp \
## 		model_mesh.cpp \
## 		shader.cpp \
## 		shader_manager.cpp \
## 		camera.cpp
#
#
#OBJS		= main.o \
#		model.o \
#		model_mesh.o \
#		shader.o \
#		shader_manager.o \
#		camera.o
#
#
#BUILDIR 	= build
#
## use this for non standard (other than from /usr/include) include directories
## -I/some/path -I/some/path2
## INCDIR		= 
#
## use this for non standard (other than from /usr/lib) library directories 
## -L/some/path -L/some/path2
## LIBDIR		= 
#
#CXXFLAGS	= -Wall -c -std=c++11
#
#LDFLAGS		= -Wall
#
#LIBS		= -lGL -lGLEW -lglfw -lassimp -lSOIL
#
#
#
#
#$(BUILDIR)/$(PROGNAME): $(OBJS)
#	$(CXX) $(BUILDIR)/*.o $(LDFLAGS) $(LIBS) -o $(BUILDIR)/$(PROGNAME) 
#
#
#main.o: main.cpp
#	$(CXX) $(CXXFLAGS) $(INCDIR) main.cpp -o $(BUILDIR)/main.o
#
#model.o: model.cpp
#	$(CXX) $(CXXFLAGS) $(INCDIR) model.cpp -o $(BUILDIR)/model.o
#
#model_mesh.o: model_mesh.cpp
#	$(CXX) $(CXXFLAGS) $(INCDIR) model_mesh.cpp -o $(BUILDIR)/model_mesh.o
#
#shader.o: shader.cpp
#	$(CXX) $(CXXFLAGS) $(INCDIR) shader.cpp -o $(BUILDIR)/shader.o
#
#shader_manager.o: shader_manager.cpp
#	$(CXX) $(CXXFLAGS) $(INCDIR) shader_manager.cpp -o $(BUILDIR)/shader_manager.o
#
#camera.o: camera.cpp
#	$(CXX) $(CXXFLAGS) $(INCDIR) camera.cpp -o $(BUILDIR)/camera.o
#
#.PHONY: clean
#
#clean:
#	rm  $(BUILDIR)/*.o $(BUILDIR)/$(PROGNAME)
#


PROGNAME = openglDemo

CXX	= g++


BUILDIR = build

SRC	:= $(wildcard *.cpp)

OBJS := $(addprefix $(BUILDIR)/, $(patsubst %.cpp, %.o, $(SRC))) 



CXXFLAGS := -Wall -c -std=c++11

LDFLAGS	:= -Wall

LIBS := -lGL -lGLEW -lglfw -lassimp -lSOIL


$(BUILDIR)/$(PROGNAME): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

$(BUILDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<
	
.PHONY: clean

clean:
	rm  $(BUILDIR)/*.o $(BUILDIR)/$(PROGNAME)

