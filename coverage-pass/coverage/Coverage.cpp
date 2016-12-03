#include "CMHelper.h"
#include <vector>
#include <llvm/Pass.h>
#include <llvm/IR/Metadata.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include <llvm/Support/Debug.h>


using namespace llvm;


namespace {
    // data structure to store the coverage mapping
    typedef std::vector<Value *> VList;
    typedef std::vector<Instruction *> IList;

    class Coverage : public FunctionPass {
    public:
        static char ID;
        Coverage() : FunctionPass(ID) { }
        bool runOnFunction(Function &F) override;
        void getAnalysisUsage(AnalysisUsage &AU) {
            AU.setPreservesCFG();
        }

    private:
        DependMap DMap;
        std::map<Value *, Value *> equalMap;

        void initialize(IList InstList);
        Value* getReplacement(Value *V);
        void handleBinOp(Instruction *I);
        void merge();
        void dump();
    };
}

char Coverage::ID = 0;
static RegisterPass<Coverage> X("coverage", "Coverage of each variable inside a Function",
                                false /* Only looks at CFG */,
                                false /* Analysis Pass */);

bool Coverage::runOnFunction(Function &F)
{
    /* storing unprocessed Instructions */
    IList WorkList;

    /* initialize WorkList with all Instructions */
    for (inst_iterator i = inst_begin(F), e = inst_end(F); i != e; ++i){
        WorkList.push_back(&*i);
    }

    errs()<<"Function: " << F.getName() << "\n";
    errs() << "initialize ... \n";
    initialize(WorkList);
    errs() << "merge ... \n";
    merge();
    errs() << "dump ... \n";
    dump();
    return false;
}

Value* Coverage::getReplacement(Value *V) {
    if (equalMap.find(V) == equalMap.end()) {
        return nullptr;
    }
    Value *result = V;
    while (equalMap.find(result) != equalMap.end()) {
        result = equalMap[result];
    }

    return result;
}

void Coverage::merge() {
    Value *tmp;
    errs() << "Replacement Map size:" << equalMap.size() << "\n";
    for (DependMap::iterator B = DMap.begin(), E=DMap.end(); B != E; B++ ) {
        Value *key = B->first;
        std::vector<NodeType> *value = &(B->second);
        // iterate over each element in the vector
        for (std::vector<NodeType>::iterator b = value->begin(), e = value->end(); b != e; b++) {
            tmp = getReplacement(b->first);
            if (tmp) {
                errs() << "replacing ["; b->first->print(errs()); errs() << "] with ["; tmp->print(errs()); errs() << "]\n";
                b->first = tmp;
                b->first->dump();
            }
        }

        tmp = getReplacement(key);
        if (tmp) {
            errs() << "replacing ["; key->print(errs()); errs() << "] with ["; tmp->print(errs()); errs() << "]\n";
            DMap.replace(key, tmp);
        }
    }
}

void Coverage::dump() {
    errs() << "Coverage Map: \n";
    int count = 0;
    DMap.dump();
    errs() << "\n\nReplacement Map(size:" << equalMap.size() << "): \n";
    for (std::map<Value*, Value*>::iterator B = equalMap.begin(), E=equalMap.end(); B != E; B++, count++) {
        errs() << count << ": ["; B->first->print(errs()); errs() << "] ";
        errs() << "replaceable with -- ["; B->second->print(errs()); errs()<< "]\n";
    }
    errs() << "\n\n";
}

void Coverage::handleBinOp(Instruction *I) {
    // iterate over each operand in the instruction
    for (Instruction::op_iterator OI = I->op_begin(), OE = I->op_end(); OI != OE; OI++) {
        Instruction *op = dyn_cast<Instruction>(OI);
        errs() << "has metadata: " << op->hasMetadata() << "\n\n";

#if 0
        if (op->hasMetadata()) {
            MDNode *md = op->getMetadata("coverage");
            StringRef index = dyn_cast<MDString>(md->getOperand(0))->getString();
            DMap.insert(I, dyn_cast<Value>(OI), index);
        } else {
            DMap.insert(I, dyn_cast<Value>(OI));
        }
#else
        errs() << "insert: [";  I->print(errs()); errs() << "], ["; OI->get()->print(errs()); errs() << "]";
        DMap.insert(I, dyn_cast<Value>(OI));
#endif

    }
}

