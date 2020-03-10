#include <iostream>
#include <vector>
#include <memory>

struct bucket{
    int amount;
    std::string name;

    bucket(int amount, std::string name)
        : amount(amount)
        , name(name) {}

    int operator()() const { return amount; }
    int revert(int amount_to_revert) {
        if(amount < amount_to_revert) {
            auto remaining = amount_to_revert - amount;
            amount = 0;
            return remaining;
        } else {
            amount -= amount_to_revert;
            return 0;
        }
    }
};

struct term {
   
    virtual ~term() {}

    virtual int operator()() const = 0;
    
    virtual int revert( int amount ) = 0;
};

class variable: public term {
    public:
    int operator()() const { return bucket_->operator()(); }
    virtual int revert( int amount ) { return bucket_->revert( amount ); }

    variable( bucket& b ) : bucket_( &b ) {}

private:
    bucket* bucket_;
};

class and_bucket: public term {
    public:
    int operator()() const { return ( *lhs_ )() + ( *rhs_ )(); }
    int revert( int amount ) { return rhs_->revert( lhs_->revert( amount ) ); }

    and_bucket( std::shared_ptr< term > lhs, std::shared_ptr< term > rhs )
        : lhs_( lhs ), rhs_( rhs ) {}

private:
    std::shared_ptr< term > lhs_;
    std::shared_ptr< term > rhs_;
};

struct expression{

    expression() = default;
    expression( bucket& b ) : term_( new variable( b ) ) {}
    expression operator+( bucket& rhs ) {
        auto rhs_expr = expression(rhs);
        return term_ == nullptr
            ? rhs_expr
            : expression( std::shared_ptr< term >( new and_bucket( term_, rhs_expr.term_ ) ) );
    }
    int revert(int amount) { return term_ == nullptr ? amount : term_->revert( amount ); }
private:
    expression(std::shared_ptr< term > term) : term_(term) {}
    std::shared_ptr< term > term_;
};


int main() {
    std::vector<bucket> buckets_order = { bucket(20,"a"), bucket(20,"b"), bucket(20,"c") };
    //20,20,20 --> revert() --> 0,10,20
    int amount_to_revert = 30;

    expression exp;
    for (auto& bucket: buckets_order) {
        exp = exp + bucket;
    }

    std::cout << exp.revert(amount_to_revert) << std::endl;
    for (auto bucket: buckets_order) {
        std::cout << bucket.name << " " << bucket() << std::endl;
    }
}