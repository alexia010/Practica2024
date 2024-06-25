# Define compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -g

# Define the target executables
TARGET_SERVER = server/main_server
TARGET_CLIENT = client/main_client

# Define the source files and header files for server
SRCS_SERVER = server/main_server.cpp server/server.cpp 
HDRS_SERVER = server/server.h

# Define the source files and header files for client
SRCS_CLIENT = client/main_client.cpp client/client.cpp client/CDNS_request.cpp client/DNS_parts.cpp
HDRS_CLIENT = client/client.h  client/CDNS_request.h client/DNS_parts.h client/IDNS_message.h client/CDNS_response.h

# Define the object files for server and client
OBJS_SERVER = $(SRCS_SERVER:.cpp=.o)
OBJS_CLIENT = $(SRCS_CLIENT:.cpp=.o)

# Default rule to build both executables
all: $(TARGET_SERVER) $(TARGET_CLIENT)

# Rule to build the server executable
$(TARGET_SERVER): $(OBJS_SERVER)
	$(CXX) $(CXXFLAGS) -o $(TARGET_SERVER) $(OBJS_SERVER)

# Rule to build the client executable
$(TARGET_CLIENT): $(OBJS_CLIENT)
	$(CXX) $(CXXFLAGS) -o $(TARGET_CLIENT) $(OBJS_CLIENT)

# Rule to build object files from source files for server
server/%.o: server/%.cpp $(HDRS_SERVER)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to build object files from source files for client
client/%.o: client/%.cpp $(HDRS_CLIENT)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and the executables
clean:
	rm -f $(OBJS_SERVER) $(OBJS_CLIENT) $(TARGET_SERVER) $(TARGET_CLIENT)

# Phony targets
.PHONY: all clean