// initializing the map. For each instruction, it will create a key:value pair
// key is the definition, and value are a list of operands used in the definition
void Coverage::initialize(IList InstList) {

    IList WorkList = InstList;

    /* Loop over the WorkList to processing each instruction */
    while (!WorkList.empty()) {
        Instruction *I = WorkList.back();
        WorkList.pop_back();

        // make sure current Instruction is removed from the WorkList
        // WorkList.erase(std::remove(WorkList.begin(), WorkList.end(), I), WorkList.end());

        // Terminator instructions, including BranchInst, CatchReturnInst, CatchSwitchInst,
        // CleanupRetrunInst, IndirectBrInst, InvokeInst, ResumeInst, ReturnInst, SwitchInst
        // and UnreachableInst, normally don't create new definitions, e.g. ret %name.
        // we currently skip them as they don't define new variables
        if (isa<TerminatorInst>(I)) {
            //errs() << "Processing(Terminator): "; I->dump();
            DEBUG_WITH_TYPE("Coverage", errs() << "Skipping: " << I->getOpcodeName() << "\n");
            continue;
        }

        if (isa<StoreInst>(I)) {
            //errs() << "Processing(store): "; I->dump();
            Value *dst = I->getOperand(1);
            Value *src = I->getOperand(0);
            DMap.insert(dst, src);
            continue;
        }

        // LoadInst, GetElementPtrInst, and Conversion Instructions (Castings) define an instruction equal to its operands
        if (isa<LoadInst>(I) || isa<GetElementPtrInst>(I) || isa<CastInst>(I)) {
            Value *V = dyn_cast<Value>(I);
            equalMap[V] = I->getOperand(0);

            //errs() << "Processing(Load/GetElement/Cast): "; I->print(errs()); errs() << ", equalMap size: " << equalMap.size() << "\n";
            continue;
        }


        // handle BinaryOperator
        if (I->isBinaryOp()) {
            //errs() << "Processing(BinOp): "; I->dump();
            handleBinOp(I);
            continue;
        }

        // after inline, it may only include intrinsic and library APIs, skip temporary
        //handle intrinsic function call, e.g. llvm.memcpy.p0i8
        // ToDo: handle CallInst better if it is out of assumption
        if (isa<CallInst>(I)) {
            //errs() << "Processing(Call): "; I->dump();
            StringRef prefix = StringRef("llvm.memcpy");
            CallInst * CI = dyn_cast<CallInst>(I);
            Function *cf = CI->getCalledFunction();
            if (cf->getName().startswith_lower(prefix)) {
                Value * dst = CI->getArgOperand(0);
                Value * src = CI->getArgOperand(1);
                DMap.insert(dst, src);
            }
            continue;
        }

        // Memory Access and Addressing: alloca, load (done), store (done), fence, cmpxchg, atomicrmw
        if (isa<AllocaInst>(I) || isa<FenceInst>(I) || isa<AtomicCmpXchgInst>(I)
            || isa<AtomicRMWInst>(I)) {
            //errs() << "Processing(Other Mem): "; I->dump();
            DEBUG_WITH_TYPE("Coverage", errs() << "Skipping: " << I->getOpcodeName() << "\n");
            continue;
        }

        // Other misc operations: landingpad, va_arg, select, call. phi, fcmp and icmp are treated as normal
        if (isa<LandingPadInst>(I) || isa<VAArgInst>(I) || isa<SelectInst>(I)) {
            //errs() << "Processing(Other): "; I->dump();
            DEBUG_WITH_TYPE("Coverage", errs() << "Skipping: " << I->getOpcodeName() << "\n");
            continue;
        }

        errs() << "Un-processed instruction:"; I->dump(); errs() << "\n";
        //llvm_unreachable("Unreachable point: above instruction not processed correctly");
    }
}
