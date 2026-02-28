//
// Created by Catalin Chirosca on 2026-02-28.
//

#pragma once

#ifndef CE_DEFINE_BIND_HPP
#define CE_DEFINE_BIND_HPP

#define BIND_FN_NO_PARAMS(fn) [this]() { return fn(); }
#define BIND_FN_ONE_PARAM(fn) [this](auto&& arg) { return fn(std::forward<decltype(arg)>(arg)); }

#endif //CE_DEFINE_BIND_HPP
