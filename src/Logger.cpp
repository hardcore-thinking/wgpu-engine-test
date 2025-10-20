#include <Logger.hpp>

void Logger::Info(std::string message) const {
	std::cout << "INFO : " << message << std::endl;
};

void Logger::Warn(std::string message) const {
	std::cout << "WARNING: " << message << std::endl;
};

void Logger::Error(std::string message) const {
	std::cerr << "ERROR: " << message << std::endl;
}
