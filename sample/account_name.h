#ifndef ENTITY_ACCOUNT_NAME_H_
#define ENTITY_ACCOUNT_NAME_H_

#include <mongoodm/mongoodm.h>


namespace entity {

class Account_name : public mongoodm::Document
{
private:
    enum {
        kFirstNameFieldNumber = 0,
        kLastNameFieldNumber = 1,
    };

public:
    Account_name();
    virtual ~Account_name();

    Account_name(const Account_name &other) { CopyFrom(other); }
    Account_name& operator=(const Account_name &other) { CopyFrom(other); return *this; }
    virtual void CopyFrom(const Value &other);
    virtual Value* Clone() const { return new Account_name(*this); }
    void Clear();

    virtual int ParseField(const std::string &name, const rapidjson::Value &json_value);

    // first_name
    inline bool has_first_name() const { return has_bit(kFirstNameFieldNumber); }
    void clear_first_name();
    const mongoodm::StringValue* first_name() const;
    mongoodm::StringValue& mutable_first_name();
    inline std::string first_name_value() const { return first_name_->GetValue().GetValue(); }
    inline void set_first_name(const std::string &value) { mutable_first_name().SetValue(value); }

    // last_name
    inline bool has_last_name() const { return has_bit(kLastNameFieldNumber); }
    void clear_last_name();
    const mongoodm::StringValue* last_name() const;
    mongoodm::StringValue& mutable_last_name();
    inline std::string last_name_value() const { return last_name_->GetValue().GetValue(); }
    inline void set_last_name(const std::string &value) { mutable_last_name().SetValue(value); }

private:
    inline bool has_bit(unsigned int field_number) const
    {
        return (_has_bits_ & ((unsigned long long)1 << field_number)) != 0;
    }
    inline void set_has_bit(unsigned int field_number)
    {
        _has_bits_ |= ((unsigned long long)1 << field_number);
    }
    inline void clear_has_bit(unsigned int field_number)
    {
        _has_bits_ &= ~((unsigned long long)1 << field_number);
    }
    inline void set_has_first_name() { set_has_bit(kFirstNameFieldNumber); }
    inline void clear_has_first_name() { clear_has_bit(kFirstNameFieldNumber); }
    inline void set_has_last_name() { set_has_bit(kLastNameFieldNumber); }
    inline void clear_has_last_name() { clear_has_bit(kLastNameFieldNumber); }

private:
    mongoodm::StringField *first_name_;
    mongoodm::StringField *last_name_;

    unsigned long long _has_bits_;
};

}  // namespace entity

#endif  // ENTITY_ACCOUNT_NAME_H_
