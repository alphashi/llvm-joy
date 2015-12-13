

SOURCE_DIR = ./ 
SRCS = lexer.cpp ast.cpp parser.cpp main.cpp

#CXX = g++ -c -g 
#CXX = g++ 
CXX = g++ -g 



#OBJS = main.o parser.o lexer.o ast.o 
#OBJS = $(SOURCE_DIR:%.cpp=%.o)
#OBJS = $(SRCS:%.cpp=%.o)
OBJS = $(SRCS:%.cpp=%.o)

#@echo$(OBJS)

OUT = main


$(OUT) : $(OBJS)
	$(CXX) $(OBJS) -o $@



clean :
	@$(RM) -f main $(OBJS)
