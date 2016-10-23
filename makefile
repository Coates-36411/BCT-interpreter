TARGET = bct

all: $(TARGET)

clean: $(TARGET)
	$(RM) $(TARGET)

re: clean all

$(TARGET): $(TARGET).c
	gcc *.c -o $(TARGET)
