#pragma once

#include "TreeAssembler.h"

struct IChainValue {
	enum class TypeChain {
		function,
		string,
		number,
		container
	};

	virtual TypeChain getType() const noexcept = 0;
};

class ChainValueFunction : public IChainValue {
public:
	enum class Operation {
		clear,
		read,
		edit,
		write,
		pop,
		push,
	};

	inline TypeChain getType() const override {
		return TypeChain::function;
	}

	inline Operation getOperation() const noexcept {
		return mOperation;
	}



private:
	Operation mOperation;
};

