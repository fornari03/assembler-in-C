CXX = g++
CXXFLAGS = -Iheaders -Wall

SRC_DIR = src
HEADER_DIR = headers

SOURCES_MONTADOR = $(SRC_DIR)/montador.cpp $(SRC_DIR)/pre_process.cpp $(SRC_DIR)/diretivas.cpp $(SRC_DIR)/files_handler.cpp $(SRC_DIR)/utils.cpp
SOURCES_LIGADOR = $(SRC_DIR)/ligador.cpp $(SRC_DIR)/files_handler.cpp $(SRC_DIR)/utils.cpp

OUTPUT_MONTADOR = montador
OUTPUT_LIGADOR = ligador

all: $(OUTPUT_MONTADOR) $(OUTPUT_LIGADOR)

$(OUTPUT_MONTADOR): $(SOURCES_MONTADOR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OUTPUT_LIGADOR): $(SOURCES_LIGADOR)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OUTPUT_MONTADOR) $(OUTPUT_LIGADOR)
