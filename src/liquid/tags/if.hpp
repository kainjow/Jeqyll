#ifndef LIQUID_IF_HPP
#define LIQUID_IF_HPP

#include "block.hpp"
#include "expression.hpp"

namespace Liquid {
    
    class Condition {
    public:
        enum class Operator {
            None,
            Equal,
            NotEqual,
            LessThan,
            GreaterThan,
            GreaterOrEqualThan,
            LessOrEqualThan,
            Contains,
        };
        
        enum class LogicalOperator {
            None,
            And,
            Or,
        };
        
        Condition()
            : op_(Operator::None)
            , logicalOp_(LogicalOperator::None)
        {
        }
        
        Condition(const Expression& a)
            : a_(a)
            , op_(Operator::None)
            , logicalOp_(LogicalOperator::None)
        {
        }
        
        Condition(const Expression& a, Operator op, const Expression& b)
            : a_(a)
            , op_(op)
            , b_(b)
            , logicalOp_(LogicalOperator::None)
        {
            (void)op_;
        }
        
        void setLogicalCondition(LogicalOperator op, const std::shared_ptr<Condition>& cond) {
            logicalOp_ = op;
            child_ = cond;
        }
        
        bool evaluate(Context& context);
    private:
        Expression a_;
        Operator op_;
        Expression b_;
        LogicalOperator logicalOp_;
        std::shared_ptr<Condition> child_;
    };
    
    class IfBlock {
    public:
        IfBlock(bool elseBlock) : isElse(elseBlock) {}
        BlockBody body;
        Condition cond;
        bool isElse;
    };
    
    class IfTag : public BlockTag {
    public:
        IfTag(bool unless, const Context& context, const StringRef& tagName, const StringRef& markup);
        
        virtual void parse(const Context& context, Tokenizer& tokenizer) override;
        
        virtual String render(Context& context) override;
        
    protected:
        virtual void handleUnknownTag(const StringRef& tagName, const StringRef& markup, Tokenizer& tokenizer) override;

    private:
        void parseTag(const StringRef& markup);
        Condition parseLogicalCondition(Parser& parser);
        Condition parseCondition(Parser& parser);
        
        bool if_;
        std::vector<IfBlock> blocks_;
    };
}

#endif
