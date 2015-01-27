/**
 * Copyright (c) 2015 Junheng Zang(junheng.zang@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.  
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 */
#include "client_pool.h"
#include <sstream>
#include "document.h"

namespace mongoodm {

ClientPool::ClientPool()
    : client_pool_(NULL)
{
}

ClientPool::~ClientPool()
{
    Cleanup();
}

bool ClientPool::Initialize(const std::string &address, int port, 
        const std::string &dbname, const std::string &dbuser, const std::string &password, 
        unsigned int min_pool_size, unsigned int max_pool_size,
        const std::string &write_concern, int write_timeout_ms,
        bool journal)
{
    std::stringstream ss;
    ss << "mongodb://";
    if (!dbuser.empty()) {
        ss << dbuser;
        if (!password.empty()) {
            ss << ":" << password;
        }
        ss << "@";
    }
    if (address.empty() || dbname.empty()) {
        return false;
    }
    ss << address;
    if (port > 0) {
        ss << ":" << port;
    }
    ss << "/" << dbname;
    ss << "?minPoolSize=" << min_pool_size
       << "&maxPoolSize=" << max_pool_size
       << "&w=" << write_concern 
       << "&wtimeoutMS=" << write_timeout_ms 
       << "&journal=" << journal;

    return Initialize(ss.str().c_str());
}

bool ClientPool::Initialize(const char *uri_string)
{
    uri_str_ = uri_string;

    mongoc_init();
    mongoc_uri_t *uri = mongoc_uri_new(uri_str_.c_str());
    client_pool_ = mongoc_client_pool_new(uri);
    mongoc_uri_destroy(uri);
    uri = NULL;

    return true;
}

void ClientPool::Cleanup()
{
    if (client_pool_ != NULL) {
        mongoc_client_pool_destroy(client_pool_);
        client_pool_ = NULL;
        mongoc_cleanup();
    }
}

Client* ClientPool::Pop()
{
	assert(client_pool_ != NULL);
    mongoc_client_t *client = mongoc_client_pool_pop(client_pool_);
	return new Client(client);
}

Client* ClientPool::TryPop()
{
	assert(client_pool_ != NULL);
    mongoc_client_t *client = mongoc_client_pool_try_pop(client_pool_);
	return new Client(client);
}

void ClientPool::Push(Client *&client)
{
	assert(client_pool_ != NULL);
    mongoc_client_pool_push(client_pool_, client->GetRaw());
	delete client;
	client = NULL;
}

}  // namespace mongoodm

