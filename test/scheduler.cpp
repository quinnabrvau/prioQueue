#include "scheduler.hpp"
#include "TestHarness.h"

TEST_GROUP(schedulerCreate)
{
    void setup() { }
    void teardown() { }
};

TEST(schedulerCreate, initEmptyPrioQueue) {
    scheduler S;
    CHECK_EQUAL(0, S.total());
    CHECK_EQUAL(0, S.ready());
    CHECK_EQUAL(0, S.waiting());
}
TEST(schedulerCreate, addTaskSize1) {
    scheduler S;
    task t;
    S.add(t);
    CHECK_EQUAL(1, S.total());
    CHECK_EQUAL(1, S.ready());
    CHECK_EQUAL(0, S.waiting());
}
TEST(schedulerCreate, addTaskSize30) {
    scheduler S;
    task T[30];
    for (int i = 0; i < 30; i++) {
        S.add(T[i]);
        CHECK_EQUAL(i+1, S.total());
        CHECK_EQUAL(i+1, S.ready());
        CHECK_EQUAL(0, S.waiting());
    }
}

TEST_GROUP(schedulerStart)
{
    void setup() { }
    void teardown() { }
};

TEST(schedulerStart, task1) {
    scheduler S;
    task t;
    S.add(t);
    S.start();
    CHECK_EQUAL(0, S.ready());
    POINTERS_EQUAL(&t, S.active());
}
TEST(schedulerStart, task2) {
    scheduler S;
    task t[2];
    S.add(t[0]); S.add(t[1]);
    S.start();
    CHECK_EQUAL(1, S.ready());
    POINTERS_EQUAL(&t[0], S.active());
}
TEST(schedulerStart, activeNone) {
    scheduler S;
    S.start();
    POINTERS_EQUAL(NULL, S.active());
}
TEST(schedulerStart, suspend1) {
    scheduler S;
    task t[2];
    S.add(t[0]); S.add(t[1]);
    S.suspend(t[0]);
    CHECK_EQUAL(1, S.ready());
    S.start();
    CHECK_EQUAL(0, S.ready());
    POINTERS_EQUAL(&t[1], S.active());
}

TEST_GROUP(schedulerRun)
{
    scheduler S;
    task A = task(1); task B = task(2); task C = task(2); task D = task(3);
    void setup() {
        S = scheduler();
        S.add(A); S.add(B); S.add(C); S.add(D);
        S.start();
    }
    void teardown() { }
};

TEST(schedulerRun, makeAActive) {
    S.run();
    POINTERS_EQUAL(&A, S.active());
    CHECK_EQUAL(3, S.ready());
}
TEST(schedulerRun, keepAActive) {
    S.run(); S.run();
    POINTERS_EQUAL(&A, S.active());
    CHECK_EQUAL(3, S.ready());
}
TEST(schedulerRun, startHigherPrioTask) {
    task max_prio = task(0);
    S.run();
    S.add(max_prio);
    POINTERS_EQUAL(&max_prio, S.active());
    CHECK_EQUAL(4, S.ready());
}
TEST(schedulerRun, stopAcheckBruns) {
    S.suspend(A);
    POINTERS_EQUAL(&B, S.active());
    CHECK_EQUAL(2, S.ready());
}
TEST(schedulerRun, stopRunning) {
    S.suspend();
    POINTERS_EQUAL(&B, S.active());
    CHECK_EQUAL(2, S.ready());
}
TEST(schedulerRun, stopAll) {
    S.suspend(A); S.suspend(B); S.suspend(C); S.suspend(D);
    POINTERS_EQUAL(NULL, S.active());
    CHECK_EQUAL(0, S.ready());
}
TEST(schedulerRun, suspendTwice) {
    S.suspend(A); S.suspend(A);
    POINTERS_EQUAL(&B, S.active());
    CHECK_EQUAL(2, S.ready());
}
TEST(schedulerRun, resumeA) {
    S.suspend();
    // S.resume(A);
    POINTERS_EQUAL(&A, S.active());
    CHECK_EQUAL(3, S.ready());
}




