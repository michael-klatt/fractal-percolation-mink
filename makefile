# FractalPercolationMink
# Author: Michael Klatt

RM := rm -rf

# All of the sources participating in the build are defined here
LIBS := -lgsl -lgslcblas -lboost_program_options

OBJS += \
./src/BinField.o \
./src/aux.o \
./src/init.o \
./src/minkowski.o \
./src/randomnumbers.o 

CPP_DEPS += \
./src/BinField.d \
./src/aux.d \
./src/init.d \
./src/minkowski.d \
./src/FractalPercolationMink_NN.d \
./src/FractalPercolationMink_NN_percolating_cluster.d \
./src/FractalPercolationMink_NNN.d \
./src/FractalPercolationMink_NNN_percolating_cluster.d \
./src/randomnumbers.d 

# All Target
all:    FractalPercolationMink_NN \
	FractalPercolationMink_NN_percolating_cluster \
	FractalPercolationMink_NNN \
	FractalPercolationMink_NNN_percolating_cluster

src/%.o: src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

# Tool invocations
FractalPercolationMink_NN: $(OBJS) ./src/FractalPercolationMink_NN.o
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o "FractalPercolationMink_NN" $(OBJS) ./src/FractalPercolationMink_NN.o $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
FractalPercolationMink_NN_percolating_cluster: $(OBJS) ./src/FractalPercolationMink_NN_percolating_cluster.o
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o "FractalPercolationMink_NN_percolating_cluster" $(OBJS) ./src/FractalPercolationMink_NN_percolating_cluster.o $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
FractalPercolationMink_NNN: $(OBJS) ./src/FractalPercolationMink_NNN.o
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o "FractalPercolationMink_NNN" $(OBJS) ./src/FractalPercolationMink_NNN.o $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
FractalPercolationMink_NNN_percolating_cluster: $(OBJS) ./src/FractalPercolationMink_NNN_percolating_cluster.o
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o "FractalPercolationMink_NNN_percolating_cluster" $(OBJS) ./src/FractalPercolationMink_NNN_percolating_cluster.o $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	@$(RM) $(OBJS)$(CPP_DEPS) FractalPercolationMink_NN FractalPercolationMink_NN_percolating_cluster FractalPercolationMink_NNN FractalPercolationMink_NNN_percolating_cluster ./src/FractalPercolationMink_NN.o ./src/FractalPercolationMink_NN_percolating_cluster.o ./src/FractalPercolationMink_NNN.o ./src/FractalPercolationMink_NNN_percolating_cluster.o
	-@echo 'Cleaning ...'
	-@echo ' '

