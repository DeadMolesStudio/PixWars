//
// Created by Artyom on 21.06.2018.
//

#ifndef CPP_SERVER_ERROR_CODE_IMPL_HPP
#define CPP_SERVER_ERROR_CODE_IMPL_HPP

#include <boost/system/error_code.hpp>

#include "error_code.hpp"


class error_code_impl {
 private:
    boost::system::error_code e;
};


#endif //CPP_SERVER_ERROR_CODE_IMPL_HPP
