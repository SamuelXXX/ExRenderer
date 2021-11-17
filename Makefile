# debug or release
ver=debug

# decide compiling parameters by target platform
ifeq ($(ver),debug)
CXXFLAGS = -c -g -Ddebug
else
CXXFLAGS = -c -O3
endif

# building path settings
EXRD_SRC_PATH = src/
TEST_SRC_PATH = testbench/

OBJ_PATH=.object\$(ver)
EXRD_OBJ_PATH=$(OBJ_PATH)\ExRenderer
TEST_OBJ_PATH=$(OBJ_PATH)\Test
TARGET_PATH = .build\$(ver)
LIB_PATH = $(TARGET_PATH)\libs

# target .exe .o .lib files
EXE_NAME = EXRDTestbench
EXE_FILE = $(TARGET_PATH)/$(EXE_NAME).exe

EXRD_OBJ_FILES:=$(patsubst $(EXRD_SRC_PATH)/%.cpp, $(EXRD_OBJ_PATH)/%.o, $(wildcard $(EXRD_SRC_PATH)/*.cpp))
EXRD_HEADER_FILES:=$(wildcard $(EXRD_SRC_PATH)/*.h)
TEST_OBJ_FILES:=$(patsubst $(TEST_SRC_PATH)/%.cpp, $(TEST_OBJ_PATH)/%.o, $(wildcard $(TEST_SRC_PATH)/*.cpp))
TEST_HEADER_FILES:=$(wildcard $(TEST_SRC_PATH)/*.h)


OBJ_FILES:=$(EXRD_OBJ_FILES) $(TEST_OBJ_FILES)

SDL_LIB_PATH:=libs/lib
SDL_INC_PATH:=libs/include
SDL_DLL_FILE:=$(TARGET_PATH)\SDL2.dll
SDL_DLL_FROM_FILE:=libs\bin\SDL2.dll


# default task, check directory and build .exe and .lib files
all:$(EXE_FILE) $(SDL_DLL_FILE)

$(EXE_FILE) : $(OBJ_FILES)
	@echo Linking Executable : $@
	@mkdir $(@D)||:
	@g++ $^ -o $@ -L$(SDL_LIB_PATH) -lmingw32 -lSDL2main -lSDL2 

$(SDL_DLL_FILE):$(SDL_DLL_FROM_FILE)
	@echo Copy SDL dll files
	@copy $(SDL_DLL_FROM_FILE) $(SDL_DLL_FILE)

$(EXRD_OBJ_PATH)/%.o : $(EXRD_SRC_PATH)/%.cpp $(EXRD_HEADER_FILES)
	@echo Building EXRD Object : $@
	@mkdir $(@D)||:
	@g++ $< $(CXXFLAGS) -o $@ -I$(SDL_INC_PATH)

$(TEST_OBJ_PATH)/%.o : $(TEST_SRC_PATH)/%.cpp $(EXRD_HEADER_FILES) $(TEST_HEADER_FILES)
	@echo Building Test Object : $@
	@mkdir $(@D)||:
	@g++ $< $(CXXFLAGS) -o $@ -I$(SDL_INC_PATH)


.PHONY : clean chkdir cleanexe cleanobj
clean : cleanlib cleanexe cleanobj
cleanexe:
	@del $(TARGET_PATH)\*.exe
cleanlib:
	@del $(LIB_PATH)\*.lib
cleanobj:
	@for /r $(OBJ_PATH) %%i in (*.o) do del /q %%i
	

