#include <iostream>
#include <boost/lambda/lambda.hpp>
#include <algorithm>

int  main(){
    std::vector<int> v {1, 3, 2};
    std::for_each(v.begin(), v.end(),
                  std::cout<<boost::lambda::_1<<"\n");
    return 0;
}