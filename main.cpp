#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <cmath>
#include <iostream>
#include <thread>

void server(int port)
{
    sf::TcpListener listener;
    sf::Socket::Status status;
    status = listener.listen(port);
    if (status != sf::Socket::Done)
    {
        std::cout << "Error listening\n";
        return;
    }

    sf::TcpSocket socket;
    status = listener.accept(socket);
    if (status != sf::Socket::Done)
    {
        std::cout << "Error accepting\n";
        return;
    }

    sf::Packet packet;
    status = socket.receive(packet);
    if (status != sf::Socket::Done)
    {
        std::cout << "Error receiving\n";
        return;
    }

    std::string message;
    packet >> message;
    std::cout << message;
    packet.clear();
    packet << "message received\n";
    status = socket.send(packet);
    if (status != sf::Socket::Done)
    {
        std::cout << "Error sending\n";
        return;
    }
}

void client(int port)
{
    sf::TcpSocket socket;
    sf::IpAddress address("localhost");
    sf::Socket::Status status;
    status = socket.connect(address, port);
    if (status != sf::Socket::Done)
    {
        std::cout << "Error connecting\n";
        return;
    }

    sf::Packet packet;
    packet << "Hello world\n";
    status = socket.send(packet);
    if (status != sf::Socket::Done)
    {
        std::cout << "Error sending\n";
        return;
    }

    packet.clear();
    status = socket.receive(packet);
    if (status != sf::Socket::Done)
    {
        std::cout << "Error receiving\n";
        return;
    }

    std::string message;
    packet >> message;
    std::cout << message;

}

bool portOpen(const std::string& address, int port)
{
    return (sf::TcpSocket().connect(address, port) == sf::Socket::Done);
}

int main()
{
    int ports[5] =
    {
        4300,
        4301,
        4302,
        4303,
        4304
    };

    int currentPort = 0;
    for (int port : ports)
    {
        if (portOpen("localhost", port))
        {
            currentPort = port;
        }
    }

    if(currentPort != 0)
    {
        std::thread serverThread([currentPort] &server(currentPort));
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        client(currentPort);
        serverThread.join();
    }
    return 0;
}
