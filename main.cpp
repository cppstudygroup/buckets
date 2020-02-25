#include <iostream>
#include <vector>
#include <memory>

struct bucket{
    int amount;
    std::string bucket_name;

    bucket(int amount, std::string name):amount(amount),bucket_name(name){

    }
};

struct term{
   
    virtual ~term() {}

    virtual int operator()() const = 0;
    
    virtual int revert( int amount ) = 0;
};

struct expression{

    expression() = default;
    std::shared_ptr< term > term_;
};

expression operator+( const expression& lhs, bucket rhs );

bucket bucket_from_str(std::string bucket_name)
{

}
int main() {
    std::vector<std::string> buckets_order = {"a", "b", "c"};
    //20,20,20 --> revert() --> 0,10,20
    int amount_to_revert = 30;

    expression exp;
    for (auto bucket_name: buckets_order){
        exp = exp + bucket(20, bucket_name);
    }


}