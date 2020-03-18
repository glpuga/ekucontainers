/**
 * ekuvector, std::vector clone.
 * @author Gerardo Puga
 * */

// Standard library
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

// gtest and gmock
#include "gtest/gtest.h"

// Library
#include <ekuvector/ekuvector.hpp>

namespace ekustd {

class Animal {
public:
  virtual std::string talk() const { return "(sound)"; }
};

class Cat : public Animal {
public:
  std::string talk() const override { return "miau"; }
};

class Dog : public Animal {
public:
  std::string talk() const override { return "barf"; }
};

class Bird : public Animal {
public:
  Bird(const std::string &sound) : sound_{sound} {}
  std::string talk() const override { return sound_; }

private:
  std::string sound_;
};

class BaseWithVirtualDestructor {
public:
  virtual ~BaseWithVirtualDestructor() = default;
};

class DerivedWithVirtualDestructor : public BaseWithVirtualDestructor {
public:
  DerivedWithVirtualDestructor(const std::shared_ptr<std::string> &obj)
      : obj_{obj} {}
private:
  std::shared_ptr<std::string> obj_;
};

class BaseWithNoVirtualDestructor {};

class DerivedWithNoVirtualDestructor : public BaseWithNoVirtualDestructor {
public:
  DerivedWithNoVirtualDestructor(const std::shared_ptr<std::string> &obj)
      : obj_{obj} {}
private:
  std::shared_ptr<std::string> obj_;
};

class EkuVectorAppendix : public testing::Test {};

TEST_F(EkuVectorAppendix, SimpleInheritance) {
  Animal animal;
  Cat cat;
  Dog dog;

  EXPECT_EQ("(sound)", animal.talk());
  EXPECT_EQ("miau", cat.talk());
  EXPECT_EQ("barf", dog.talk());
}

TEST_F(EkuVectorAppendix, PolymorphismThroughReferences) {
  Animal animal;
  Cat cat;
  Dog dog;

  Animal &animal1 = animal;
  Animal &animal2 = cat;
  Animal &animal3 = dog;

  EXPECT_EQ("(sound)", animal1.talk());
  EXPECT_EQ("miau", animal2.talk());
  EXPECT_EQ("barf", animal3.talk());
}

TEST_F(EkuVectorAppendix, PolymorphismThroughPointers) {
  std::shared_ptr<Animal> animal;

  animal = std::make_shared<Cat>();
  EXPECT_EQ("miau", animal->talk());

  animal = std::make_shared<Dog>();
  EXPECT_EQ("barf", animal->talk());

  animal = std::make_shared<Animal>();
  EXPECT_EQ("(sound)", animal->talk());
}

TEST_F(EkuVectorAppendix, BewareOfTrivialSlicing) {
  Animal animal = Animal{};
  Animal cat = Cat{};
  Animal dog = Dog{};

  // this is ok
  EXPECT_EQ("(sound)", animal.talk());
  // this is not, slicing the reduces cats and dogs to plain animals
  EXPECT_EQ("(sound)", cat.talk());
  EXPECT_EQ("(sound)", dog.talk());
}

TEST_F(EkuVectorAppendix, WeirderSlicing) {
  Bird duck = Bird("cuack");
  Bird canary = Bird("piopi");

  Animal &animal = duck;
  // This is just fine, plain old polymorphism
  EXPECT_EQ("cuack", animal.talk());

  // This is wrong, the copy operation is not virtual,
  // causing the canary to cuack
  animal = canary;
  EXPECT_NE("piopi", animal.talk());
}

TEST_F(EkuVectorAppendix, DontForgetTheVirtualDestructor) {
  auto obj1 = std::make_shared<std::string>("obj1");
  auto obj2 = std::make_shared<std::string>("obj2");

  // These are currently the only references to each object
  EXPECT_EQ(1, obj1.use_count());
  EXPECT_EQ(1, obj2.use_count());

  {
    // A new copy of each object is now stored in a container
    // object, increasing their use count to two
    std::unique_ptr<BaseWithVirtualDestructor> base_ref_1 =
        std::make_unique<DerivedWithVirtualDestructor>(obj1);
    std::unique_ptr<BaseWithNoVirtualDestructor> base_ref_2 =
        std::make_unique<DerivedWithNoVirtualDestructor>(obj2);

    EXPECT_EQ(2, obj1.use_count());
    EXPECT_EQ(2, obj2.use_count());
  }

  // base_ref_1 and base_ref_2 are now out of scope, so only a single
  // reference to each object should remain. However, since one
  // of the classes is lacking a virtual destructor at the base class,
  // the use count of obj2 is now wrong, and we'll have a memory leak when
  // at the exit of the function
  EXPECT_EQ(1, obj1.use_count());
  EXPECT_EQ(2, obj2.use_count()); // this is WRONG
}

}; // namespace ekustd
