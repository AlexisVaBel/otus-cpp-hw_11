#include "async.h"
#include <iostream>

#include "../context/contextholder.h"
namespace async {



handle_t connect(std::size_t bulk) {    
    return ContextHolder::get_instance().do_connect(bulk);
}

void receive(handle_t handle, const char *data, std::size_t size) {    
    ContextHolder::get_instance().do_receive(handle, data, size);
}

void disconnect(handle_t handle) {    
    ContextHolder::get_instance().disconnect(handle);
}

}
