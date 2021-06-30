#include <QApplication>

#include "control/control.h"

#include <cstdlib> // std::getenv
#include <string>

#include <boost/program_options.hpp>

struct Params {
    std::string host;
};

constexpr const char* hostOpt = "host";

Params setParameters(int argc, char** argv) {
    namespace po = boost::program_options;

    Params params;
    try {
        po::options_description desc("Allowed options");

        desc.add_options()
                ("help, h", "produce help message")
                ("version, v", "get client version")
                (hostOpt, po::value<std::string>()->default_value("127.0.0.1"), "host basic gateway server")
                ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count(hostOpt) != 0U) {
            params.host = vm[hostOpt].as<std::string>();
        }
    }
    catch(...)
    {
        std::cerr << "Failed parse params" << std::endl;
    }

    return params;
}



int main(int argc, char** argv) {
    auto params = setParameters(argc, argv);
    QApplication a(argc, argv);

    Storage::SqliteConfig config;
    config.folder_path = std::string{std::getenv("HOME")} + "/AppChat/";
    config.connection_string = "file://" + config.folder_path + "cache.db";

    Control control(config, params.host);
    control.run_app(argc, argv);

    return a.exec();
}
