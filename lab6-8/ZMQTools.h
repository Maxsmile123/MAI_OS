#ifndef ZMQTOOLS_H
#define ZMQTOOLS_H

#include "zmq.hpp"
#include <string>

extern const int BROADCAST_ID;

std::string create_name_of_socket(int node_id);
std::string create_name_of_socket_to_children(int node_id);
std::string create_name_of_socket_to_parent(int node_id);

#endif //ZMQTOOLS_H
