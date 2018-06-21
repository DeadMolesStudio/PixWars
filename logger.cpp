#include <boost/date_time/posix_time/posix_time.hpp>
#include <fstream>
#include <string>


void write_log(const std::string& message, const std::string& filename) {
    std::ofstream log_file(filename, std::ios::app);
    log_file << boost::posix_time::microsec_clock::local_time() << ": "
             << message << std::endl;

    log_file.close();
}
