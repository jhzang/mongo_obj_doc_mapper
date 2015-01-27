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
#include "field.h"
#include "document.h"

namespace mongoodm {

internal::NullValue NullValue;
const BoolValue TrueValue(true);
const BoolValue FalseValue(false);


Value* Value::Create(const rapidjson::Value &json_value)
{
    Value *value = NULL;
    if (json_value.IsNull()) {
        value = dynamic_cast<Value*>(const_cast<internal::NullValue*>(&NullValue));
    }
    else if (json_value.IsBool()) {
        if (json_value.IsTrue()) {
            value = const_cast<Value*>(dynamic_cast<const Value*>(&TrueValue));
        }
        else {
            value = const_cast<Value*>(dynamic_cast<const Value*>(&FalseValue));
        }
    }
    else if (json_value.IsInt()) {
        value = new Int32Value(json_value.GetInt());
    }
    else if (json_value.IsUint()) {
        value = new UInt32Value(json_value.GetUint());
    }
    else if (json_value.IsInt64()) {
        value = new Int64Value(json_value.GetInt64());
    }
    else if (json_value.IsUint64()) {
        value = new UInt64Value(json_value.GetUint64());
    }
    else if (json_value.IsDouble()) {
        value = new DoubleValue(json_value.GetDouble());
    }
    else if (json_value.IsString()) {
        value = new StringValue(json_value.GetString(), json_value.GetStringLength());
    }
    else if (json_value.IsObject()) {
        if (json_value.HasMember("$oid")) {
            ObjectIdValue *v = new ObjectIdValue();
            v->FromJsonValue(json_value);
            value = v;
        }
        else if (json_value.HasMember("$date")) {
            DateTimeValue *v = new DateTimeValue();
            v->FromJsonValue(json_value);
            value = v;
        }
        else if (json_value.HasMember("$binary")) {
            BinaryValue *v = new BinaryValue();
            v->FromJsonValue(json_value);
            value = v;
        }
        else {
            Document *v = new Document();
            v->FromJsonValue(json_value);
            value = v;
        }
    }
    else if (json_value.IsArray()) {
        ArrayValue *v = new ArrayValue();
        v->FromJsonValue(json_value);
        value = v;
    }

    return value;
}

}  // namespace mongoodm

