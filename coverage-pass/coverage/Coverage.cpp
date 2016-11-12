#include <vector>
#include "llvm/Pass.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Support/Debug.h"

using namespace llvm;


namespace {
    typedef std::map<Value *, std::vector<Value *>> CMap;
    typedef std::vector<Value *> VList;
    typedef std::vector<Instruction *> IList;

    const int width = 10;

    class Coverage : public FunctionPass {
        public:
            static char ID;
            Coverage() : FunctionPass(ID) { }
            bool runOnFunction(Function &F) override;
            void getAnalysisUsage(AnalysisUsage &AU) {
                AU.setPreservesCFG();
            }

        private:
            void initialize(IList InstList, CMap &map);
            void print(CMap map);
            void print(VList list);
            VList minimalSet(CMap Map, VList FullSet);
            void concatenate(CMap &map, IList ordered);
    };
}

char Coverage::ID = 0;
static RegisterPass<Coverage> X("coverage", "Coverage of each variable inside a Function",
        false /* Only looks at CFG */,
        false /* Analysis Pass */);

bool Coverage::runOnFunction(Function &F)
{
    CMap CoverageSets;

    /* storing unprocessed Instructions */
    IList WorkList, Ordered;
    VList FullSet; // storing global variables' definitions

    /* initialize WorkList with all Instructions */
    for (inst_iterator i = inst_begin(F), e = inst_end(F); i != e; ++i){
        WorkList.push_back(&*i);
        Ordered.push_back(&*i);
    }


    errs()<<"Function: " << F.getName() << "\n";
    initialize(WorkList, CoverageSets);

    DEBUG_WITH_TYPE("Coverage", errs() << "before concatenation:\n");
    DEBUG_WITH_TYPE("Coverage", print(CoverageSets));
    DEBUG_WITH_TYPE("Coverage", errs() << "After concatenation:\n");
    concatenate(CoverageSets, Ordered);

    print(CoverageSets);
    errs() << "\n";
    minimalSet(CoverageSets, FullSet);
}

