# Problem 033 - list

**ACMOJ Problem ID**: 2100

## Table of Contents

- [Problem 033 - list](#problem-033-list)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
    - [Background](#background)
  - [Assignment Description](#assignment-description)
    - [Grade Composition](#grade-composition)
  - [Assignment Requirements](#assignment-requirements)
    - [Input Format](#input-format)
    - [Output Format](#output-format)
    - [Samples](#samples)
    - [Data Constraints](#data-constraints)
  - [Per-Testcase Resource Limits](#per-testcase-resource-limits)
  - [Test Data](#test-data)
  - [Submission Requirements](#submission-requirements)
    - [OJ Git Repository Compilation Process](#oj-git-repository-compilation-process)
    - [Git Configuration Requirements](#git-configuration-requirements)
    - [Submission Guidelines](#submission-guidelines)
    - [Evaluation Notes](#evaluation-notes)
    - [Academic Integrity](#academic-integrity)

## Introduction

### Background

Implement STL-like list with iterators. Template-based, supports push/pop/insert/erase/splice.

## Assignment Description

### Grade Composition

| Grading Component | Percentage |
| :--: | :--: |
| Pass **2100. list** | 80% |
| Code Review | 20% |

Here are several points that need clarification:

- In the Code Review, we will **strictly examine your code style and repository organization structure, etc.**

- This assignment provides some sample data for testing, stored in the `/workspace/data/033/data_test/` directory. Note that these are not the test cases on the Online Judge. Passing all local test cases does not guarantee that you will pass the OJ tests.

- Besides the provided sample data, we also encourage you to design your own test data based on your program logic to assist debugging.

## Assignment Requirements

### Problem Description

题目下发数据见 [attachment]2100-public.zip[/attachment] / [JBox](https://jbox.sjtu.edu.cn/l/l1iV5f)（你可能需要交大 VPN 以访问，若依然无法下载请联系助教），包含模板代码与下发数据。本题所有基础测试点公开。

基于模版代码实现`list`类，提供操作链表相关的迭代器。

#### 接口说明

* 构造与析构：支持默认构造，拷贝构造。

* `list &operator=(const list &other)`

  提供从另一个list赋值的操作，将`other`的数据拷贝之后赋值过来

* `front()`, `back()`

* `empty()`, `size()`, `clear()`

* `push_back()`, `push_front()`

* `pop_back()`, `pop_front()`

完成以上内容可以拿到本题至少 50% 的分数。

* 迭代器 `iterator` 和 `const_iterator` 的基础操作

* `insert(iterator pos, const T &value)`

  在`pos`之前插入值为`value`的节点，返回这个节点对应的迭代器

* `erase(iterator pos)`

删除在`pos`位置的节点，返回下一个节点对应的迭代器

完成以上内容可以拿到本题另外至少 20% 的分数。此外 30% 的分数为性能测试，测试程序将把你的 list 与 std::list 的性能进行比较，给出得分；若你的程序性能达到 std::list 的 50% 即可拿到满分。

我们期望你能拿到不低于 95% 的分数。

#### 提示

* 不保证模板类型有默认构造函数，处理方法同 `vector`。保证模板类型有`operator<` 和 `operator==` 的定义。
* 你可能需要用到: operator new/placement new

#### 模板代码

```cpp
#pragma once
#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include <memory>
#include <cstddef>

namespace sjtu {


/**
 * @tparam T Type of the elements.
 * Be careful that T may not be default constructable.
 * 
 * @brief A list that supports operations like std::list.
 * 
 * We encourage you to design the implementation yourself.
 * As for the memory management, you may use std::allocator,
 * new/delete, malloc/free or something else.
*/
template <typename T>
class list {
  public:
    class iterator;
    class const_iterator;

  public:

    /**
     * Constructs & Assignments
     * At least three: default constructor, copy constructor/assignment
     * Bonus: move/initializer_list constructor/assignment
     */
    list() {}
    list(const list &other) {}
    list &operator=(const list &other) {}

    /* Destructor. */
    ~list() {}

    /* Access the first / last element. */
    T &front() noexcept {}
    T &back()  noexcept {}
    const T &front() const noexcept {}
    const T &back()  const noexcept {}

    /* Return an iterator pointing to the first element. */
    iterator begin() noexcept {}
    const_iterator cbegin() const noexcept {}

    /* Return an iterator pointing to one past the last element. */
    iterator end() noexcept {}
    const_iterator cend() const noexcept {}

    /* Checks whether the container is empty. */
    bool empty() const noexcept {}
    /* Return count of elements in the container. */
    size_t size() const noexcept {}

    /* Clear the contents. */
    void clear() noexcept {}

    /**
     * @brief Insert value before pos (pos may be the end() iterator).
     * @return An iterator pointing to the inserted value.
     * @throw Throw if the iterator is invalid.
     */
    iterator insert(iterator pos, const T &value) {}

    /**
     * @brief Remove the element at pos (remove end() iterator is invalid).
     * returns an iterator pointing to the following element, if pos pointing to
     * the last element, end() will be returned.
     * @throw Throw if the container is empty, or the iterator is invalid.
     */
    iterator erase(iterator pos) noexcept {}

    /* Add an element to the front/back. */
    void push_front(const T &value) {}
    void push_back (const T &value) {}

    /* Removes the first/last element. */
    void pop_front() noexcept {}
    void pop_back () noexcept {}

  public:

    /**
     * Basic requirements:
     * operator ++, --, *, ->
     * operator ==, != between iterators and const iterators
     * constructing a const iterator from an iterator
     * 
     * If your implementation meets these requirements,
     * you may not comply with the following template.
     * You may even move this template outside the class body,
     * as long as your code works well.
     * 
     * Contact TA whenever you are not sure.
     */
    class iterator {
      private:
        /**
         * TODO just add whatever you want.
         */

      public:
        iterator &operator++()  {}
        iterator &operator--()  {}
        iterator operator++(int) {}
        iterator operator--(int) {}

        T &operator*()  const noexcept {}
        T *operator->() const noexcept {}

        /* A operator to check whether two iterators are same (pointing to the same memory) */
        friend bool operator == (const iterator &a, const iterator &b) {}
        friend bool operator != (const iterator &a, const iterator &b) {}
    };

    /**
     * Const iterator should have same functions as iterator, just for a const object.
     * It should be able to construct from an iterator.
     * It should be able to compare with an iterator.
     */
    class const_iterator {};
};

} // namespace sjtu

#endif // SJTU_LIST_HPP

```

### Input Format

See the problem description above.

### Output Format

See the problem description above.

### Samples

No sample data provided for this problem.

### Data Constraints

See the problem description for constraints.

## Per-Testcase Resource Limits

- **Time Limit (per test case)**: 9999 ms
- **Memory Limit (per test case)**: 256 MiB
- **Disk Usage**: No disk usage is permitted.

## Test Data

The test data for this problem is located at `/workspace/data/033/data_test/`.

Attachments and starter files for this problem are located at `/workspace/data/033/`.

## Submission Requirements

### OJ Git Repository Compilation Process

For Git compilation, we will first clone the repository using a command similar to:
```bash
git clone <repo_url> . --depth 1 --recurse-submodules --shallow-submodules --no-local
```

Then we check if there is a `CMakeLists.txt` file. If it exists, we run (if not, a warning message will be displayed):
```bash
cmake .
```

Finally, we check if there is any of `GNUmakefile`/`makefile`/`Makefile` (if cmake was run previously, this will be the generated Makefile). If it exists, we run (if not, a warning message will be displayed):
```bash
make
```

After this process is complete, we will use the `code` file in the project root directory as the compilation result.

A `CMakeLists.txt` file is provided in the project. You can use or modify it as needed. The local environment has gcc-13 and g++-13 available.

### Git Configuration Requirements

**IMPORTANT**: You must create a `.gitignore` file in your project root directory to avoid OJ evaluation conflicts.

The `.gitignore` file should include at least the following entries:

```gitignore
CMakeFiles/
CMakeCache.txt
```

### Submission Guidelines

- The submitted code must be able to compile successfully through the above compilation process
- The compiled executable file name must be `code`
- The program needs to be able to read data from standard input and write results to standard output
- Please ensure the code runs correctly within the given time and space limits
- **You must use C++ or C language** to implement this assignment

### Evaluation Notes

- The evaluation system will test your program using the provided test data
- The program output must exactly match the expected output (including format)
- Exceeding time or memory limits will be judged as the corresponding error type
- Please pay attention to the overall time performance of your code and the time complexity of each part of your algorithm.

### Academic Integrity

If any violations are found during evaluation or code review (including but not limited to using unconventional methods to pass test cases), your final score may be significantly reduced or become **0 points**.
