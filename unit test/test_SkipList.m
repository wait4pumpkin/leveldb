//
//  unit_test.m
//  unit test
//
//  Created by Pumpkin Lee on 9/26/17.
//  Copyright © 2017 Pumpkin Lee. All rights reserved.
//

#import <XCTest/XCTest.h>

#include <random>
#include <unordered_set>

#include "SkipList.hpp"

using namespace std;
using namespace lancer::leveldb::utils;

@interface unit_test : XCTestCase

@end

@implementation unit_test

typedef uint64_t Key;

struct Comparator {
    int operator()(const Key& a, const Key& b) const {
        if (a < b) {
            return -1;
        } else if (a > b) {
            return +1;
        } else {
            return 0;
        }
    }
};

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testInsertAndSearch {
    const int N = 2000;
    const int R = 5000;
    
    default_random_engine generator;
    uniform_int_distribution<int> distribution(0, 1000);

    unordered_set<Key> keys;
    Comparator cmp;
    SkipList<Key, Key, Comparator> list;
    for (int i = 0; i < N; i++) {
        Key key = distribution(generator) % R;
        if (keys.insert(key).second) {
            list.Insert(key, key);
        }
    }
    
    Key value;
    for (Key i = 0; i < R; i++) {
        if (list.Search(i, value))
        {
            XCTAssertEqual(keys.count(i), 1);
        }
        else
        {
            XCTAssertEqual(keys.count(i), 0);
        }
    }
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
