///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2019 Mikael Simberg
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///////////////////////////////////////////////////////////////////////////////

#include <hpx/hpx_main.hpp>
#include <hpx/include/async.hpp>
#include <hpx/include/iostreams.hpp>

void fun(hpx::future<double> x)
{
    hpx::cout << "The value of x is " << x.get() << hpx::endl;
}

int main()
{
    auto f = hpx::async([](){ return 3.14; });
    // The function in then takes the future as an argument.
    f.then(&fun);

    return 0;
}
