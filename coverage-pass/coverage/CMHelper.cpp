
#include "CMHelper.h"

void DependMap::insert(Value *parent, Value *child, StringRef meta) {
    // new item in map
    if (DMap.find(parent) == DMap.end()) {
        DMap[parent] = std::vector<NodeType>();
    }
    DMap[parent].push_back(std::make_pair(child, meta));
}

void DependMap::replace(Value *OldKey, Value *NewKey) {
    iterator it = DMap.find(OldKey);
    std::swap(DMap[NewKey], it->second);
    DMap.erase(it);
}

void DependMap::dump() {
    int count = 0;
    for (iterator B = DMap.begin(), E = DMap.end(); B != E; B++, count++) {
        Value *key = B->first;
        std::vector<NodeType> value = B->second;
        errs() << count << ": [" << key->getName() << "] Covers -- ";
        for (std::vector<NodeType>::iterator b = value.begin(), e = value.end(); b != e; b++) {
            if (b->first->hasName()) {
                errs() << "[" << b->first->getName() << "]";
            } else {
                errs() << "["; b->first->print(errs()); errs() << "]";
            }
            if (b+1 < e) {
                errs() << ", ";
            }
        }
        errs() << "\n";
    }
}
