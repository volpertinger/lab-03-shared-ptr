// Copyright 2020 Merzlov Nikolay merzlovnik@mail.ru

#ifndef INCLUDE_SHARED_PTR_HPP_
#define INCLUDE_SHARED_PTR_HPP_

#include <atomic>
#include <utility>

class ControlBlock {
  std::atomic_int counter;

 public:
  ControlBlock() : counter(0) {}

  ~ControlBlock() = default;

  void increment() { ++counter; }

  void decrement() { --counter; }

  int get() { return counter; }
};

template <class T>

class Shared_ptr {
  T* data;
  ControlBlock control_block;

 public:
  Shared_ptr();

  Shared_ptr(T* ptr);

  Shared_ptr(const Shared_ptr& arg);

  Shared_ptr(Shared_ptr&& arg);

  ~Shared_ptr();

  auto operator=(const Shared_ptr& arg) -> Shared_ptr&;

  auto operator=(Shared_ptr&& arg) -> Shared_ptr&;

  // проверяет, указывает ли указатель на объект
  operator bool() const;
  auto operator*() const -> T&;
  auto operator-> () const -> T*;

  auto get() -> T*;
  void reset();
  void reset(T* ptr);
  void swap(Shared_ptr& arg);

  // возвращает количество объектов SharedPtr, которые ссылаются на тот же
  // управляемый объект
  auto use_count() const -> std::size_t;
};

#endif  // INCLUDE_SHARED_PTR_HPP_
