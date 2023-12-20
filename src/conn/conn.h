//
// Created by 沈昊 on 2023/12/17.
//

#ifndef ICE_CXX_CONN_H
#define ICE_CXX_CONN_H
#include "asio.hpp"
#include "asio/detail/noncopyable.hpp"
#include <functional>
#include <utility>
#include <vector>
class Conn : protected asio::detail::noncopyable {
public:
    using Addr = std::pair<asio::ip::address, uint16_t>;
    using Buffer = std::vector<uint8_t>;
    using Slice = nonstd::string_view;
    using onAsyncSendResultHdlrFn = std::function<void(size_t, const std::error_code &)>;
    using onAsyncReadResultHdlrFn = std::function<void(const Buffer &, const std::error_code &, const Addr &)>;

public:
    virtual ~Conn() = default;
    virtual std::shared_ptr<asio::io_context> get_io_context() = 0;
    virtual size_t send(Slice buffer) = 0;
    virtual size_t send_to(Slice buffer, const Addr &addr) = 0;
    virtual size_t recv(Buffer &buffer) = 0;
    virtual size_t read_from(Buffer &buffer, const Addr &addr) = 0;
    virtual void async_send_to(Slice buffer, const Addr &addr, const onAsyncSendResultHdlrFn &) = 0;
    virtual void async_send(Slice buffer, const onAsyncSendResultHdlrFn &) = 0;
    virtual void async_read(const onAsyncReadResultHdlrFn &) = 0;
    virtual Addr local_addr() = 0;
    virtual Addr remote_addr() = 0;
};


#endif//ICE_CXX_CONN_H
