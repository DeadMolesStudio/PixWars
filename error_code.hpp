#ifndef CPP_SERVER_ERROR_CODE_HPP
#define CPP_SERVER_ERROR_CODE_HPP


class error_code {
 private:
    class error_code_impl;
    error_code_impl* p;
};


#endif //CPP_SERVER_ERROR_CODE_HPP
