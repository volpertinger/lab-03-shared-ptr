// Copyright 2020 Merzlov Nikolay merzlovnik@mail.ru

#ifndef INCLUDE_SHARED_PTR_HPP_
#define INCLUDE_SHARED_PTR_HPP_

#include <atomic>
#include <utility>

class ControlBlock {
  std::atomic_int strong_counter;
  std::atomic_int weak_counter;

 public:
  ControlBlock() : strong_counter(0), weak_counter(0) {}

  ~ControlBlock() = default;

  int increment_strong() {
    ++strong_counter;
    return strong_counter;
  }

  int increment_weak() {
    ++weak_counter;
    return weak_counter;
  }

  int decrement_strong() {
    --strong_counter;
    return strong_counter;
  }

  int decrement_weak() {
    --weak_counter;
    return weak_counter;
  }

  int get_strong() { return strong_counter; }

  int get_weak() { return weak_counter; }
};

template <class T>

class Shared_ptr {
  T* data;
  ControlBlock* control_block;

 public:
  Shared_ptr<T>() {
    data = nullptr;
    control_block = nullptr;
  }

  explicit Shared_ptr(T* ptr) {
    data = ptr;
    control_block = new ControlBlock;
    control_block->increment_strong();
  }

  Shared_ptr(const Shared_ptr& arg) {
    data = arg.data;
    control_block = arg.control_block;
    control_block->increment_strong();
  }

  Shared_ptr(Shared_ptr&& arg) noexcept {
    data = std::move(arg.data);
    control_block = std::move(arg.control_block);
  }

  ~Shared_ptr() {
    if (control_block != nullptr) {
      if (!control_block->decrement_strong()) {
        delete data;
        delete control_block;
      }
    }
  }

  auto operator=(Shared_ptr& arg) noexcept {
    if (data != arg.data) {
      if (control_block != nullptr) {
        if (!control_block->decrement_strong()) {
          delete data;
          delete control_block;
        }
      }
      data = arg.data;
      control_block = arg.control_block;
      control_block->increment_strong();
    }
    return *this;
  }

  auto operator=(Shared_ptr&& arg) noexcept {
    if (*this != arg) {
      std::swap(data, arg.data);
      std::swap(control_block, arg.control_block);
    }
    return *this;
  }

  operator bool() const noexcept { return data != nullptr; }
  auto operator*() const noexcept { return *data; }
  auto operator-> () const noexcept { return data; }

  auto get() { return *data; }
  void reset() {
    if (!control_block->decrement_strong()) {
      delete data;
      delete control_block;
    }
    data = nullptr;
    control_block = nullptr;
  }
  void reset(T* ptr) {
    if (control_block != nullptr) {
      if (!control_block->decrement_strong()) {
        delete data;
        delete control_block;
      }
    }
    data = ptr;
    control_block = new ControlBlock;
    control_block->increment_strong();
  }
  void swap(Shared_ptr& arg) {
    if (arg.data != data) {
      std::swap(data, arg.data);
      std::swap(control_block, arg.control_block);
    }
  }

  auto use_count() const { return control_block->get_strong(); }
};

#endif  // INCLUDE_SHARED_PTR_HPP_
