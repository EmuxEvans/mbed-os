/* Socket
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

#ifndef SOCKET_ADDRESS_H
#define SOCKET_ADDRESS_H

#include <stdint.h>

/** Maximum size of IP address
*/
#define NSAPI_IP_SIZE 16

/** Maximum size of MAC address
*/
#define NSAPI_MAC_SIZE 18

// Predeclared classes
class NetworkInterface;

/**
 * A general socket address composed of the IP address and port
 */
class SocketAddress {
public:
    /** SocketAddress construction using DNS resolution
    /param iface    NetworkInterface to use for DNS resolution
    /param addr     Null-terminated hostname that will be resolved
    /param port     16-bit port
    /note on failure, IP address and port will be set to null
    */
    SocketAddress(NetworkInterface *iface, const char *addr, uint16_t port = 0);

    /** SocketAddress construction
    /param addr     Null-terminated IP address
    /param port     16-bit port
    /note on failure, IP address and port will be set to null
    */
    SocketAddress(const char *addr = 0, uint16_t port = 0);

    /** SocketAddress construction
    /param addr     SocketAddress to copy
    */
    SocketAddress(const SocketAddress &addr);
   
    /** Set the IP address
    \param addr     Null-terminated string representing the IP address
     */
    void set_ip_address(const char *addr);

    /** Set the port
    \param port     16-bit port
     */
    void set_port(uint16_t port);
    
    /** Get the IP address
    \return         The string representation of the IP Address
     */
    const char *get_ip_address() const;
    
    /** Get the port
    \return         The 16-bit port
     */
    uint16_t get_port(void) const;

private:
    char _ip_address[NSAPI_IP_SIZE];
    uint16_t _port;
};

#endif
