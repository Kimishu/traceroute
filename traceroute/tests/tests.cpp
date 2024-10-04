//
// Created by Kimihito on 04.10.2024.
//

#include <gtest/gtest.h>
#include "../src/Tracerouter/Tracerouter.h"

int globalArgc = 0;
char** globalArgv = nullptr;

//Tracerouter tests
TEST(TracerouteRunTest1,ConstructorArgs){
    Tracerouter tracerouter("google.com");
    EXPECT_NO_THROW(tracerouter.Trace());
}

TEST(TracerouteRunTest2,SetDestination){
    Tracerouter tracerouter;
    tracerouter.SetDestination("google.com");
    EXPECT_NO_THROW(tracerouter.Trace());
}

TEST(TracerouteRunTest3, UnexistedDestination){
    Tracerouter tracerouter;
    EXPECT_EXIT(tracerouter.SetDestination("1.1.4.1.2.254.25"), ::testing::ExitedWithCode(
            static_cast<int>(Tracerouter::ErrorCodes::UnexistedDestionationErr)), "");
}

TEST(TracerouteRunTest4, CommandLineTest){
    ASSERT_GE(globalArgc, 2);
    Tracerouter tracerouter(globalArgv[1]);
    EXPECT_NO_THROW(tracerouter.Trace());
}

int main(int argc, char** argv) {
    globalArgc = argc;
    globalArgv = argv;

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}