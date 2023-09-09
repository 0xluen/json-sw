#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;
using namespace boost::asio;


int main() {
    std::ifstream configFile("config.txt");
    std::map<std::string, std::string> configData;

    if (configFile.is_open()) {
        std::string line;
        while (std::getline(configFile, line)) {
            size_t delimiterPos = line.find('=');
            if (delimiterPos != std::string::npos) {
                std::string key = line.substr(0, delimiterPos);
                std::string value = line.substr(delimiterPos + 1);
                configData[key] = value;
            }
        }
        configFile.close();
    }
    else {
        std::cerr << "Config file not found or unable to open." << std::endl;
        return 1;
    }

    std::cout << "DB_HOST: " << configData["DB_HOST"] << std::endl;
    std::cout << "DB_USER: " << configData["DB_USER"] << std::endl;
    std::cout << "DB_PASSWORD: " << configData["DB_PASSWORD"] << std::endl;
    std::cout << "DB_NAME: " << configData["DB_NAME"] << std::endl;


    int port = 5000;

    io_service service;
    ip::tcp::endpoint endpoint(ip::tcp::v4(), port);
    ip::tcp::acceptor acceptor(service, endpoint);

    cout << "Server is running on port 5000." << endl;

    while (true) {
        ip::tcp::socket socket(service);
        acceptor.accept(socket);

        string clientIP = socket.remote_endpoint().address().to_string();

        ifstream jsonFile("data.json");
        string jsonData((istreambuf_iterator<char>(jsonFile)), istreambuf_iterator<char>());

        string httpResponse = "HTTP/1.1 200 OK\r\n";
        httpResponse += "Content-Type: application/json\r\n";
        httpResponse += "Content-Length: " + to_string(jsonData.size()) + "\r\n";
        httpResponse += "\r\n" + jsonData;

        ofstream logFile("server.log", ios_base::app);
        logFile << "IP: " << clientIP << " - Port: " << socket.remote_endpoint().port() << endl;
        logFile.close();

        cout << "IP: " << clientIP << " - Port: " << socket.remote_endpoint().port() << endl;

        boost::system::error_code ignored_error;
        write(socket, buffer(httpResponse), ignored_error);
    }

    return 0;
}
