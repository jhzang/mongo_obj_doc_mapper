#include <string>
#include <mongoc.h>
#include <mongoodm/mongoodm.h>
#include "account.h"

using namespace mongoodm;

int main()
{
    std::string uri_str = "mongodb://127.0.0.1:27017/test/?minPoolSize=1&maxPoolSize=5&w=1&wtimeoutMS=1000&journal=true";
    std::string db_name = "test";
    std::string collection_name = "sample";

    Account acc0;

    mongoc_init();

    mongoc_uri_t *uri_ = mongoc_uri_new(uri_str.c_str());
    mongoc_client_pool_t *conn_pool_ = mongoc_client_pool_new(uri_);
    mongoc_uri_destroy(uri_);
    uri_ = NULL;

    mongoc_client_t *client_ = mongoc_client_pool_pop(conn_pool_);
    mongoc_collection_t *collection_ = mongoc_client_get_collection(client_, db_name.c_str(), collection_name.c_str());
    bson_t *query = bson_new();
    mongoc_cursor_t *cursor = mongoc_collection_find(collection_, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
    const bson_t *d = NULL;
    while (mongoc_cursor_next(cursor, &d)) {
        size_t length = 0;
        char *str = bson_as_json(d, &length);
        std::cout << str << std::endl;
        if (acc0.FromJsonString(str, length)) {
            std::cout << "Parsing Account succeeded" << std::endl;
            std::cout << acc0.ToJsonString() << std::endl;
        }
        else {
            std::cout << "Parsing Account failed" << std::endl;
        }
        bson_free(str);
    }
    bson_destroy(query);
    mongoc_collection_destroy(collection_);
    mongoc_client_pool_push(conn_pool_, client_);

    mongoc_client_pool_destroy(conn_pool_);
    conn_pool_ = NULL;
    mongoc_cleanup();

    std::cout << "------------------------test ClientPool------------------------------" << std::endl;

    ClientPool pool;
    if (!pool.Initialize(uri_str.c_str())) {
        std::cerr << "Initializing ClientPool failed. uri_str=\"" << uri_str << "\"" << std::endl;
        return 1;
    }

	Client *client = pool.Pop();
    std::string server_status_str;
    if (client->GetServerStatus(server_status_str)) {
        std::cout << "Server status: " << server_status_str << std::endl;
    }

	Collection *collection = client->GetCollection(db_name.c_str(), "account");
	collection->Drop();

    std::cout << "------Inserting into account------" << std::endl;
    Account acc = acc0;
    acc.clear__id();
    acc.mutable_age().SetValue(10);
    if (!collection->InsertDocument(&acc)) {
        std::cerr << "Inserting account failed. acc=\"" << acc.ToJsonString() << "\"" << std::endl;
		delete collection;
		pool.Push(client);
        return 2;
    }
    std::cout << "------Finding in account------" << std::endl;
    std::vector<Account*> accounts;
    int count = collection->FindDocuments(accounts, "{\"age\":10}");
    if (count == 0) {
        std::cout << "No documents in test.account" << std::endl;
		delete collection;
		pool.Push(client);
        return 3;
    }
    for (std::vector<Account*>::iterator it = accounts.begin(); it != accounts.end(); ++it) {
        std::cout << (*it)->ToJsonString() << std::endl;
        std::cout << "---Updating " << (*it)->GetIdField()->GetValue()->ToJsonString() << "---" << std::endl;
        (*it)->mutable_height().SetValue(2.11);
        (*it)->clear_male();
        if (!collection->UpdateDocument(MONGOC_UPDATE_NONE, *it)) {
            std::cout << "Failed" << std::endl;
        }
        else {
            std::cout << (*it)->ToJsonString() << std::endl;
        }
    }
    for (std::vector<Account*>::iterator it = accounts.begin(); it != accounts.end(); ++it) {
        delete *it;
        *it = NULL;
    }
	delete collection;
	pool.Push(client);
    return 0;
}

