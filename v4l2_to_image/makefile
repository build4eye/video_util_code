CC = gcc
prom = v4l2_to_image

obj = main.o v4l2_device.o
#deps = main.h

$(prom): $(obj)
	$(CC) -o $(prom) $(obj)
	rm -rf $(obj)

main.o: main.c 
	$(CC) -c main.c

%.o: %.c 
	$(CC) -c $< -o $@

clean:
	rm -rf $(obj) $(prom)