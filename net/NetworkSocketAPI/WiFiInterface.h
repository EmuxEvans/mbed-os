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

#ifndef WIFI_INTERFACE_H
#define WIFI_INTERFACE_H

#include "NetworkInterface.h"

/** Enum for WiFi encryption types
*/
enum nsapi_security_t {
    NSAPI_SECURITY_NONE = 0,   /*!< open access point */
    NSAPI_SECURITY_WEP,        /*!< phrase conforms to WEP */
    NSAPI_SECURITY_WPA,        /*!< phrase conforms to WPA */
    NSAPI_SECURITY_WPA2,       /*!< phrase conforms to WPA2 */
};

/** WiFiInterface class
 *  Common interface that is shared between WiFi devices
 */
class WiFiInterface : public NetworkInterface
{
public:
    /** Start the interface
    /param ssid     Name of the network to connect to
    /param pass     Security passphrase to connect to the network
    /param security Type of encryption for connection
    /return         0 on success, negative on failure
    */
    virtual int connect(const char *ssid, const char *pass, nsapi_security_t security = NSAPI_SECURITY_NONE) = 0;

    /** Stop the interface
    /return     0 on success, negative on failure
    */
    virtual int disconnect() = 0;
};

#endif
