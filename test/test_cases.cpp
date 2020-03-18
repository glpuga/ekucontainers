/**
 * ekuvector, std::vector clone.
 * @author Gerardo Puga
 * */

// Standard library
#include <cstdint>
#include <string>
#include <utility>

// gtest and gmock
#include "gtest/gtest.h"

// Library
#include <ekuvector/ekuvector.hpp>

namespace ekustd {

class IChar {
public:
  IChar() { ++default_constructor_; }
  IChar(char value) {
    ++value_constructor_;
    value_ = value;
  }
  IChar(const IChar &other) {
    ++copy_ops_;
    value_ = other.value_;
  }
  IChar(IChar &&other) {
    ++move_ops_;
    value_ = other.value_;
  }
  IChar &operator=(const IChar &other) {
    ++copy_ops_;
    value_ = other.value_;
    return *this;
  }
  IChar &operator=(IChar &&other) {
    ++move_ops_;
    value_ = other.value_;
    return *this;
  }

  static void reset() {
    default_constructor_ = 0;
    value_constructor_ = 0;
    copy_ops_ = 0;
    move_ops_ = 0;
  }

  static int32_t default_constructor_;
  static int32_t value_constructor_;
  static int32_t copy_ops_;
  static int32_t move_ops_;

private:
  char value_;
};

int32_t IChar::default_constructor_;
int32_t IChar::value_constructor_;
int32_t IChar::copy_ops_;
int32_t IChar::move_ops_;

class EkuVectorTests : public testing::Test {};

class ConstructorTests : public EkuVectorTests {};

TEST_F(ConstructorTests, DefaultConstructor) {
  ekuvector<int32_t> uut_pod;
  ekuvector<std::string> uut_obj;

  EXPECT_EQ(0, uut_pod.size());
  EXPECT_EQ(0, uut_obj.size());
}

TEST_F(ConstructorTests, ConstructorWithCount) {
  ekuvector<int32_t> uut_pod(3);
  ekuvector<std::string> uut_obj(3);

  EXPECT_EQ(3, uut_pod.size());
  ASSERT_EQ(3, uut_obj.size());
  EXPECT_EQ("", uut_obj[0]);
  EXPECT_EQ("", uut_obj[1]);
  EXPECT_EQ("", uut_obj[2]);
}

TEST_F(ConstructorTests, ConstructorWithCountAndValueNoAllocator) {
  const int32_t canary_pod{99};
  const std::string canary_obj{"test string"};

  ekuvector<int32_t> uut_pod(3, canary_pod);
  ekuvector<std::string> uut_obj(3, canary_obj);

  ASSERT_EQ(3, uut_pod.size());
  EXPECT_EQ(canary_pod, uut_pod[0]);
  EXPECT_EQ(canary_pod, uut_pod[1]);
  EXPECT_EQ(canary_pod, uut_pod[2]);

  ASSERT_EQ(3, uut_obj.size());
  EXPECT_EQ(canary_obj, uut_obj[0]);
  EXPECT_EQ(canary_obj, uut_obj[1]);
  EXPECT_EQ(canary_obj, uut_obj[2]);
}

TEST_F(ConstructorTests, ConstructorWithIteratorsNoAllocator) {
  const std::vector<int32_t> std_vector_pod({97, 98, 99});
  const std::vector<std::string> std_vector_obj({"97", "98", "99"});

  ekuvector<int32_t> uut_pod(std_vector_pod.begin(), std_vector_pod.end());
  ekuvector<std::string> uut_obj(std_vector_obj.begin(), std_vector_obj.end());

  ASSERT_EQ(3, uut_pod.size());
  EXPECT_EQ(97, uut_pod[0]);
  EXPECT_EQ(98, uut_pod[1]);
  EXPECT_EQ(99, uut_pod[2]);

  ASSERT_EQ(3, uut_obj.size());
  EXPECT_EQ("97", uut_obj[0]);
  EXPECT_EQ("98", uut_obj[1]);
  EXPECT_EQ("99", uut_obj[2]);
}

TEST_F(ConstructorTests, CopyConstructorNoAllocator) {
  const ekuvector<int32_t> source_vector_pod({97, 98, 99});
  const ekuvector<std::string> source_vector_obj({"97", "98", "99"});

  ekuvector<int32_t> uut_pod(source_vector_pod);
  ekuvector<std::string> uut_obj(source_vector_obj);

  ASSERT_EQ(3, uut_pod.size());
  EXPECT_EQ(97, uut_pod[0]);
  EXPECT_EQ(98, uut_pod[1]);
  EXPECT_EQ(99, uut_pod[2]);

  ASSERT_EQ(3, uut_obj.size());
  EXPECT_EQ("97", uut_obj[0]);
  EXPECT_EQ("98", uut_obj[1]);
  EXPECT_EQ("99", uut_obj[2]);

  ASSERT_EQ(3, source_vector_pod.size());
  ASSERT_EQ(3, source_vector_obj.size());
}

TEST_F(ConstructorTests, MoveConstructorNoAllocator) {
  ekuvector<int32_t> source_vector_pod({97, 98, 99});
  ekuvector<std::string> source_vector_obj({"97", "98", "99"});

  ekuvector<int32_t> uut_pod(std::move(source_vector_pod));
  ekuvector<std::string> uut_obj(std::move(source_vector_obj));

  ASSERT_EQ(3, uut_pod.size());
  EXPECT_EQ(97, uut_pod[0]);
  EXPECT_EQ(98, uut_pod[1]);
  EXPECT_EQ(99, uut_pod[2]);

  ASSERT_EQ(3, uut_obj.size());
  EXPECT_EQ("97", uut_obj[0]);
  EXPECT_EQ("98", uut_obj[1]);
  EXPECT_EQ("99", uut_obj[2]);

  // sources should have been emptied
  ASSERT_EQ(0, source_vector_pod.size());
  ASSERT_EQ(0, source_vector_obj.size());
}

TEST_F(ConstructorTests, ConstructFromInitiliazerListNoAllocator) {
  std::initializer_list<int32_t> source_init_list_pod({97, 98, 99});
  std::initializer_list<std::string> source_init_list_obj({"97", "98", "99"});

  ekuvector<int32_t> uut_pod(source_init_list_pod);
  ekuvector<std::string> uut_obj(source_init_list_obj);

  ASSERT_EQ(3, uut_pod.size());
  EXPECT_EQ(97, uut_pod[0]);
  EXPECT_EQ(98, uut_pod[1]);
  EXPECT_EQ(99, uut_pod[2]);

  ASSERT_EQ(3, uut_obj.size());
  EXPECT_EQ("97", uut_obj[0]);
  EXPECT_EQ("98", uut_obj[1]);
  EXPECT_EQ("99", uut_obj[2]);
}

class AssignmentTests : public EkuVectorTests {};

TEST_F(AssignmentTests, CopyAssignment) {
  {
    ekuvector<int32_t> source_vector_pod({97, 98, 99});
    ekuvector<std::string> source_vector_obj({"97", "98", "99"});

    ekuvector<int32_t> uut_pod;
    ekuvector<std::string> uut_obj;

    ASSERT_EQ(0, uut_pod.size());
    ASSERT_EQ(0, uut_obj.size());

    uut_pod = source_vector_pod;
    uut_obj = source_vector_obj;

    ASSERT_EQ(3, source_vector_pod.size());
    ASSERT_EQ(3, source_vector_obj.size());

    ASSERT_EQ(3, uut_pod.size());
    EXPECT_EQ(97, uut_pod[0]);
    EXPECT_EQ(98, uut_pod[1]);
    EXPECT_EQ(99, uut_pod[2]);

    ASSERT_EQ(3, uut_obj.size());
    EXPECT_EQ("97", uut_obj[0]);
    EXPECT_EQ("98", uut_obj[1]);
    EXPECT_EQ("99", uut_obj[2]);
  }
  {
    ekuvector<IChar> source({'a', 'b', 'c'});
    ekuvector<IChar> instrumented_uut;
    IChar::reset();
    instrumented_uut = source;
    EXPECT_EQ(0, IChar::value_constructor_);
    EXPECT_EQ(3, IChar::copy_ops_);
    EXPECT_EQ(0, IChar::move_ops_);
  }
}

TEST_F(AssignmentTests, MoveAssignment) {
  {
    ekuvector<int32_t> source_vector_pod({97, 98, 99});
    ekuvector<std::string> source_vector_obj({"97", "98", "99"});

    ekuvector<int32_t> uut_pod;
    ekuvector<std::string> uut_obj;

    ASSERT_EQ(0, uut_pod.size());
    ASSERT_EQ(0, uut_obj.size());

    uut_pod = std::move(source_vector_pod);
    uut_obj = std::move(source_vector_obj);

    ASSERT_EQ(3, uut_pod.size());
    EXPECT_EQ(97, uut_pod[0]);
    EXPECT_EQ(98, uut_pod[1]);
    EXPECT_EQ(99, uut_pod[2]);

    ASSERT_EQ(3, uut_obj.size());
    EXPECT_EQ("97", uut_obj[0]);
    EXPECT_EQ("98", uut_obj[1]);
    EXPECT_EQ("99", uut_obj[2]);
  }
  {
    ekuvector<IChar> source({'a', 'b', 'c'});
    ekuvector<IChar> instrumented_uut;
    IChar::reset();
    instrumented_uut = std::move(source);
    EXPECT_EQ(0, IChar::value_constructor_);
    EXPECT_EQ(0, IChar::copy_ops_);
    EXPECT_EQ(3, IChar::move_ops_);
  }
}

TEST_F(AssignmentTests, InitializerListAssignment) {
  std::initializer_list<int32_t> source_init_list_pod({97, 98, 99});
  std::initializer_list<std::string> source_init_list_obj({"97", "98", "99"});

  ekuvector<int32_t> uut_pod;
  ekuvector<std::string> uut_obj;

  ASSERT_EQ(0, uut_pod.size());
  ASSERT_EQ(0, uut_obj.size());

  uut_pod = std::move(source_init_list_pod);
  uut_obj = std::move(source_init_list_obj);

  ASSERT_EQ(3, uut_pod.size());
  EXPECT_EQ(97, uut_pod[0]);
  EXPECT_EQ(98, uut_pod[1]);
  EXPECT_EQ(99, uut_pod[2]);

  ASSERT_EQ(3, uut_obj.size());
  EXPECT_EQ("97", uut_obj[0]);
  EXPECT_EQ("98", uut_obj[1]);
  EXPECT_EQ("99", uut_obj[2]);
}

class AssignMethodTests : public EkuVectorTests {};

TEST_F(AssignMethodTests, AssignValueSmallerThanOriginal) {
  ekuvector<int32_t> uut_pod({97, 98, 99});
  ekuvector<std::string> uut_obj({"97", "98", "99"});

  uut_pod.assign(2, 42);
  uut_obj.assign(2, "42");

  ASSERT_EQ(3, uut_pod.size());
  EXPECT_EQ(42, uut_pod[0]);
  EXPECT_EQ(42, uut_pod[1]);
  EXPECT_EQ(99, uut_pod[2]);

  ASSERT_EQ(3, uut_obj.size());
  EXPECT_EQ("42", uut_obj[0]);
  EXPECT_EQ("42", uut_obj[1]);
  EXPECT_EQ("99", uut_obj[2]);
}

TEST_F(AssignMethodTests, AssignValueLargerThanOriginal) {
  ekuvector<int32_t> uut_pod({97, 98, 99});
  ekuvector<std::string> uut_obj({"97", "98", "99"});

  uut_pod.assign(5, 42);
  uut_obj.assign(5, "42");

  ASSERT_EQ(5, uut_pod.size());
  EXPECT_EQ(42, uut_pod[0]);
  EXPECT_EQ(42, uut_pod[1]);
  EXPECT_EQ(42, uut_pod[2]);
  EXPECT_EQ(42, uut_pod[3]);
  EXPECT_EQ(42, uut_pod[4]);

  ASSERT_EQ(5, uut_obj.size());
  EXPECT_EQ("42", uut_obj[0]);
  EXPECT_EQ("42", uut_obj[1]);
  EXPECT_EQ("42", uut_obj[2]);
  EXPECT_EQ("42", uut_obj[3]);
  EXPECT_EQ("42", uut_obj[4]);
}

TEST_F(AssignMethodTests, AssignWithIteratorsSmallerThanOriginal) {
  std::vector<int32_t> std_vector_source_pod(3, 42);
  std::vector<std::string> std_vector_source_obj(3, "42");

  ekuvector<int32_t> uut_pod({97, 98, 99});
  ekuvector<std::string> uut_obj({"97", "98", "99"});

  uut_pod.assign(std_vector_source_pod.begin(), std_vector_source_pod.end());
  uut_obj.assign(std_vector_source_obj.begin(), std_vector_source_obj.end());

  ASSERT_EQ(3, uut_pod.size());
  EXPECT_EQ(42, uut_pod[0]);
  EXPECT_EQ(42, uut_pod[1]);
  EXPECT_EQ(42, uut_pod[2]);

  ASSERT_EQ(3, uut_obj.size());
  EXPECT_EQ("42", uut_obj[0]);
  EXPECT_EQ("42", uut_obj[1]);
  EXPECT_EQ("42", uut_obj[2]);
}

TEST_F(AssignMethodTests, AssignWithIteratorsLargerThanOriginal) {
  std::vector<int32_t> std_vector_source_pod(5, 42);
  std::vector<std::string> std_vector_source_obj(5, "42");

  ekuvector<int32_t> uut_pod({97, 98, 99});
  ekuvector<std::string> uut_obj({"97", "98", "99"});

  uut_pod.assign(std_vector_source_pod.begin(), std_vector_source_pod.end());
  uut_obj.assign(std_vector_source_obj.begin(), std_vector_source_obj.end());

  ASSERT_EQ(5, uut_pod.size());
  EXPECT_EQ(42, uut_pod[0]);
  EXPECT_EQ(42, uut_pod[1]);
  EXPECT_EQ(42, uut_pod[2]);
  EXPECT_EQ(42, uut_pod[3]);
  EXPECT_EQ(42, uut_pod[4]);

  ASSERT_EQ(5, uut_obj.size());
  EXPECT_EQ("42", uut_obj[0]);
  EXPECT_EQ("42", uut_obj[1]);
  EXPECT_EQ("42", uut_obj[2]);
  EXPECT_EQ("42", uut_obj[3]);
  EXPECT_EQ("42", uut_obj[4]);
}

TEST_F(AssignMethodTests, AssignWithInitListSmallerThanOriginal) {
  std::initializer_list<int32_t> std_init_list_source_pod({42, 42, 42});
  std::initializer_list<std::string> std_init_list_source_obj(
      {"42", "42", "42"});

  ekuvector<int32_t> uut_pod({97, 98, 99});
  ekuvector<std::string> uut_obj({"97", "98", "99"});

  uut_pod.assign(std_init_list_source_pod);
  uut_obj.assign(std_init_list_source_obj);

  ASSERT_EQ(3, uut_pod.size());
  EXPECT_EQ(42, uut_pod[0]);
  EXPECT_EQ(42, uut_pod[1]);
  EXPECT_EQ(42, uut_pod[2]);

  ASSERT_EQ(3, uut_obj.size());
  EXPECT_EQ("42", uut_obj[0]);
  EXPECT_EQ("42", uut_obj[1]);
  EXPECT_EQ("42", uut_obj[2]);
}

TEST_F(AssignMethodTests, AssignWithInitListLargerThanOriginal) {
  std::initializer_list<int32_t> std_init_list_source_pod({42, 42, 42, 42, 42});
  std::initializer_list<std::string> std_init_list_source_obj(
      {"42", "42", "42", "42", "42"});

  ekuvector<int32_t> uut_pod({97, 98, 99});
  ekuvector<std::string> uut_obj({"97", "98", "99"});

  uut_pod.assign(std_init_list_source_pod);
  uut_obj.assign(std_init_list_source_obj);

  ASSERT_EQ(5, uut_pod.size());
  EXPECT_EQ(42, uut_pod[0]);
  EXPECT_EQ(42, uut_pod[1]);
  EXPECT_EQ(42, uut_pod[2]);
  EXPECT_EQ(42, uut_pod[3]);
  EXPECT_EQ(42, uut_pod[4]);

  ASSERT_EQ(5, uut_obj.size());
  EXPECT_EQ("42", uut_obj[0]);
  EXPECT_EQ("42", uut_obj[1]);
  EXPECT_EQ("42", uut_obj[2]);
  EXPECT_EQ("42", uut_obj[3]);
  EXPECT_EQ("42", uut_obj[4]);
}

class ElementAccessTests : public EkuVectorTests {};

TEST_F(ElementAccessTests, AtMemberNonConst) {
  ekuvector<int32_t> uut_pod({97, 98, 99});
  ekuvector<std::string> uut_obj({"97", "98", "99"});

  EXPECT_EQ(97, uut_pod.at(0));
  EXPECT_EQ(98, uut_pod.at(1));
  EXPECT_EQ(99, uut_pod.at(2));
  EXPECT_THROW(uut_pod.at(3), std::out_of_range);
  EXPECT_THROW(uut_pod.at(-1), std::out_of_range);

  uut_pod.at(1) = 42;
  EXPECT_EQ(42, uut_pod.at(1));

  EXPECT_EQ("97", uut_obj.at(0));
  EXPECT_EQ("98", uut_obj.at(1));
  EXPECT_EQ("99", uut_obj.at(2));
  EXPECT_THROW(uut_obj.at(3), std::out_of_range);
  EXPECT_THROW(uut_obj.at(-1), std::out_of_range);

  uut_obj.at(1) = "42";
  EXPECT_EQ("42", uut_obj.at(1));
}

TEST_F(ElementAccessTests, AtMemberWhenConst) {
  const ekuvector<int32_t> uut_pod({97, 98, 99});
  const ekuvector<std::string> uut_obj({"97", "98", "99"});

  EXPECT_EQ(97, uut_pod.at(0));
  EXPECT_EQ(98, uut_pod.at(1));
  EXPECT_EQ(99, uut_pod.at(2));
  EXPECT_THROW(uut_pod.at(3), std::out_of_range);
  EXPECT_THROW(uut_pod.at(-1), std::out_of_range);

  EXPECT_EQ("97", uut_obj.at(0));
  EXPECT_EQ("98", uut_obj.at(1));
  EXPECT_EQ("99", uut_obj.at(2));
  EXPECT_THROW(uut_obj.at(3), std::out_of_range);
  EXPECT_THROW(uut_obj.at(-1), std::out_of_range);
}

TEST_F(ElementAccessTests, FrontAndBackNonConst) {
  ekuvector<int32_t> uut_pod({97, 98, 99});
  ekuvector<std::string> uut_obj({"97", "98", "99"});

  EXPECT_EQ(97, uut_pod.front());
  EXPECT_EQ(99, uut_pod.back());
  uut_pod.front() = 42;
  EXPECT_EQ(42, uut_pod.front());
  EXPECT_EQ(99, uut_pod.back());
  uut_pod.back() = 42;
  EXPECT_EQ(42, uut_pod.front());
  EXPECT_EQ(42, uut_pod.back());

  EXPECT_EQ("97", uut_obj.front());
  EXPECT_EQ("99", uut_obj.back());
  uut_obj.front() = "42";
  EXPECT_EQ("42", uut_obj.front());
  EXPECT_EQ("99", uut_obj.back());
  uut_obj.back() = "42";
  EXPECT_EQ("42", uut_obj.front());
  EXPECT_EQ("42", uut_obj.back());
}

class IteratorTests : public EkuVectorTests {};

TEST_F(IteratorTests, IteratorMembersBegin) {
  ekuvector<int32_t> uut_pod({97, 98, 99});
  ekuvector<std::string> uut_obj({"97", "98", "99"});

  const ekuvector<int32_t> c_uut_pod(uut_pod);
  const ekuvector<std::string> c_uut_obj(uut_obj);

  EXPECT_EQ(97, *uut_pod.begin());
  EXPECT_EQ(97, *uut_pod.cbegin());
  EXPECT_EQ(97, *c_uut_pod.begin());
  EXPECT_EQ(97, *c_uut_pod.cbegin());

  *uut_pod.begin() = 42;
  EXPECT_EQ(42, *uut_pod.begin());

  EXPECT_EQ("97", *uut_obj.begin());
  EXPECT_EQ("97", *uut_obj.cbegin());
  EXPECT_EQ("97", *c_uut_obj.begin());
  EXPECT_EQ("97", *c_uut_obj.cbegin());

  *uut_obj.begin() = "42";
  EXPECT_EQ("42", *uut_obj.begin());
}

TEST_F(IteratorTests, IteratorMembersEnd) {
  ekuvector<int32_t> uut_pod({97, 98, 99});
  ekuvector<std::string> uut_obj({"97", "98", "99"});

  const ekuvector<int32_t> c_uut_pod(uut_pod);
  const ekuvector<std::string> c_uut_obj(uut_obj);

  EXPECT_EQ(99, *(uut_pod.end() - 1));
  EXPECT_EQ(99, *(uut_pod.cend() - 1));
  EXPECT_EQ(99, *(c_uut_pod.end() - 1));
  EXPECT_EQ(99, *(c_uut_pod.cend() - 1));

  *(uut_pod.end() - 1) = 42;
  EXPECT_EQ(42, *(uut_pod.end() - 1));

  EXPECT_EQ("99", *(uut_obj.end() - 1));
  EXPECT_EQ("99", *(uut_obj.cend() - 1));
  EXPECT_EQ("99", *(c_uut_obj.end() - 1));
  EXPECT_EQ("99", *(c_uut_obj.cend() - 1));

  *(uut_obj.end() - 1) = "42";
  EXPECT_EQ("42", *(uut_obj.end() - 1));
}

TEST_F(IteratorTests, IteratorMembersReverseBegin) {
  ekuvector<int32_t> uut_pod({97, 98, 99});
  ekuvector<std::string> uut_obj({"97", "98", "99"});

  const ekuvector<int32_t> c_uut_pod(uut_pod);
  const ekuvector<std::string> c_uut_obj(uut_obj);

  EXPECT_EQ(99, *uut_pod.rbegin());
  EXPECT_EQ(99, *uut_pod.crbegin());
  EXPECT_EQ(99, *c_uut_pod.rbegin());
  EXPECT_EQ(99, *c_uut_pod.crbegin());

  *uut_pod.rbegin() = 42;
  EXPECT_EQ(42, *uut_pod.rbegin());

  EXPECT_EQ("99", *uut_obj.rbegin());
  EXPECT_EQ("99", *uut_obj.crbegin());
  EXPECT_EQ("99", *c_uut_obj.rbegin());
  EXPECT_EQ("99", *c_uut_obj.crbegin());

  *uut_obj.rbegin() = "42";
  EXPECT_EQ("42", *uut_obj.rbegin());
}

TEST_F(IteratorTests, IteratorMembersReverseEnd) {
  ekuvector<int32_t> uut_pod({97, 98, 99});
  ekuvector<std::string> uut_obj({"97", "98", "99"});

  const ekuvector<int32_t> c_uut_pod(uut_pod);
  const ekuvector<std::string> c_uut_obj(uut_obj);

  EXPECT_EQ(97, *(uut_pod.rend() - 1));
  EXPECT_EQ(97, *(uut_pod.crend() - 1));
  EXPECT_EQ(97, *(c_uut_pod.rend() - 1));
  EXPECT_EQ(97, *(c_uut_pod.crend() - 1));

  *(uut_pod.rend() - 1) = 42;
  EXPECT_EQ(42, *(uut_pod.rend() - 1));

  EXPECT_EQ("97", *(uut_obj.rend() - 1));
  EXPECT_EQ("97", *(uut_obj.crend() - 1));
  EXPECT_EQ("97", *(c_uut_obj.rend() - 1));
  EXPECT_EQ("97", *(c_uut_obj.crend() - 1));

  *(uut_obj.rend() - 1) = "42";
  EXPECT_EQ("42", *(uut_obj.rend() - 1));
}

TEST_F(IteratorTests, MovingForwardRangeFor) {
  {
    ekuvector<int32_t> uut_pod({97, 98, 99});
    std::vector<int32_t> output_sequence;
    for (auto &value : uut_pod) {
      output_sequence.push_back(value);
    }
    EXPECT_EQ(std::vector<int32_t>({97, 98, 99}), output_sequence);
  }
  {
    ekuvector<std::string> uut_obj({"97", "98", "99"});
    std::vector<std::string> output_sequence;
    for (auto &value : uut_obj) {
      output_sequence.push_back(value);
    }
    EXPECT_EQ(std::vector<std::string>({"97", "98", "99"}), output_sequence);
  }
}

TEST_F(IteratorTests, MovingForwardClassicFor) {
  {
    ekuvector<int32_t> uut_pod({97, 98, 99});
    std::vector<int32_t> output_sequence;
    for (auto it = uut_pod.begin(); it != uut_pod.end(); ++it) {
      output_sequence.push_back(*it);
    }
    EXPECT_EQ(std::vector<int32_t>({97, 98, 99}), output_sequence);
  }
  {
    ekuvector<std::string> uut_obj({"97", "98", "99"});
    std::vector<std::string> output_sequence;
    for (auto it = uut_obj.begin(); it != uut_obj.end(); ++it) {
      output_sequence.push_back(*it);
    }
    EXPECT_EQ(std::vector<std::string>({"97", "98", "99"}), output_sequence);
  }
}

TEST_F(IteratorTests, MovingForwardClassicForConstIterator) {
  {
    ekuvector<int32_t> uut_pod({97, 98, 99});
    std::vector<int32_t> output_sequence;
    for (auto it = uut_pod.cbegin(); it != uut_pod.cend(); ++it) {
      output_sequence.push_back(*it);
    }
    EXPECT_EQ(std::vector<int32_t>({97, 98, 99}), output_sequence);
  }
  {
    ekuvector<std::string> uut_obj({"97", "98", "99"});
    std::vector<std::string> output_sequence;
    for (auto it = uut_obj.cbegin(); it != uut_obj.cend(); ++it) {
      output_sequence.push_back(*it);
    }
    EXPECT_EQ(std::vector<std::string>({"97", "98", "99"}), output_sequence);
  }
}

TEST_F(IteratorTests, MovingBackwardClassicForConstIterator) {
  {
    ekuvector<int32_t> uut_pod({97, 98, 99});
    std::vector<int32_t> output_sequence;
    for (auto it = uut_pod.crbegin(); it != uut_pod.crend(); ++it) {
      output_sequence.push_back(*it);
    }
    EXPECT_EQ(std::vector<int32_t>({99, 98, 97}), output_sequence);
  }
  {
    ekuvector<std::string> uut_obj({"97", "98", "99"});
    std::vector<std::string> output_sequence;
    for (auto it = uut_obj.crbegin(); it != uut_obj.crend(); ++it) {
      output_sequence.push_back(*it);
    }
    EXPECT_EQ(std::vector<std::string>({"99", "98", "97"}), output_sequence);
  }
}

TEST_F(IteratorTests, MovingForwardClassicForConstSource) {
  {
    const ekuvector<int32_t> uut_pod({97, 98, 99});
    std::vector<int32_t> output_sequence;
    for (auto it = uut_pod.begin(); it != uut_pod.end(); ++it) {
      output_sequence.push_back(*it);
    }
    EXPECT_EQ(std::vector<int32_t>({97, 98, 99}), output_sequence);
  }
  {
    const ekuvector<std::string> uut_obj({"97", "98", "99"});
    std::vector<std::string> output_sequence;
    for (auto it = uut_obj.begin(); it != uut_obj.end(); ++it) {
      output_sequence.push_back(*it);
    }
    EXPECT_EQ(std::vector<std::string>({"97", "98", "99"}), output_sequence);
  }
}

TEST_F(IteratorTests, MovingBackwardClassicForConstSource) {
  {
    const ekuvector<int32_t> uut_pod({97, 98, 99});
    std::vector<int32_t> output_sequence;
    for (auto it = uut_pod.rbegin(); it != uut_pod.rend(); ++it) {
      output_sequence.push_back(*it);
    }
    EXPECT_EQ(std::vector<int32_t>({99, 98, 97}), output_sequence);
  }
  {
    const ekuvector<std::string> uut_obj({"97", "98", "99"});
    std::vector<std::string> output_sequence;
    for (auto it = uut_obj.rbegin(); it != uut_obj.rend(); ++it) {
      output_sequence.push_back(*it);
    }
    EXPECT_EQ(std::vector<std::string>({"99", "98", "97"}), output_sequence);
  }
}

class StorageManagementTests : public EkuVectorTests {};

TEST_F(StorageManagementTests, EmptyAndSizeMembers) {
  {
    const ekuvector<int32_t> uut_pod;
    const ekuvector<std::string> uut_obj;

    EXPECT_TRUE(uut_pod.empty());
    EXPECT_EQ(0, uut_pod.size());
  }
  {
    const ekuvector<int32_t> uut_pod({97, 98, 99});
    const ekuvector<std::string> uut_obj({"97", "98", "99"});

    EXPECT_FALSE(uut_pod.empty());
    EXPECT_EQ(3, uut_pod.size());
  }
}

TEST_F(StorageManagementTests, DataMemberNonConst) {
  ekuvector<int32_t> uut_pod;
  ekuvector<std::string> uut_obj;

  EXPECT_EQ(nullptr, uut_pod.data());
  EXPECT_EQ(nullptr, uut_obj.data());

  uut_pod = {97, 98, 99};
  uut_obj = {"97", "98", "99"};

  EXPECT_NE(nullptr, uut_pod.data());
  EXPECT_NE(nullptr, uut_obj.data());
}

TEST_F(StorageManagementTests, CapacityAndReserveMembers) {
  {
    ekuvector<int32_t> uut;

    auto capacity_0 = uut.capacity();
    // increase capacity
    uut.reserve(uut.capacity() + 1);
    auto capacity_1 = uut.capacity();
    // capacity should stay the same
    uut.reserve(uut.capacity() - 1);
    auto capacity_2 = uut.capacity();
    // increase capacity again
    uut.reserve(uut.capacity() + 1);
    auto capacity_3 = uut.capacity();

    EXPECT_TRUE(uut.empty());
    EXPECT_EQ(0, uut.size());
    EXPECT_TRUE(capacity_1 > capacity_0);
    EXPECT_TRUE(capacity_2 == capacity_1);
    EXPECT_TRUE(capacity_3 > capacity_2);
  };

  {
    ekuvector<std::string> uut;

    auto capacity_0 = uut.capacity();
    // increase capacity
    uut.reserve(uut.capacity() + 1);
    auto capacity_1 = uut.capacity();
    // capacity should stay the same
    uut.reserve(uut.capacity() - 1);
    auto capacity_2 = uut.capacity();
    // increase capacity again
    uut.reserve(uut.capacity() + 1);
    auto capacity_3 = uut.capacity();

    EXPECT_TRUE(uut.empty());
    EXPECT_EQ(0, uut.size());
    EXPECT_TRUE(capacity_1 > capacity_0);
    EXPECT_TRUE(capacity_2 == capacity_1);
    EXPECT_TRUE(capacity_3 > capacity_2);
  };
}

TEST_F(StorageManagementTests, ClearOperation) {
  {
    ekuvector<int32_t> uut({97, 98, 99});
    EXPECT_FALSE(uut.empty());
    EXPECT_EQ(3, uut.size());
    uut.clear();
    EXPECT_TRUE(uut.empty());
    EXPECT_EQ(0, uut.size());
  };

  {
    ekuvector<std::string> uut({"97", "98", "99"});
    EXPECT_FALSE(uut.empty());
    EXPECT_EQ(3, uut.size());
    uut.clear();
    EXPECT_TRUE(uut.empty());
    EXPECT_EQ(0, uut.size());
  };
}

class InsertTests : public EkuVectorTests {};

TEST_F(InsertTests, CopyInsert) {
  {
    const ekuvector<int32_t> expected{1, 2, 3, 4};
    ekuvector<typename decltype(expected)::type> uut;
    typename decltype(uut)::type var;

    // insert on an empty vector
    var = expected[1];
    uut.insert(uut.begin(), var);
    // insert at the start
    var = expected[0];
    uut.insert(uut.begin(), var);
    // insert at the end
    var = expected[3];
    uut.insert(uut.end(), var);
    // insert somewhere in the middle
    var = expected[2];
    uut.insert(uut.begin() + 2, var);

    EXPECT_EQ(expected, uut);
  }

  {
    const ekuvector<std::string> expected{"1", "2", "3", "4"};
    ekuvector<typename decltype(expected)::type> uut;
    typename decltype(uut)::type var;

    // insert on an empty vector
    var = expected[1];
    uut.insert(uut.begin(), var);
    // insert at the start
    var = expected[0];
    uut.insert(uut.begin(), var);
    // insert at the end
    var = expected[3];
    uut.insert(uut.end(), var);
    // insert somewhere in the middle
    var = expected[2];
    uut.insert(uut.begin() + 2, var);

    EXPECT_EQ(expected, uut);
  }

  {
    ekuvector<IChar> uut;
    IChar var;
    IChar::reset();
    uut.insert(uut.begin(), var);

    EXPECT_EQ(0, IChar::value_constructor_);
    EXPECT_EQ(1, IChar::copy_ops_);
    EXPECT_EQ(0, IChar::move_ops_);
  }
}

TEST_F(InsertTests, MoveInsert) {
  {
    const ekuvector<int32_t> expected{1, 2, 3, 4};
    ekuvector<typename decltype(expected)::type> uut;
    typename decltype(uut)::type var;

    // insert on an empty vector
    var = expected[1];
    uut.insert(uut.begin(), std::move(var));
    // insert at the start
    var = expected[0];
    uut.insert(uut.begin(), std::move(var));
    // insert at the end
    var = expected[3];
    uut.insert(uut.end(), std::move(var));
    // insert somewhere in the middle
    var = expected[2];
    uut.insert(uut.begin() + 2, std::move(var));

    EXPECT_EQ(expected, uut);
  }

  {
    const ekuvector<std::string> expected{"1", "2", "3", "4"};
    ekuvector<typename decltype(expected)::type> uut;
    typename decltype(uut)::type var;

    // insert on an empty vector
    var = expected[1];
    uut.insert(uut.begin(), std::move(var));
    // insert at the start
    var = expected[0];
    uut.insert(uut.begin(), std::move(var));
    // insert at the end
    var = expected[3];
    uut.insert(uut.end(), std::move(var));
    // insert somewhere in the middle
    var = expected[2];
    uut.insert(uut.begin() + 2, std::move(var));

    EXPECT_EQ(expected, uut);
  }

  {
    ekuvector<IChar> uut;
    IChar var;
    IChar::reset();
    uut.insert(uut.begin(), std::move(var));

    EXPECT_EQ(0, IChar::value_constructor_);
    EXPECT_EQ(0, IChar::copy_ops_);
    EXPECT_EQ(1, IChar::move_ops_);
  }
}

TEST_F(InsertTests, MultipleInsert) {
  {
    ekuvector<int32_t> uut_empty;
    ekuvector<int32_t> uut_before({97, 98});
    ekuvector<int32_t> uut_middle({97, 98});
    ekuvector<int32_t> uut_after({97, 98});

    uut_empty.insert(uut_empty.begin(), 2, 42);
    uut_before.insert(uut_before.begin(), 2, 42);
    uut_middle.insert(uut_middle.begin() + 1, 2, 42);
    uut_after.insert(uut_after.end(), 2, 42);

    EXPECT_EQ(ekuvector<int32_t>({42, 42}), uut_empty);
    EXPECT_EQ(ekuvector<int32_t>({42, 42, 97, 98}), uut_before);
    EXPECT_EQ(ekuvector<int32_t>({97, 42, 42, 98}), uut_middle);
    EXPECT_EQ(ekuvector<int32_t>({97, 98, 42, 42}), uut_after);
  };

  {
    ekuvector<std::string> uut_empty;
    ekuvector<std::string> uut_before({"97", "98"});
    ekuvector<std::string> uut_middle({"97", "98"});
    ekuvector<std::string> uut_after({"97", "98"});

    uut_empty.insert(uut_empty.begin(), 2, "42");
    uut_before.insert(uut_before.begin(), 2, "42");
    uut_middle.insert(uut_middle.begin() + 1, 2, "42");
    uut_after.insert(uut_after.end(), 2, "42");

    EXPECT_EQ(ekuvector<std::string>({"42", "42"}), uut_empty);
    EXPECT_EQ(ekuvector<std::string>({"42", "42", "97", "98"}), uut_before);
    EXPECT_EQ(ekuvector<std::string>({"97", "42", "42", "98"}), uut_middle);
    EXPECT_EQ(ekuvector<std::string>({"97", "98", "42", "42"}), uut_after);
  };
}

TEST_F(InsertTests, InsertThroughIterators) {
  {
    const std::vector<int32_t> sub_seq(2, 42);

    ekuvector<int32_t> uut_empty;
    ekuvector<int32_t> uut_before({97, 98});
    ekuvector<int32_t> uut_middle({97, 98});
    ekuvector<int32_t> uut_after({97, 98});

    uut_empty.insert(uut_empty.begin(), sub_seq.begin(), sub_seq.end());
    uut_before.insert(uut_before.begin(), sub_seq.begin(), sub_seq.end());
    uut_middle.insert(uut_middle.begin() + 1, sub_seq.begin(), sub_seq.end());
    uut_after.insert(uut_after.end(), sub_seq.begin(), sub_seq.end());

    EXPECT_EQ(ekuvector<int32_t>({42, 42}), uut_empty);
    EXPECT_EQ(ekuvector<int32_t>({42, 42, 97, 98}), uut_before);
    EXPECT_EQ(ekuvector<int32_t>({97, 42, 42, 98}), uut_middle);
    EXPECT_EQ(ekuvector<int32_t>({97, 98, 42, 42}), uut_after);
  };

  {
    const std::vector<std::string> sub_seq(2, "42");

    ekuvector<std::string> uut_empty;
    ekuvector<std::string> uut_before({"97", "98"});
    ekuvector<std::string> uut_middle({"97", "98"});
    ekuvector<std::string> uut_after({"97", "98"});

    uut_empty.insert(uut_empty.begin(), sub_seq.begin(), sub_seq.end());
    uut_before.insert(uut_before.begin(), sub_seq.begin(), sub_seq.end());
    uut_middle.insert(uut_middle.begin() + 1, sub_seq.begin(), sub_seq.end());
    uut_after.insert(uut_after.end(), sub_seq.begin(), sub_seq.end());

    EXPECT_EQ(ekuvector<std::string>({"42", "42"}), uut_empty);
    EXPECT_EQ(ekuvector<std::string>({"42", "42", "97", "98"}), uut_before);
    EXPECT_EQ(ekuvector<std::string>({"97", "42", "42", "98"}), uut_middle);
    EXPECT_EQ(ekuvector<std::string>({"97", "98", "42", "42"}), uut_after);
  };
}

TEST_F(InsertTests, InsertFromInitializerList) {
  {
    const std::initializer_list<int32_t> sub_seq = {42, 42};

    ekuvector<int32_t> uut_empty;
    ekuvector<int32_t> uut_before({97, 98});
    ekuvector<int32_t> uut_middle({97, 98});
    ekuvector<int32_t> uut_after({97, 98});

    uut_empty.insert(uut_empty.begin(), sub_seq);
    uut_before.insert(uut_before.begin(), sub_seq);
    uut_middle.insert(uut_middle.begin() + 1, sub_seq);
    uut_after.insert(uut_after.end(), sub_seq);

    EXPECT_EQ(ekuvector<int32_t>({42, 42}), uut_empty);
    EXPECT_EQ(ekuvector<int32_t>({42, 42, 97, 98}), uut_before);
    EXPECT_EQ(ekuvector<int32_t>({97, 42, 42, 98}), uut_middle);
    EXPECT_EQ(ekuvector<int32_t>({97, 98, 42, 42}), uut_after);
  };

  {
    const std::initializer_list<std::string> sub_seq = {"42", "42"};

    ekuvector<std::string> uut_empty;
    ekuvector<std::string> uut_before({"97", "98"});
    ekuvector<std::string> uut_middle({"97", "98"});
    ekuvector<std::string> uut_after({"97", "98"});

    uut_empty.insert(uut_empty.begin(), sub_seq);
    uut_before.insert(uut_before.begin(), sub_seq);
    uut_middle.insert(uut_middle.begin() + 1, sub_seq);
    uut_after.insert(uut_after.end(), sub_seq);

    EXPECT_EQ(ekuvector<std::string>({"42", "42"}), uut_empty);
    EXPECT_EQ(ekuvector<std::string>({"42", "42", "97", "98"}), uut_before);
    EXPECT_EQ(ekuvector<std::string>({"97", "42", "42", "98"}), uut_middle);
    EXPECT_EQ(ekuvector<std::string>({"97", "98", "42", "42"}), uut_after);
  };
}

class EraseTests : public EkuVectorTests {};

TEST_F(EraseTests, EraseAtPos) {
  {
    const ekuvector<int32_t> uut({96, 97, 98});
    auto uut_begin = uut;
    auto uut_middle = uut;
    auto uut_end = uut;

    uut_begin.erase(uut_begin.begin());
    uut_middle.erase(uut_middle.begin() + 1);
    uut_end.erase(uut_end.begin() + 2);

    EXPECT_EQ(ekuvector<int32_t>({97, 98}), uut_begin);
    EXPECT_EQ(ekuvector<int32_t>({96, 98}), uut_middle);
    EXPECT_EQ(ekuvector<int32_t>({96, 97}), uut_end);
  };

  {
    const ekuvector<std::string> uut({"96", "97", "98"});
    auto uut_begin = uut;
    auto uut_middle = uut;
    auto uut_end = uut;

    uut_begin.erase(uut_begin.begin());
    uut_middle.erase(uut_middle.begin() + 1);
    uut_end.erase(uut_end.begin() + 2);

    EXPECT_EQ(ekuvector<std::string>({"97", "98"}), uut_begin);
    EXPECT_EQ(ekuvector<std::string>({"96", "98"}), uut_middle);
    EXPECT_EQ(ekuvector<std::string>({"96", "97"}), uut_end);
  };
}

TEST_F(EraseTests, EraseWithIterators) {
  {
    const ekuvector<int32_t> uut({96, 97, 98});
    auto uut_begin = uut;
    auto uut_middle = uut;
    auto uut_end = uut;

    uut_begin.erase(uut_begin.begin(), uut_begin.begin() + 2);
    uut_middle.erase(uut_middle.end() - 2, uut_middle.end());
    uut_end.erase(uut_end.begin(), uut_end.end());

    EXPECT_EQ(ekuvector<int32_t>({98}), uut_begin);
    EXPECT_EQ(ekuvector<int32_t>({96}), uut_middle);
    EXPECT_EQ(ekuvector<int32_t>{}, uut_end);
  };

  {
    const ekuvector<std::string> uut({"96", "97", "98"});
    auto uut_begin = uut;
    auto uut_middle = uut;
    auto uut_end = uut;

    uut_begin.erase(uut_begin.begin(), uut_begin.begin() + 2);
    uut_middle.erase(uut_middle.end() - 2, uut_middle.end());
    uut_end.erase(uut_end.begin(), uut_end.end());

    EXPECT_EQ(ekuvector<std::string>({"98"}), uut_begin);
    EXPECT_EQ(ekuvector<std::string>({"96"}), uut_middle);
    EXPECT_EQ(ekuvector<std::string>{}, uut_end);
  };
}

class PushPopTests : public EkuVectorTests {};

TEST_F(PushPopTests, CopyPushBack) {
  {
    ekuvector<int32_t> uut;
    uut.push_back(97);
    uut.push_back(98);
    uut.push_back(99);
    EXPECT_EQ(ekuvector<int32_t>({97, 98, 99}), uut);
  }

  {
    ekuvector<std::string> uut;
    uut.push_back("97");
    uut.push_back("98");
    uut.push_back("99");
    EXPECT_EQ(ekuvector<std::string>({"97", "98", "99"}), uut);
  }

  {
    ekuvector<IChar> uut;
    IChar var;
    IChar::reset();
    uut.push_back(var);
    EXPECT_EQ(0, IChar::value_constructor_);
    EXPECT_EQ(1, IChar::copy_ops_);
    EXPECT_EQ(0, IChar::move_ops_);
  }
}

TEST_F(PushPopTests, MovePushBack) {
  {
    ekuvector<int32_t> uut;
    int32_t value;
    value = 97;
    uut.push_back(std::move(value));
    value = 98;
    uut.push_back(std::move(value));
    value = 99;
    uut.push_back(std::move(value));
    EXPECT_EQ(ekuvector<int32_t>({97, 98, 99}), uut);
  }

  {
    ekuvector<std::string> uut;
    std::string value;
    value = "97";
    uut.push_back(std::move(value));
    value = "98";
    uut.push_back(std::move(value));
    value = "99";
    uut.push_back(std::move(value));
    EXPECT_EQ(ekuvector<std::string>({"97", "98", "99"}), uut);
  }

  {
    ekuvector<IChar> uut;
    IChar var;
    IChar::reset();
    uut.push_back(std::move(var));
    EXPECT_EQ(0, IChar::value_constructor_);
    EXPECT_EQ(0, IChar::copy_ops_);
    EXPECT_EQ(1, IChar::move_ops_);
  }
}

TEST_F(PushPopTests, PopBack) {
  {
    ekuvector<int32_t> uut{97, 98, 99};
    uut.pop_back();
    EXPECT_EQ(ekuvector<int32_t>({97, 98}), uut);
    uut.pop_back();
    EXPECT_EQ(ekuvector<int32_t>({97}), uut);
    uut.pop_back();
    EXPECT_EQ(ekuvector<int32_t>{}, uut);
  };

  {
    ekuvector<std::string> uut{"97", "98", "99"};
    uut.pop_back();
    EXPECT_EQ(ekuvector<std::string>({"97", "98"}), uut);
    uut.pop_back();
    EXPECT_EQ(ekuvector<std::string>({"97"}), uut);
    uut.pop_back();
    EXPECT_EQ(ekuvector<std::string>{}, uut);
  };
}

TEST_F(StorageManagementTests, ResizeWithDefaultConstructor) {
  {
    ekuvector<int32_t> uut;
    EXPECT_EQ(0, uut.size());
    uut.resize(10);
    EXPECT_EQ(10, uut.size());
    uut.resize(10);
    EXPECT_EQ(10, uut.size());
    uut.resize(20);
    EXPECT_EQ(20, uut.size());
    uut.resize(0);
    EXPECT_EQ(0, uut.size());
  };

  {
    ekuvector<std::string> uut;
    EXPECT_EQ(0, uut.size());
    uut.resize(10);
    EXPECT_EQ(10, uut.size());
    uut.resize(10);
    EXPECT_EQ(10, uut.size());
    uut.resize(20);
    EXPECT_EQ(20, uut.size());
    uut.resize(0);
    EXPECT_EQ(0, uut.size());
  };
}

TEST_F(StorageManagementTests, ResizeWithCopyConstructor) {
  {
    ekuvector<int32_t> uut;
    EXPECT_EQ(0, uut.size());
    uut.resize(10, 42);
    EXPECT_EQ(10, uut.size());
    uut.resize(10, 42);
    EXPECT_EQ(10, uut.size());
    uut.resize(20, 42);
    EXPECT_EQ(20, uut.size());
    uut.resize(0, 42);
    EXPECT_EQ(0, uut.size());
  };

  {
    ekuvector<std::string> uut;
    EXPECT_EQ(0, uut.size());
    uut.resize(10, "42");
    EXPECT_EQ(10, uut.size());
    uut.resize(10, "42");
    EXPECT_EQ(10, uut.size());
    uut.resize(20, "42");
    EXPECT_EQ(20, uut.size());
    uut.resize(0, "42");
    EXPECT_EQ(0, uut.size());
  };
}

TEST_F(StorageManagementTests, Swap) {
  {
    const ekuvector<int32_t> uut_org_a{97, 98, 99};
    const ekuvector<int32_t> uut_org_b{42, 42};
    ekuvector<int32_t> uut_a(uut_org_a);
    ekuvector<int32_t> uut_b(uut_org_b);
    uut_a.swap(uut_b);
    EXPECT_EQ(uut_org_a, uut_b);
    EXPECT_EQ(uut_org_b, uut_a);
  };

  {
    const ekuvector<std::string> uut_org_a{"97", "98", "99"};
    const ekuvector<std::string> uut_org_b{"42", "42"};
    ekuvector<std::string> uut_a(uut_org_a);
    ekuvector<std::string> uut_b(uut_org_b);
    uut_a.swap(uut_b);
    EXPECT_EQ(uut_org_a, uut_b);
    EXPECT_EQ(uut_org_b, uut_a);
  };
}

class EmplaceTests : public EkuVectorTests {};

TEST_F(EmplaceTests, EmplaceAtPos) {
  {
    ekuvector<int32_t> uut;
    uut.emplace(uut.begin(), 2);
    uut.emplace(uut.begin(), 1);
    uut.emplace(uut.end(), 4);
    uut.emplace(uut.begin() + 2, 3);
    EXPECT_EQ(ekuvector<int32_t>({1, 2, 3, 4}), uut);
  };

  {
    ekuvector<std::string> uut;
    uut.emplace(uut.begin(), "2");
    uut.emplace(uut.begin(), "1");
    uut.emplace(uut.end(), "4");
    uut.emplace(uut.begin() + 2, "3");
    EXPECT_EQ(ekuvector<std::string>({"1", "2", "3", "4"}), uut);
  };
}

TEST_F(EmplaceTests, EmplaceBack) {
  {
    ekuvector<int32_t> uut;
    uut.emplace_back(97);
    uut.emplace_back(98);
    uut.emplace_back(99);
    EXPECT_EQ(ekuvector<int32_t>({97, 98, 99}), uut);
  };

  {
    ekuvector<std::string> uut;
    uut.emplace_back("97");
    uut.emplace_back("98");
    uut.emplace_back("99");
    EXPECT_EQ(ekuvector<std::string>({"97", "98", "99"}), uut);
  };
}

class OperatorTests : public EkuVectorTests {};

TEST_F(OperatorTests, EqualityOperators) {
  const ekuvector<int32_t> v_aa{97, 98, 99};
  const ekuvector<int32_t> v_ab{97, 98, 99};
  const ekuvector<int32_t> v_ba{42, 42, 42};
  const ekuvector<int32_t> v_bb{42, 42};
  const ekuvector<int32_t> v_ca;
  const ekuvector<int32_t> v_cb;

  EXPECT_TRUE(v_aa == v_aa);
  EXPECT_TRUE(v_aa == v_ab);
  EXPECT_TRUE(v_aa != v_ba);
  EXPECT_TRUE(v_aa != v_bb);
  EXPECT_TRUE(v_aa != v_ca);
  EXPECT_TRUE(v_aa != v_cb);

  EXPECT_TRUE(v_ab == v_ab);
  EXPECT_TRUE(v_ab != v_ba);
  EXPECT_TRUE(v_ab != v_bb);
  EXPECT_TRUE(v_ab != v_ca);
  EXPECT_TRUE(v_ab != v_cb);

  EXPECT_TRUE(v_ba == v_ba);
  EXPECT_TRUE(v_ba != v_bb);
  EXPECT_TRUE(v_ba != v_ca);
  EXPECT_TRUE(v_ba != v_cb);

  EXPECT_TRUE(v_bb == v_bb);
  EXPECT_TRUE(v_bb != v_ca);
  EXPECT_TRUE(v_bb != v_cb);

  EXPECT_TRUE(v_ca == v_ca);
  EXPECT_TRUE(v_ca == v_cb);

  EXPECT_TRUE(v_cb == v_cb);
}

TEST_F(OperatorTests, ComparisonOperators) {
  const ekuvector<int32_t> v_a{1, 2, 3};
  const ekuvector<int32_t> v_b{2, 1, 2};
  const ekuvector<int32_t> v_c{2, 2, 1};
  const ekuvector<int32_t> v_d{2, 2, 2};
  const ekuvector<int32_t> v_e;
  const ekuvector<int32_t> v_r{2, 2, 2};

  EXPECT_TRUE(v_a < v_r);
  EXPECT_FALSE(v_a >= v_r);
  EXPECT_TRUE(v_a <= v_r);
  EXPECT_FALSE(v_a > v_r);

  EXPECT_TRUE(v_b < v_r);
  EXPECT_FALSE(v_b >= v_r);
  EXPECT_TRUE(v_b <= v_r);
  EXPECT_FALSE(v_b > v_r);

  EXPECT_TRUE(v_c < v_r);
  EXPECT_FALSE(v_c >= v_r);
  EXPECT_TRUE(v_c <= v_r);
  EXPECT_FALSE(v_c > v_r);

  EXPECT_FALSE(v_d < v_r);
  EXPECT_TRUE(v_d >= v_r);
  EXPECT_TRUE(v_d <= v_r);
  EXPECT_FALSE(v_d > v_r);

  EXPECT_TRUE(v_e < v_r);
  EXPECT_FALSE(v_e >= v_r);
  EXPECT_TRUE(v_e <= v_r);
  EXPECT_FALSE(v_e > v_r);
}

}; // namespace ekustd
