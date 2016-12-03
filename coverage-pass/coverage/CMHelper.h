//
// Created by Chao Chen on 12/1/16.
//

#ifndef COVERAGE_PASS_CMHELPER_H
#define COVERAGE_PASS_CMHELPER_H

#include <vector>
#include <llvm/Pass.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

typedef std::pair<Value *, StringRef> NodeType;
typedef std::map<Value *, std::vector<NodeType>> DMapType;

class DependMap {
private:
    DMapType DMap;
public:
    typedef DMapType::iterator iterator;
    iterator begin() {return DMap.begin();}
    iterator end() {return DMap.end(); }

    void insert(Value *parent, Value *child, StringRef meta="");
    void replace(Value *oldkey, Value *newkey);
    void dump();

};

#endif //COVERAGE_PASS_CMHELPER_H

