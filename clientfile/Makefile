cc = gcc
deps = client.h server.h utils.h
server_prom = server
server_obj = server.o utils.o
client_prom = client
client_obj = client.o utils.o

$(server_prom): $(server_obj)
	$(cc) -o $(server_prom) $(server_obj) -pthread

$(client_prom): $(client_obj)
	$(cc) -o $(client_prom) $(client_obj)

%.o: %.c $(deps)
	$(cc) -c $< -o $@

clean:
	rm -rf $(server_prom) $(client_prom) $(server_obj) $(client_obj)