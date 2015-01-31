#ifndef COLOR_H_
#define COLOR_H_

#include <mongoodm/mongoodm.h>

using namespace mongoodm;

class Color : public Document
{
private:
    enum {
        kRedFieldNumber = 0,
        kGreenFieldNumber = 1,
        kBlueFieldNumber = 2,
    };

public:
    Color();
    ~Color();

    Color(const Color &other);
    Color& operator=(const Color &other);
    void CopyFrom(const Color &other);
    void Clear();

    virtual bool ParseField(const std::string &name, const rapidjson::Value &json_value);

    // red
    bool has_red() const { return has_bit(kRedFieldNumber); }
    void clear_red();
    const Int32Value* red() const;
    Int32Value& mutable_red();
    void set_red(int32_t value) { mutable_red().SetValue(value); }

    // green
    bool has_green() const { return has_bit(kGreenFieldNumber); }
    void clear_green();
    const Int32Value* green() const;
    Int32Value& mutable_green();
    void set_green(int32_t value) { mutable_green().SetValue(value); }

    // blue
    bool has_blue() const { return has_bit(kBlueFieldNumber); }
    void clear_blue();
    const Int32Value* blue() const;
    Int32Value& mutable_blue();
    void set_blue(int32_t value) { mutable_blue().SetValue(value); }

private:
    bool has_bit(unsigned int field_number) const
    {
        return (_has_bits_ & ((unsigned long long)1 << field_number)) != 0;
    }
    void set_has_bit(unsigned int field_number)
    {
        _has_bits_ |= ((unsigned long long)1 << field_number);
    }
    void clear_has_bit(unsigned int field_number)
    {
        _has_bits_ &= ~((unsigned long long)1 << field_number);
    }
    void set_has_red() { set_has_bit(kRedFieldNumber); }
    void clear_has_red() { clear_has_bit(kRedFieldNumber); }
    void set_has_green() { set_has_bit(kGreenFieldNumber); }
    void clear_has_green() { clear_has_bit(kGreenFieldNumber); }
    void set_has_blue() { set_has_bit(kBlueFieldNumber); }
    void clear_has_blue() { clear_has_bit(kBlueFieldNumber); }

private:
    Int32Field *red_;
    Int32Field *green_;
    Int32Field *blue_;

    unsigned long long _has_bits_;
};

#endif  // COLOR_H_
