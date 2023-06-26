#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ExampleClass.hpp"
#include <TypeManager.hpp>

struct PODExample
{
    int a;
    int b;
    double c;
};

using PrimitiveTypes = meta::TypePack<
    TestType<int>,
    TestType<float>,
    TestType<double>>;

using PODType = meta::TypePack<
    TestType<PODExample>>;

using TestTypes = meta::concat_t<PrimitiveTypes, PODType>::unpack_t<GTESTBuilder>;

template<class T>
struct ExampleTest : public ::testing::Test
{
    ExampleTest ()
    {
    }

};

TYPED_TEST_SUITE_P(ExampleTest);

TYPED_TEST_P(ExampleTest, TEST1){
    typename TypeParam::type value {};
    typename TypeParam::builder {} (value, 1);
    ExampleClass<typename TypeParam::type> to_test {value};
    typename TypeParam::eq EQ {};
    ASSERT_TRUE(EQ(to_test.get_t(), value));

    typename TypeParam::builder{} (value, 2);
    ASSERT_FALSE(EQ(to_test.get_t(), value));
}

REGISTER_TYPED_TEST_SUITE_P(ExampleTest,
        TEST1
        );

INSTANTIATE_TYPED_TEST_SUITE_P(ParametrizedExampleTest,
        ExampleTest,
        TestTypes);