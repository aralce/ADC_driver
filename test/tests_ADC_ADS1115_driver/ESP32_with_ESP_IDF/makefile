name_of_test = "ADC_driver -> ADS1115_driver -> ESP32_with_ESP-IDF"

TEST_MAIN_FOLDER_DIR = ./
MAIN_TEST_FOLDER_DIR = ../../
MAIN_PROGRAM_DIR = $(MAIN_TEST_FOLDER_DIR)../
UNDER_TEST_PROGRAM_DIR = $(MAIN_PROGRAM_DIR)Implementation/ADC_ADS1115_driver/ESP32_ESP_IDF/
ADC_LIBRARY_DIR = $(MAIN_TEST_FOLDER_DIR)mocks/ADC_library/ADC_firmware/ADC_ADS1115_driver/ESP32_with_Arduino/

CPPUTEST_HOME = ../../cpputest/

COMPILER_INCLUDE_FLAGS  = -I$(CPPUTEST_HOME)include
COMPILER_INCLUDE_FLAGS += -I$(ADC_LIBRARY_DIR)
COMPILER_INCLUDE_FLAGS += -I$(MAIN_PROGRAM_DIR)
COMPILER_INCLUDE_FLAGS += -I$(MAIN_TEST_FOLDER_DIR)

FLAG_FOR_DEFINE = -D IS_RUNNING_TESTS

CXX = g++
CXXFLAGS  =  -Wall $(COMPILER_INCLUDE_FLAGS) $(FLAG_FOR_DEFINE)

##UNCOMMENT TO TEST MEMORY LEAK
#CXXFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorNewMacros.h

LD_LIBRARIES  = -L$(CPPUTEST_HOME)lib -lCppUTest -lCppUTestExt

BUILD_OUTPUT_DIR = build/

all: create_build_folder link_objects_of_tests
	@echo $(name_of_test)
	@$(BUILD_OUTPUT_DIR)tests.out

clean:
	@rm -f -r $(BUILD_OUTPUT_DIR)

create_build_folder:
	@mkdir -p $(BUILD_OUTPUT_DIR)

OBJECT_FILES  = $(patsubst %.cpp, %.o, $(notdir $(wildcard $(TEST_MAIN_FOLDER_DIR)*.cpp)))
OBJECT_FILES += $(patsubst %.cpp, %.o, $(notdir $(wildcard $(UNDER_TEST_PROGRAM_DIR)*.cpp)))
OBJECT_FILES += $(patsubst %.cpp, %.o, $(notdir $(wildcard $(ARDUINO_MOCK_DIR)*.cpp)))
OBJECT_FILES += $(patsubst %.cpp, %.o, $(notdir $(wildcard $(ADC_LIBRARY_DIR)*.cpp)))
OBJECT_FILES_ON_DIR = $(addprefix $(BUILD_OUTPUT_DIR),$(OBJECT_FILES))

link_objects_of_tests: $(OBJECT_FILES_ON_DIR)
	@$(CXX) $(CXXFLAGS) $^ $(LD_LIBRARIES) -o $(BUILD_OUTPUT_DIR)tests.out


.INTERMEDIATE: $(OBJECT_FILES_ON_DIR)

vpath %.cpp $(TEST_MAIN_FOLDER_DIR)
vpath %.cpp $(UNDER_TEST_PROGRAM_DIR)
vpath %.cpp %.h $(ADC_LIBRARY_DIR)

$(BUILD_OUTPUT_DIR)%.o : %.cpp
	@$(CXX) $(CXXFLAGS) $< $(LD_LIBRARIES) -c -o $@

$(BUILD_OUTPUT_DIR)%.o : %.cpp %.h
	@$(CXX) $(CXXFLAGS) $< $(LD_LIBRARIES) -c -o $@

print:
	@echo $(OBJECT_FILES_ON_DIR)