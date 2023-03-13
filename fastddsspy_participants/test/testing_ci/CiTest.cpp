// Copyright 2021 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <cpp_utils/testing/gtest_aux.hpp>
#include <gtest/gtest.h>

#include <thread>

TEST(CiTest, asan)
{
    int* x = new int(42);
    ASSERT_EQ(*x, 42);
}

TEST(CiTest, tsan)
{
    int x = 1;
    auto foo = [&x](){ x++; };

    std::thread t1(foo);
    std::thread t2(foo);

    t1.join();
    t2.join();

    ASSERT_EQ(x, 3);
}

TEST(CiTest, fail)
{
    ASSERT_TRUE(false);
}

TEST(CiTest, flaky)
{
    ASSERT_TRUE(false);
}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
