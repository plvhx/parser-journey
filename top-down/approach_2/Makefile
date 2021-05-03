CC = gcc
CFLAGS = -DDEBUG -g

PARSER_DIR = .
PARSER_OBJS = \
	$(PARSER_DIR)/ast.o \
	$(PARSER_DIR)/lexer.o \
	$(PARSER_DIR)/main.o \
	$(PARSER_DIR)/parser.o \
	$(PARSER_DIR)/str.o

all: $(PARSER_OBJS)
	$(CC) $(CFLAGS) -o parser $(PARSER_OBJS)

clean:
	rm -f \
		$(PARSER_DIR)/ast.o \
		$(PARSER_DIR)/lexer.o \
		$(PARSER_DIR)/main.o \
		$(PARSER_DIR)/parser.o \
		$(PARSER_DIR)/str.o \
		$(PARSER_DIR)/parser

$(PARSER_DIR)/ast.o: $(PARSER_DIR)/ast.c
	$(CC) $(CFLAGS) -c $< -o $@

$(PARSER_DIR)/lexer.o: $(PARSER_DIR)/lexer.c
	$(CC) $(CFLAGS) -c $< -o $@

$(PARSER_DIR)/main.o: $(PARSER_DIR)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(PARSER_DIR)/parser.o: $(PARSER_DIR)/parser.c
	$(CC) $(CFLAGS) -c $< -o $@

$(PARSER_DIR)/str.o: $(PARSER_DIR)/str.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean
