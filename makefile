CXX ?= g++

DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CXXFLAGS += -g
else
    CXXFLAGS += -O2

endif

objects=main.o timer/lst_timer.o http/http_conn.o log/log.o CGImysql/sql_connection_pool.o webserver.o config.o

server: $(objects)
	$(CXX) $(CXXFLAGS) -o server $^ -lpthread -lmysqlclient
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<
clean:
	find . -name "*.o" -type f -delete
	rm -r server