// initializing the map. For each instruction, it will create a key:value pair
// key is the definition, and value are a list of operands used in the definition
void Coverage::initialize(IList InstList, CMap &map) {

    IList WorkList = InstList;
    std::map<Value *, Value *> equalMap;
    int changed = true;

    /* Loop over the WorkList to processing each instruction */
    while (!WorkList.empty()) {
        Instruction *I = WorkList.back();
        Value *V = dyn_cast<Value>(I);
        WorkList.pop_back();
        // Terminator instructions normally don't create new definitions, e.g. ret %name.
        // We skip them currently
        if (isa<TerminatorInst>(I)) {
            DEBUG_WITH_TYPE("Coverage", errs() << "Skipping: " << I->getOpcodeName() << "\n");
            continue;
        }

        // Currently have no idea how to handle the following instructions, skip them temporarily
        // ToDo: get correct handle method for these instructions

        // Vector operations: extractelement, insertelement, shufflevector
        if (isa<ShuffleVectorInst>(I) || isa<InsertElementInst>(I) || isa<ExtractElementInst>(I)) {
            DEBUG_WITH_TYPE("Coverage", errs() << "Skipping: " << I->getOpcodeName() << "\n");
            continue;
        }

        // Aggregate operations: extractValue, insertValue
        if (isa<ExtractValueInst>(I) || isa<InsertValueInst>(I)) {
            DEBUG_WITH_TYPE("Coverage", errs() << "Skipping: " << I->getOpcodeName() << "\n");
            continue;
        }

        // Memory Access and Addressing: alloca, load (done), store (done), fence, cmpxchg, atomicrmw
        if (isa<AllocaInst>(I) || isa<FenceInst>(I) || isa<AtomicCmpXchgInst>(I)
                || isa<AtomicRMWInst>(I)) {
            DEBUG_WITH_TYPE("Coverage", errs() << "Skipping: " << I->getOpcodeName() << "\n");
            continue;
        }

        // this is trying to handle GetElementPtrInst.
        // Currently, we just treat its definition is the same to its base operand.
        // This could be helpful for array, but struct may need some different way.
        if ( isa<GetElementPtrInst>(I) ) {
            Value *op = I->getOperand(0);
            equalMap[V] = op;
            continue;
        }

        if (isa<LoadInst>(I)) {
            Value *op = I->getOperand(0);
            equalMap[V] = op;
            continue;
        }

        // instruction `store op1, op2` is to move value from op1 to op2
        // therefore, we say op2 will cover op1.
        if (isa<StoreInst>(I)) {
            Value *dst = I->getOperand(1);
            Value *src = I->getOperand(0);

            //errs() << "Instruction: \n";
            //I->dump();
            //errs() << "operand type: ";
            //dst->getType()->dump();
            //errs() << " is a instruction: " << isa<Instruction>(dst);
            //errs() << "\n\n\n";

            // first time to use the map element, need to initialize (allocate memory for) it.
            if (map.find(dst) == map.end()) {
                map[dst] = VList();
            }
            map[dst].push_back(src);
            continue;
        }

        // handle sextInst
        if (isa<SExtInst>(I) || isa<BitCastInst>(I)) {
            Value *op = I->getOperand(0);
            equalMap[V] = op;
            continue;
        }

        // Conversion Operations: trunc .. to, zext, sext, fptrunc, fpext, fptoui, fptosi, bitcast etc.
        if (isa<TruncInst>(I)) {
            DEBUG_WITH_TYPE("Coverage", errs() << "Skipping: " << I->getOpcodeName() << "\n");
            continue;
        }

        // after inline, it may only include intrinsic and library APIs, skip temporary
        //handle intrinsic function call, e.g. llvm.memcpy.p0i8
        // ToDo: handle CallInst better if it is out of assumption
        if (isa<CallInst>(I)) {
            StringRef prefix = StringRef("llvm.memcpy");
            CallInst * CI = dyn_cast<CallInst>(I);
            Function *cf = CI->getCalledFunction();
            //errs() << "Called function: " << cf->getName() << "\n";
            if (cf->getName().startswith_lower(prefix)) {
                //errs() << "Processing function: " << cf->getName() << "\n";
                Value * dst = CI->getArgOperand(0);
                Value * src = CI->getArgOperand(1);
                //errs() << "dst: ";
                //dst->print(errs());
                //errs() << "\nsrc: ";
                //src->print(errs());
                //errs() << "\n";

                if (map.find(dst) == map.end()) {
                    map[dst] = VList();
                }
                map[dst].push_back(src);
            }
            continue;
        }

        // Other misc operations: landingpad, va_arg, select, call. phi, fcmp and icmp are treated as normal
        if (isa<LandingPadInst>(I) || isa<VAArgInst>(I) || isa<SelectInst>(I)) {
            DEBUG_WITH_TYPE("Coverage", errs() << "Skipping: " << I->getOpcodeName() << "\n");
            continue;
        }


        // following code is to handle all other instructions including
        // binary and shift operations
        // form of c = a op b. They define new definitions.

        // the first time to process the instruction, initialize the data structure
        if (map.find(V) == map.end())
            map[V] = std::vector<Value*>();

        // iterate over each operand in the instruction
        for (Instruction::op_iterator OI = I->op_begin(), OE = I->op_end(); OI != OE; OI++) {
            if (isa<Constant>(OI)) { // operand is a constant
                DEBUG_WITH_TYPE("Coverage", errs() << "Instruction: " << I->getName() << " Skipping Constant:");
                DEBUG_WITH_TYPE("Coverage", dyn_cast<Constant>(OI)->dump());
                continue;
            }
            // Operand is a LoadInst, we treat load definition equal to its operand.
            map[V].push_back(dyn_cast<Value>(OI));
        }

        // make sure current Instruction is removed from the WorkList
        WorkList.erase(std::remove(WorkList.begin(), WorkList.end(), I), WorkList.end());
    }

    //errs() << "equal map: \n";
    //for (std::map<Value *, Value *>::iterator it = equalMap.begin(), et = equalMap.end(); it != et; it++) {
    //    errs() << "[";
    //    it->first->print(errs());
    //    errs() << "] maps to [";
    //    it->second->print(errs());
    //    errs() << "]\n";
    //}

    //errs() << "map before replacing: \n";
    //print(map);
    //errs() << "---------------------\n";

    // replace instruction according to equalMap
    while(changed) {
        changed = false;
        VList ToRemove;
        for (CMap::iterator MB = map.begin(), ME = map.end(); MB != ME; MB++) {
            Value *key = MB->first;
            // key is replaceable
            if (equalMap.find(key) != equalMap.end()) {
                // if the key to be replaced with has been already in map
                if (map.find(equalMap[key]) != map.end()) {
                    map[equalMap[key]].insert(map[equalMap[key]].end(), (MB->second).begin(), (MB->second).end());
                } else { // key not in the map
                    std::swap(map[equalMap[key]], MB->second);
                }
                map.erase(key);
                key = equalMap[key];
                changed = true;
            }
            for(VList::iterator VB = map[key].begin(), VE = map[key].end(); VB != VE; VB++) {
               if (equalMap.find(*VB) != equalMap.end()) {
                   ToRemove.push_back(*VB);
                   map[key].push_back(equalMap[*VB]);
                   changed = true;
               }
            }
            for (VList::iterator VB = ToRemove.begin(), VE = ToRemove.end(); VB != VE; VB++) {
                map[key].erase(std::remove(map[key].begin(), map[key].end(), *VB), map[key].end());
            }
            ToRemove.clear();
        }

    }
    //errs() << "\n\nmap after replacing: \n";
    //print(map);
    //errs() << "---------------------\n";
}

