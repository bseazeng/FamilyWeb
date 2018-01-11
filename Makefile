server-objs:=fw_main.o fw_file.o
test-objs:=test.o
test2-objs:=test2.o
all:webserver test2_server test_server
%.o:%.c
	$(CC) -c $<
webserver:$(server-objs)
	$(CC) -o $@ $(server-objs)
test_server:$(test-objs)
	$(CC) -o $@ $(test-objs)
test2_server:$(test2-objs)
	$(CC) -o $@ $(test2-objs)
clean:
	rm *.o webserver* test2_server* test_server*