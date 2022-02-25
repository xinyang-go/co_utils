//
// Created by xinyang on 2/25/22.
//
#pragma once
#ifndef CO_UTILS_HPP
#define CO_UTILS_HPP

#include <coroutine>
#include <exception>
#include <iterator>
#include <concepts>

template<typename T>
class generator {
public:
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        generator get_return_object() {
            return handle_type::from_promise(*this);
        }

        std::suspend_never initial_suspend() {
            return {};
        }

        std::suspend_always final_suspend() noexcept {
            return {};
        }

        void unhandled_exception() {
            exception = std::current_exception();
        }

        template<std::convertible_to<T> From>
        std::suspend_always yield_value(From &&from) {
            value = std::forward<From>(from);
            return {};
        }

        T value;
        std::exception_ptr exception;
    };

    ~generator() { handle.destroy(); }

private:
    struct iterator : public std::iterator<std::forward_iterator_tag, T> {
        iterator &operator++() {
            handle.resume();
            return *this;
        }

        iterator &operator++(int) {
            handle.resume();
            return *this;
        }

        T &operator*() {
            return handle.promise().value;
        }

        T *operator->() {
            return &handle.promise().value;
        }

        bool operator!=(const iterator &) {
            if (handle.promise().exception) {
                std::rethrow_exception(handle.promise().exception);
            }
            return !handle.done();
        }

        iterator(handle_type _handle) : handle(_handle) {}

        handle_type handle;
    };

    generator(handle_type _handle) : handle(_handle) {}

public:
    iterator begin() { return handle; }

    iterator end() { return handle; }

private:
    handle_type handle;
};

#endif /* CO_UTILS_HPP */
