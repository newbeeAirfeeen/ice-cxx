//
// Created by 沈昊 on 2023/12/16.
//
#include "stun.h"
std::string StunField::to_string() const {
    return nonstd::string_view::to_string();
}
nonstd::string_view StunMessage::to_string_view() const {
    return {this->data(), this->size()};
}

std::unique_ptr<MagicCookie> MagicCookie::get_from(const StunMessage &m) {
    auto magic_ = m.to_string_view();
    auto magic_cookie_ = magic_.substr(4, 4);
    nonstd::string_view magic_cookies((const char *) magic_cookie, 4);
    if (magic_cookie_ != magic_cookies) {
        return nullptr;
    }
    std::unique_ptr<MagicCookie> mc(new MagicCookie());
    return std::move(mc);
}

std::string MagicCookie::to_string() const {
    return {this->rbegin(), this->rend()};
}


bool MagicCookie::operator==(const MagicCookie &m) const {
    return this->compare(static_cast<const nonstd::string_view &>(m)) == 0;
}

bool MagicCookie::operator!=(const MagicCookie &m) const {
    return !this->operator==(m);
}

auto is_stun_message(const StunMessage & m) -> bool {
    // the most significant 2 bits of the first byte must be 0
    auto bytes = m.to_string_view();

    if (bytes.size() < STUN_FIXED_HEADER_LENGTH || bytes.size() % 4 != 0) {
        return false;
    }

    auto bits = bytes[0] & 0xC0;
    if (bits != 0) {
        return false;
    }

    auto magic_cookie = MagicCookie::get_from(m);
    if (!magic_cookie) {
        return false;
    }
    return true;

}


//    0                 1
//   2  3  4 5 6 7 8 9 0 1 2 3 4 5
//  +--+--+-+-+-+-+-+-+-+-+-+-+-+-+
//  |M |M |M|M|M|C|M|M|M|C|M|M|M|M|
//  |11|10|9|8|7|1|6|5|4|0|3|2|1|0|
//  +--+--+-+-+-+-+-+-+-+-+-+-+-+-+
//
auto get_stun_type(const StunMessage &m) -> StunType {
    if (!is_stun_message(m)) {
        return StunType::UNKNOWN;
    }
    auto bytes = m.to_string_view();
    auto c1 = bytes[0] & 0x01;
    auto c0 = bytes[1] & 0x10;
    auto type = (c1 << 1) | c0;
    switch (type) {
        case 0b00:
            return StunType::STUN_REQUEST;
        case 0b01:
            return StunType::STUN_INDICATION;
        case 0b10:
            return StunType::STUN_RESPONSE;
        case 0b11:
            return StunType::STUN_ERROR_RESPONSE;
        default:
            return StunType::UNKNOWN;
    }
}

std::unique_ptr<TransactionId> TransactionId::get_from(const StunMessage & m){
    auto bytes = m.to_string_view();
    auto trans_id = bytes.substr(8, STUN_TRANSACTION_ID_LENGTH);
    std::unique_ptr<TransactionId> tid(new TransactionId());
    return std::move(tid);
}

bool TransactionId::operator==(const TransactionId & t) const{
    return this->compare(t) == 0;
}

bool TransactionId::operator!=(const TransactionId & t) const{
    return !this->operator==(t);
}

std::string TransactionId::to_string() const {
    return {this->begin(), this->end()};
}


