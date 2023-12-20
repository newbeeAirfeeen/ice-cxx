//
// Created by 沈昊 on 2023/12/17.
//

#ifndef ICE_CXX_STUN_HPP
#define ICE_CXX_STUN_HPP
#include "nonstd/string_view.hpp"
#include <memory>
#include <string>
#include "conn/conn.h"
/// for IPv4, the actual STUN message would need
/// to be less than 548 bytes (576 minus 20-byte IP header, minus 8-byte
/// UDP header, assuming no IP options are used
constexpr uint32_t STUN_MAX_IPV4_MESSAGE_SIZE = 548;
constexpr uint32_t STUN_MAX_IPV6_MESSAGE_SIZE = 1280;
constexpr uint32_t STUN_UDP_RC_COUNT = 7;
constexpr uint32_t STUN_UDP_RM_COUNT = 16;
constexpr uint32_t STUN_UDP_RETRANSMIT_TIME = 500;
constexpr uint32_t STUN_TRANSACTION_ID_LENGTH = 12;
constexpr uint32_t STUN_FIXED_HEADER_LENGTH = 20;
class StunField : protected nonstd::string_view {
public:
    virtual ~StunField() = default;
    virtual std::string to_string() const;
};
struct StunMessage : protected std::string {
    friend class StunSetter;
    friend class StunField;

public:
    nonstd::string_view to_string_view() const;
};

enum class StunType : uint8_t {
    UNKNOWN,
    STUN_REQUEST,
    STUN_INDICATION,
    STUN_RESPONSE,
    STUN_ERROR_RESPONSE
};


class MagicCookie : public StunField {
public:
    static constexpr uint8_t magic_cookie[] = {0x21, 0x12, 0xA4, 0x42};

public:
    static std::unique_ptr<MagicCookie> get_from(const StunMessage &);

    bool operator==(const MagicCookie &) const;
    bool operator!=(const MagicCookie &) const;

public:
    ~MagicCookie() override = default;
    std::string to_string() const override;

private:
    MagicCookie() = default;
};

class StunSetter {
public:
    virtual ~StunSetter() = default;
    virtual void add_to(StunMessage &) = 0;
};

class TransactionId : public StunField {
public:
    static std::unique_ptr<TransactionId> get_from(const StunMessage &);

    bool operator==(const TransactionId &) const;
    bool operator!=(const TransactionId &) const;
    std::string to_string() const override;
};

/// at any time, a client MAY have multiple outstanding STUN requests
///   with the same STUN server (that is, multiple transactions in
///   progress, with different transaction IDs)
/// a client SHOULD limit itself to ten outstanding transactions to same server
class StunAgent {
public:
    virtual ~StunAgent() = default;

public:
    void verify(const StunMessage &);
    void try_verify(const StunMessage &, const std::error_code& e);

};


auto get_stun_type(const StunMessage &) -> StunType;
auto is_stun_message(const StunMessage &) -> bool;

#endif//ICE_CXX_STUN_HPP
