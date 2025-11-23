# Makefile para o projeto Space Invaders
# Compilador
CXX = g++

# Flags de compilação (Modo Debug, C++11, warnings)
CXXFLAGS = -g -Wall -std=c++11

# Diretórios
IDIR = include
SDIR = src
ODIR = obj
BINDIR = bin

# --- Flags de Linkagem Específicas do Sistema ---
# (Descomente a seção para o seu SO)

# Para LINUX (instalado via apt)
LDFLAGS = -lGL -lglut -lGLU

# Para WINDOWS (usando MSYS2/MinGW)
# LDFLAGS = -lGL -lglut -lGLU -lmingw32

# Para MAC OS (se necessário)
# LDFLAGS = -framework OpenGL -framework GLUT

# --- Restante do Makefile ---

# Encontra todos os arquivos .cpp em src
SOURCES = $(wildcard $(SDIR)/*.cpp)

# Gera nomes dos arquivos objeto .o em obj/
OBJECTS = $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(SOURCES))

# Nome do executável
TARGET = $(BINDIR)/space_invaders

# Regra principal
all: $(TARGET)
	@echo "Compilação concluída com sucesso!"
	@echo "Execute com: ./$(TARGET)"

# Regra para linkar o executável
$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) -o $(TARGET) $^ $(LDFLAGS)
	@echo "Build finalizado: $(TARGET)"

# Regra para compilar arquivos .cpp para .o
$(ODIR)/%.o: $(SDIR)/%.cpp
	@mkdir -p $(ODIR)
	$(CXX) $(CXXFLAGS) -I$(IDIR) -c $< -o $@

# Regra para limpar os arquivos compilados
clean:
	rm -rf $(ODIR) $(BINDIR)
	@echo "Arquivos de build removidos."

# Regra para recompilar tudo
rebuild: clean all

# Regra para executar o jogo
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run
