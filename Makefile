#--------------------------------------------------------------------
#												        Configuration
#--------------------------------------------------------------------
GCC=gcc
GXX=g++
CFLAGS= -g #-O3 -fopenmp
LDFLAGS=
BIN_DIR=./bin
INC_DIR=./include
OBJ_DIR=./obj
SRC_DIR=./src
LIB_DIR=./lib
BIN=$(BIN_DIR)/cmd
SRC=$(wildcard ${SRC_DIR}/*.cpp)
OBJ=$(addprefix $(OBJ_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(SRC))))
INCLUDE=-I$(INC_DIR) `pkg-config opencv --cflags`
DEP=$(addprefix $(BIN_DIR)/, $(patsubst %.cpp, %.d, $(notdir $(SRC))))
DLL_FILES=
BIN_CFLAGS=$(CFLAGS) $(INCLUDE) 
BIN_LDFLAGS=$(LDFLAGS) -L $(LIB) -l$(DLL_FILES) -fopenmp `pkg-config opencv --libs`
#--------------------------------------------------------------------
#           													Build
#--------------------------------------------------------------------
.PHONY : all build-bin make-dir
all:	make-dir build-bin

#generate  directories
make-dir:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR) $(OBJ_DIR)


#----------------------------------------------------------------------
#    											   Include dependencies
#----------------------------------------------------------------------
#ifeq ($(filter $(no_dep_clean), $(MAKECMDGOALS)),)
sinclude $(DEP)
#endif


build-bin: $(BIN)


$(BIN) : $(OBJ)
	$(GXX) $(BIN_LDFLAGS) $^ -o $@
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(GXX)  $(BIN_CFLAGS) -MM -MT '$(BIN_DIR)/$*.d $(OBJ_DIR)/$*.o' "$<" \
	-MF  "$(BIN_DIR)/$*.d"
	$(COMPILE.cpp) $(BIN_CFLAGS) $< -o $@

#----------------------------------------------------------------------
#    											   Cleaning
#---------------------------------------------------------------------- 

clean: bin-clean obj-clean dep-clean  
bin-clean:
	rm -f $(BIN)
obj-clean:
	rm -f $(OBJ)
dep-clean:
	rm -f $(DEP)

no_dep_targets += clean bin-clean obj-clean dep-clean