// This function is to concatenate the coverage map. If the key of a set is in others values vector,
// It will be appended to the end of that vector
void Coverage::concatenate(CMap &map, IList ordered) {
    VList ToRemove;
    VList ToAppend;
    bool changed = true;

    while(changed) {
        changed = false;
        IList tmp = ordered;
        while(!tmp.empty()) {
            Instruction *I = tmp.back();
            Value *key = dyn_cast<Value>(I);
            tmp.pop_back();

            if (isa<StoreInst>(I)) {
                key = dyn_cast<Value>(I->getOperand(1));
            }

            if (map.find(key) == map.end()) continue;
            // if instruction has been marked as to be removed, skip it
            if ( std::find(ToRemove.begin(), ToRemove.end(), key) != ToRemove.end() ) continue;

            // iterate over each element to check whether it is also a key in the map
            // if yes concatenate that set to the end and mark it to be removed
            for ( VList::iterator LI = map[key].begin(), LE = map[key].end(); LI != LE; LI++) {
                if (map.find((*LI)) != map.end() && *LI != key) {
                    ToAppend.insert(ToAppend.end(), map[*LI].begin(), map[*LI].end());
                    ToRemove.push_back(*LI);
                    changed = true;
                }
            }
            // append to the end
            DEBUG_WITH_TYPE("Coverage", std::sort(ToAppend.begin(), ToAppend.end()));
            DEBUG_WITH_TYPE("Coverage", ToAppend.erase(std::unique(ToAppend.begin(), ToAppend.end()), ToAppend.end()));
            map[key].insert(map[key].end(), ToAppend.begin(), ToAppend.end());
            
            DEBUG_WITH_TYPE("Coverage", errs() << "Append to [" << key->getName() << "]: "); 
            DEBUG_WITH_TYPE("Coverage", print(ToAppend));
            ToAppend.clear();
            // remove duplicates
            std::sort(map[key].begin(), map[key].end() );
            map[key].erase(std::unique(map[key].begin(), map[key].end()), map[key].end());
        }

        //remove those has been concatenated
        for ( VList::iterator LI = ToRemove.begin(), LE = ToRemove.end(); LI != LE; LI++) {
            map.erase(*LI);
        }
        DEBUG_WITH_TYPE("Coverage", errs() << "to Remove:"); 
        DEBUG_WITH_TYPE("Coverage", print(ToRemove));
        ToRemove.clear();
    }

    // remove duplicates
    for ( CMap::iterator MB = map.begin(), ME = map.end(); MB != ME; MB++) {
        Value *key = MB->first;
        std::sort(map[key].begin(), map[key].end());
        map[key].erase(std::unique(map[key].begin(), map[key].end()), map[key].end());
    }
}



VList Coverage::minimalSet(CMap Map, VList FullSet) {
    for (CMap::iterator it = Map.begin(), et = Map.end(); it != et; it++);
    return VList();
}

void Coverage::print(CMap map) {
    int pos = 0;
    for (CMap::iterator MB = map.begin(), ME = map.end(); MB != ME; MB++) {
        pos=0;
        Value *key = MB->first;
        std::vector<Value *> set = MB->second;

        if (key->hasName()) {
            errs() << "[" << key->getName() << "] covers: \n";
        } else {
            errs() << "[";
            key->print(errs());
            errs() << "] covers: \n";

        }
        for (VList::iterator si = set.begin(), se = set.end(); si != se; si++) {
            if ((*si)->hasName())
                errs() << "[" << (*si)->getName() << "]";
            else {
                errs() << "[";
                (*si)->print(errs());
                errs() << "]";
            }
            if ((si + 1) != se)
                errs() << ",";
            pos++;
            if (pos % width == 0) {
                pos = 0;
                errs() << "\n";
            }
        }
        errs() << "\n\n";
    }
}

void Coverage::print(VList list) {
    int pos = 0;
    for (VList::iterator LB = list.begin(), LE=list.end(); LB != LE; LB++) {
        errs() << "[" << (*LB)->getName() << "]";
        if ((LB + 1) != LE)
            errs() << ",";

        pos++;
        if (pos % width == 0) {
            pos = 0;
            errs() << "\n";
        }
    }
    errs() << "\n";
}
