#Author: Koumis

#Compiler flags
CC = gcc
CFLAGS = -Wall

#File Names
MAIN   = main
UTILS  = utils
PGRANK = pagerank

SRC_DIR = src
OBJ_DIR = obj

#Final Target
pagerank: $(MAIN).o $(UTILS).o $(PGRANK).o 
	$(CC) $(CFLAGS) -o pagerank $(OBJ_DIR)/*.o

#Object Targets
$(MAIN).o: $(SRC_DIR)/$(MAIN).c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $^ -o $(OBJ_DIR)/$@

$(UTILS).o: $(SRC_DIR)/$(UTILS).c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $^ -o $(OBJ_DIR)/$@

$(PGRANK).o: $(SRC_DIR)/$(PGRANK).c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $^ -o $(OBJ_DIR)/$@

$(OBJ_DIR):
	mkdir -p $@

clean: 
	rm -rf $(OBJ_DIR) *.csv pagerank
