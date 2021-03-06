/* ESP8266 implementation of NetworkInterfaceAPI
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 

#include "ESP8266Interface.h"

// Various timeouts for different ESP8266 operations
#define ESP8266_CONNECT_TIMEOUT 15000
#define ESP8266_SEND_TIMEOUT    500
#define ESP8266_RECV_TIMEOUT    0
#define ESP8266_MISC_TIMEOUT    500


// ESP8266Interface implementation
ESP8266Interface::ESP8266Interface(PinName tx, PinName rx, bool debug)
    : _esp(tx, rx, debug)
{
    memset(_ids, 0, sizeof(_ids));
}

int ESP8266Interface::connect(
    const char *ssid,
    const char *pass,
    nsapi_security_t security)
{
    _esp.setTimeout(ESP8266_CONNECT_TIMEOUT);

    if (!_esp.startup(3)) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    if (!_esp.dhcp(true, 1)) {
        return NSAPI_ERROR_DHCP_FAILURE;
    }

    if (!_esp.connect(ssid, pass)) {
        return NSAPI_ERROR_NO_CONNECTION;
    }

    if (!_esp.getIPAddress()) {
        return NSAPI_ERROR_DHCP_FAILURE;
    }

    return 0;
}

int ESP8266Interface::disconnect()
{
    _esp.setTimeout(ESP8266_MISC_TIMEOUT);

    if (!_esp.disconnect()) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }

    return 0;
}

const char *ESP8266Interface::get_ip_address()
{
    return _esp.getIPAddress();
}

const char *ESP8266Interface::get_mac_address()
{
    return _esp.getMACAddress();
}

struct esp8266_socket {
    int id;
    nsapi_protocol_t proto;
    bool connected;
};

void *ESP8266Interface::socket_create(nsapi_protocol_t proto)
{
    // Look for an unused socket
    int id = -1;
 
    for (int i = 0; i < ESP8266_SOCKET_COUNT; i++) {
        if (!_ids[i]) {
            id = i;
            _ids[i] = true;
            break;
        }
    }
 
    if (id == -1) {
        return 0;
    }
    
    struct esp8266_socket *socket = new struct esp8266_socket;
    if (!socket) {
        return 0;
    }
    
    socket->id = id;
    socket->proto = proto;
    socket->connected = false;
    return socket;
}

void ESP8266Interface::socket_destroy(void *handle)
{
    struct esp8266_socket *socket = (struct esp8266_socket *)handle;
    _ids[socket->id] = false;
    delete socket;
}

int ESP8266Interface::socket_set_option(void *handle, int optname, const void *optval, unsigned optlen)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

int ESP8266Interface::socket_get_option(void *handle, int optname, void *optval, unsigned *optlen)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

int ESP8266Interface::socket_bind(void *handle, int port)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

int ESP8266Interface::socket_listen(void *handle, int backlog)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

int ESP8266Interface::socket_connect(void *handle, const SocketAddress &addr)
{
    struct esp8266_socket *socket = (struct esp8266_socket *)handle;
    _esp.setTimeout(ESP8266_MISC_TIMEOUT);

    const char *proto = (socket->proto == NSAPI_UDP) ? "UDP" : "TCP";
    if (!_esp.open(proto, socket->id, addr.get_ip_address(), addr.get_port())) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }
    
    socket->connected = true;
    return 0;
}
    
bool ESP8266Interface::socket_is_connected(void *handle)
{
    return true;
}

int ESP8266Interface::socket_accept(void *handle, void **connection)
{
    return NSAPI_ERROR_UNSUPPORTED;
}

int ESP8266Interface::socket_send(void *handle, const void *data, unsigned size)
{
    struct esp8266_socket *socket = (struct esp8266_socket *)handle;
    _esp.setTimeout(ESP8266_SEND_TIMEOUT);
 
    if (!_esp.send(socket->id, data, size)) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }
 
    return size;
}

int ESP8266Interface::socket_recv(void *handle, void *data, unsigned size)
{
    struct esp8266_socket *socket = (struct esp8266_socket *)handle;
    _esp.setTimeout(ESP8266_RECV_TIMEOUT);
 
    int32_t recv = _esp.recv(socket->id, data, size);
    if (recv < 0) {
        return NSAPI_ERROR_WOULD_BLOCK;
    }
 
    return recv;
}

int ESP8266Interface::socket_sendto(void *handle, const SocketAddress &addr, const void *data, unsigned size)
{
    struct esp8266_socket *socket = (struct esp8266_socket *)handle;
    if (!socket->connected) {
        int err = socket_connect(socket, addr);
        if (err < 0) {
            return err;
        }
    }
    
    return socket_send(socket, data, size);
}

int ESP8266Interface::socket_recvfrom(void *handle, SocketAddress *addr, void *data, unsigned size)
{
    struct esp8266_socket *socket = (struct esp8266_socket *)handle;    
    return socket_recv(socket, data, size);
}

int ESP8266Interface::socket_close(void *handle, bool shutdown)
{
    struct esp8266_socket *socket = (struct esp8266_socket *)handle;
    _esp.setTimeout(ESP8266_MISC_TIMEOUT);
 
    if (!_esp.close(socket->id)) {
        return NSAPI_ERROR_DEVICE_ERROR;
    }
 
    return 0;
}

void ESP8266Interface::socket_attach_accept(void *handle, void (*callback)(void *), void *id)
{
}

void ESP8266Interface::socket_attach_send(void *handle, void (*callback)(void *), void *id)
{
}

void ESP8266Interface::socket_attach_recv(void *handle, void (*callback)(void *), void *id)
{
}
