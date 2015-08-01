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
#ifndef MONGOODM_UTILITY_H_
#define MONGOODM_UTILITY_H_

#include <string>
#include <cassert>
#include <mongoc.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <mongoodm/value.h>

namespace mongoodm {

class Utility
{
public:
    static bool BsonToJsonString(const bson_t *b, std::string &str)
    {
        assert(b != NULL);
        size_t length = 0;
        char *s = bson_as_json(b, &length);
        str.assign(s, length);
        bson_free(s);
        return true;
    }

    static std::string JsonToString(rapidjson::Value &json_val)
    {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        json_val.Accept(writer);
        return buffer.GetString();
    }

    static bool MongoDateTime_To_time_t(const rapidjson::Value &from, time_t &to)
    {
        DateTimeValue dt_val;
        if (!dt_val.FromJsonValue(from)) {
            return false;
        }
        to = dt_val.GetTime();
        return true;
    }

    static std::string MongoDateTime_From_time_t(time_t from)
    {
        return DateTimeValue(from).ToJsonString();
    }
};

}  // namespace mongoodm

#endif  // MONGOODM_UTILITY_H_

