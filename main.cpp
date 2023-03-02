#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <cmath>
#include <iostream>
#include <thread>

void server()
{
    int ports[5] =
    {
        44300,
        44301,
        44302,
        4303,
        4304
    };

    std::cout << "gnugnjoifdgjdsigjhroigjergijgoigjoiewnfdoi\n";
    sf::TcpListener listener;
    sf::Socket::Status status;
    int currentPort = 0;
    for (int port : ports)
    {
        status = listener.listen(port);
        if (status == sf::Socket::Done)
        {
            currentPort = port;
            break;
        }
    }
    if(currentPort == 0)
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

void client()
{
    int ports[5] =
    {
        44300,
        44301,
        44302,
        4303,
        4304
    };

    sf::TcpSocket socket;
    sf::IpAddress address("localhost");
    sf::Socket::Status status;
    int currentPort = 0;
    for (int port : ports)
    {
        status = socket.connect(address, port);
        if (status == sf::Socket::Done)
        {
            currentPort = port;
            break;
        }
    }
    if (currentPort == 0)
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


int main()
{
    std::thread serverThread(&server);
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    client();
    serverThread.join();
    return 0;
}
