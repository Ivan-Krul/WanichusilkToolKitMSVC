#pragma once
#include <string>
#include <vector>
#include <memory>

#include "Token.h"


class ParentNode {
public:
    inline ParentNode() noexcept = default;
    inline ParentNode(const TokenDescriptor& token) noexcept {
        mToken = token;
    }
    inline ParentNode(const ParentNode& self) noexcept {
        mToken = self.mToken;
    }
    inline TokenDescriptor getTokenDescriptor() const noexcept {
        return mToken;
    }
    inline virtual ~ParentNode() noexcept {}
protected:
    TokenDescriptor mToken;
};


class RootNode : public ParentNode {
public:
    inline RootNode() :ParentNode(TokenDescriptor()) {}

    template<typename T>
    inline typename std::enable_if<std::is_base_of<T, ParentNode>::value>::type push(std::shared_ptr<T> node) {
        mNodes.push_back(node);
    }
    inline size_t size() const noexcept {
        return mNodes.size();
    }
    inline void clear() noexcept {
        mNodes.clear();
    }
    inline const std::vector<std::shared_ptr<ParentNode>>& getArray() const noexcept {
        return mNodes;
    }

private:
    std::vector<std::shared_ptr<ParentNode>> mNodes;

};

class InsideNode : public ParentNode {
public:
    inline InsideNode(const TokenDescriptor& mToken, const std::shared_ptr<ParentNode>& guts) noexcept
        : ParentNode(mToken), mGuts(guts) {
    }

    inline std::shared_ptr<ParentNode> getGutsNode() const noexcept {
        return mGuts;
    }

private:
    std::shared_ptr<ParentNode> mGuts;
};

class ElementNode : public ParentNode {
public:
    inline ElementNode(const TokenDescriptor& token) noexcept : ParentNode(token) {};

    inline ElementNode(const TokenDescriptor& token, const std::vector<InsideNode>& indexes) noexcept : ParentNode(token) {
        mIndexes = indexes;
    }

    inline size_t getIndexSize() const noexcept {
        return mIndexes.size();
    }
    inline InsideNode operator[](size_t index) const noexcept {
        return mIndexes.at(index);
    }
private:
    std::vector<InsideNode> mIndexes;
};


class ValueNode : public ParentNode {
public:
    inline ValueNode(const TokenDescriptor& token) noexcept : ParentNode(token) {}
};



class ActionNode : public ParentNode {
public:
    inline ActionNode(const TokenDescriptor& token) noexcept : ParentNode(token) {}
};

class ConvertNode : public ParentNode {
public:
    inline ConvertNode(const TokenDescriptor& token) noexcept : ParentNode(token) {}
};


class UnarOperatorNode : public ParentNode {
public:
    UnarOperatorNode(const TokenDescriptor& mToken, const std::shared_ptr<ParentNode>& mOperand) noexcept
        : ParentNode(mToken), mOperand(mOperand) {
    }

    inline std::shared_ptr<ParentNode> getOperandNode() const noexcept {
        return mOperand;
    }
private:
    std::shared_ptr<ParentNode> mOperand;
};


class BinarOperatorNode : public ParentNode {
public:
    inline BinarOperatorNode(const TokenDescriptor& mToken, const std::shared_ptr<ParentNode>& mLeftOperand, const std::shared_ptr<ParentNode>& mRightOperand) noexcept
        : ParentNode(mToken), mLeftOperand(mLeftOperand), mRightOperand(mRightOperand) {}

    inline std::shared_ptr<ParentNode> getLeftOperandNode() const noexcept {
        return mLeftOperand;
    }
    inline std::shared_ptr<ParentNode> getRightOperandNode() const noexcept {
        return mRightOperand;
    }

private:
    std::shared_ptr<ParentNode> mLeftOperand;
    std::shared_ptr<ParentNode> mRightOperand;
    
};

