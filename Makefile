rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
src := $(call rwildcard,./,*.cpp)

obj = $(patsubst %.cpp,%.o,$(src))

LDFLAGS = -lSDL2
LDFLAGS += -lzmq  

INTELMAC_INCLUDEDIR=/usr/local/include			# Intel mac
APPLESILICON_INCLUDEDIR=/opt/homebrew/include	# Apple Silicon
UBUNTU_APPLESILICON_INCLUDEDIR=/usr/include		# Apple Silicon Ubuntu VM
UBUNTU_INTEL_INCLUDEDIR=/usr/include			# Intel Ubuntu VM

REPO_INCLUDEDIR=include/

INTELMAC_LIBPATH=/usr/local/lib 							# Intel mac
APPLESILICON_LIBPATH=/opt/homebrew/lib						# Apple Silicon
UBUNTU_APPLESILICON_LIBPATH=/usr/lib/aarch64-linux-gnu		# Apple Silicon Ubuntu VM
UBUNTU_INTEL_LIBPATH=/usr/lib/x86_64-linux-gnu				# Intel Ubuntu VM

MACOS_INCLUDE=$(APPLESILICON_INCLUDEDIR)
MACOS_LIB=$(APPLESILICON_LIBPATH)
UBUNTU_INCLUDE=$(UBUNTU_APPLESILICON_INCLUDEDIR)
UBUNTU_LIB=$(UBUNTU_APPLESILICON_LIBPATH)

MACOS_COMPILER=/usr/bin/clang++
UBUNTU_COMPILER=/usr/bin/g++

all: main

uname_s := $(shell uname -s)
main: $(obj)
ifeq ($(uname_s),Darwin)
	$(MACOS_COMPILER) -o $@ $^ $(LDFLAGS) -L$(MACOS_LIB)
else ifeq ($(uname_s),Linux)
	$(UBUNTU_COMPILER) -o $@ $^ $(LDFLAGS) -L$(UBUNTU_LIB)
endif

uname_s := $(shell uname -s)
%.o: %.cpp
ifeq ($(uname_s),Darwin)
	$(MACOS_COMPILER) -c $^ -o $@ -I$(MACOS_INCLUDE) -I$(REPO_INCLUDEDIR)
else ifeq ($(uname_s),Linux)
	$(UBUNTU_COMPILER) -c $^ -o $@ -I$(UBUNTU_INCLUDE) -I$(REPO_INCLUDEDIR)
endif

.PHONY: clean
clean:
	rm -f $(obj) main