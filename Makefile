########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - can be customized
CC = g++
CXXFLAGS = -std=c++11 -Wall -Isrc/common
LDFLAGS =

# Base source and object directories
SRCDIR = src
OBJDIR = obj

# Detect OS and set platform-specific variables
ifeq ($(OS),Windows_NT)
    LDLIBS = -lwinmm
    EXE = .exe
    DEL = del
    RM =
    PLATFORM_SRCDIR = $(SRCDIR)/win32
    PLATFORM_OBJDIR = $(OBJDIR)/win32
else
    LDLIBS = -lX11
    EXE =
    DEL =
    RM = rm
    PLATFORM_SRCDIR = $(SRCDIR)/linux
    PLATFORM_OBJDIR = $(OBJDIR)/linux
endif

# Common source files (e.g., main.cpp, windowcheck.h etc)
COMMON_SRCDIR = $(SRCDIR)/common
COMMON_OBJDIR = $(OBJDIR)/common
COMMON_SRCS = $(wildcard $(COMMON_SRCDIR)/*.cpp)

# Platform specific source files
PLATFORM_SRCS = $(wildcard $(PLATFORM_SRCDIR)/*.cpp)

# All source files
SRC = $(COMMON_SRCS) $(PLATFORM_SRCS)

# Object files corresponding to sources, separate folders for common and platform
OBJ_COMMON = $(patsubst $(COMMON_SRCDIR)/%.cpp,$(COMMON_OBJDIR)/%.o,$(COMMON_SRCS))
OBJ_PLATFORM = $(patsubst $(PLATFORM_SRCDIR)/%.cpp,$(PLATFORM_OBJDIR)/%.o,$(PLATFORM_SRCS))
OBJ = $(OBJ_COMMON) $(OBJ_PLATFORM)

# Dependency files
DEP_COMMON = $(OBJ_COMMON:.o=.d)
DEP_PLATFORM = $(OBJ_PLATFORM:.o=.d)
DEP = $(DEP_COMMON) $(DEP_PLATFORM)

########################################################################
####################### Targets beginning here #########################
########################################################################

APPNAME = OnTrackinator

# Default target
all: $(APPNAME)$(EXE)

# Link executable
$(APPNAME)$(EXE): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# Compile common source files
$(COMMON_OBJDIR)/%.o: $(COMMON_SRCDIR)/%.cpp
ifeq ($(OS),Windows_NT)
	@if not exist "$(COMMON_OBJDIR)" mkdir "$(COMMON_OBJDIR)"
else
	@mkdir -p "$(COMMON_OBJDIR)"
endif
	$(CC) $(CXXFLAGS) -c $< -o $@

# Compile platform-specific source files
$(PLATFORM_OBJDIR)/%.o: $(PLATFORM_SRCDIR)/%.cpp
ifeq ($(OS),Windows_NT)
	@if not exist "$(PLATFORM_OBJDIR)" mkdir "$(PLATFORM_OBJDIR)"
else
	@mkdir -p "$(PLATFORM_OBJDIR)"
endif
	$(CC) $(CXXFLAGS) -c $< -o $@


# Generate dependency files for all sources
%.d: %.cpp
	@$(CC) $(CXXFLAGS) -MM $< -MT $(@:.d=.o) > $@

# Include dependency files if present
-include $(DEP)

################### Cleaning rules ###################
.PHONY: clean
clean:
ifeq ($(OS),Windows_NT)
	-for %%F in ($(OBJ) $(DEP) $(APPNAME)$(EXE)) do del /Q "%%F"
else
	$(RM) -f $(OBJ) $(DEP) $(APPNAME)
endif

.PHONY: cleandep
cleandep:
ifeq ($(OS),Windows_NT)
	-for %%F in ($(DEP)) do del /Q "%%F"
else
	$(RM) -f $(DEP)
endif
