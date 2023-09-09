#include <iostream>
#include <fstream>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;


int main() {
    int port = 5000 ;
    
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
