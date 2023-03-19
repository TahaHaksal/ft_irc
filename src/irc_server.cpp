#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void    ft_error(std::string message)
{
    std::cout << message << std::endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[])
{
    int serverSocket, clientSocket, maxClients = MAX_CLIENTS;
    int clientSockets[MAX_CLIENTS] = {0};
    struct sockaddr_in serverAddress, clientAddress;
	int addrlen = sizeof(serverAddress);
    char buffer[BUFFER_SIZE];
    fd_set readfds;

    // Yeni bir soket oluştur
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
        ft_error("Soket oluşturma başarısız oldu");

    // Soket adresini ayarla
    memset(&serverAddress, 0x00, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(6667); // IRC sunucu bağlantı noktası

    int a = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &a, sizeof(a)) < 0)
        ft_error("Socket option hatası bulundu!");

    // Soket adresine bağlan
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
        ft_error("Soket adresine bağlanma başarısız oldu");

    // Dinleme moduna geç
    if (listen(serverSocket, maxClients) == -1)
        ft_error("Dinleme moduna geçme başarısız oldu");

    std::cout << "IRC sunucusu başlatıldı\n";
    while (true)
    {
        FD_ZERO(&readfds);
        FD_SET(serverSocket, &readfds);

        // Client soketlerini ayarla
        for (int i = 0; i < maxClients; i++)
        {
            clientSocket = clientSockets[i];
            if (clientSocket > 0)
                FD_SET(clientSocket, &readfds);
        }

        // Gelen bağlantıları bekle
        int activity = select(FD_SETSIZE, &readfds, NULL, NULL, NULL);

        if (activity < 0)
            ft_error("Select fonksiyonu hata verdi");

        // Gelen bağlantı var mı kontrol et
        if (FD_ISSET(serverSocket, &readfds))
        {
            int newClient = accept(serverSocket, (struct sockaddr*)&serverAddress, (socklen_t*)&addrlen);
            if (newClient == -1)
            {
                std::cerr << "Yeni bir bağlantı kabul edilemedi\n";
                continue;
            }

            // Client soketlerini kontrol et
            for (int i = 0; i < maxClients; i++)
            {
                if (clientSockets[i] == 0)
                {
                    clientSockets[i] = newClient;
                    std::cout << "Yeni bir bağlantı kabul edildi. Soket numarası: " << newClient << "\n";
                    break ;
                }
            }
        }

        // Client soketlerindeki verileri kontrol et
        for (int i = 0; i < maxClients; i++)
        {
            clientSocket = clientSockets[i];
            if (FD_ISSET(clientSocket, &readfds))
            {
                int numBytes = recv(clientSocket, buffer, BUFFER_SIZE, 0);
                if (numBytes == 0)
                {
                    // Bağlantı kesildi
                    std::cout << "Soket numarası " << clientSocket << " bağlantısı kesildi\n";
                    close(clientSocket);
                    clientSockets[i] = 0;
                }
                else
                {
                    // Veri alındı
                    std::string message(buffer, numBytes);
                    message.erase(std::remove(message.begin(), message.end(), '\r'), message.end()); // \r karakterini kaldır
                    std::cout << "Soket numarası " << clientSocket << " tarafından mesaj alındı: " << message << "\n";

                    // Kullanıcının girdiği komutu işle
                    std::vector<std::string> tokens = split(message, ' ');
                    if (tokens[0] == "QUIT")
                    {
                        // Kullanıcı çıkış yapmak istiyor
                        std::cout << "Soket numarası " << clientSocket << " tarafından çıkış yapıldı\n";
                        close(clientSocket);
                        clientSockets[i] = 0;
                    }
                    else if (tokens[0] == "JOIN")
                    {
                        // Kullanıcı bir kanala katılmak istiyor
                        std::string channelName = tokens[1];
                        std::cout << "Soket numarası " << clientSocket << " tarafından " << channelName << " kanalına katılma talebi geldi\n";
                        if (tokens.size() < 2) {
                            std::cout << "Hata: Kanal adı belirtilmedi" << std::endl;
                            break;
                        }

                        // Kanal adını tokens[1]'den alırız

                        // Kanal adının başında "#" işareti yoksa ekleriz
                        if (channelName[0] != '#') {
                            channelName = "#" + channelName;
                        }

                        // Kullanıcının ilgili kanala katılması için JOIN komutu oluşturulur
                        std::string joinCommand = "JOIN " + channelName + "\r\n";

                        // JOIN komutu sunucuya gönderilir
                        int sendResult = send(clientSocket, joinCommand.c_str(), joinCommand.length(), 0);

                        if (sendResult == -1) {
                            std::cout << "Hata: JOIN komutu gönderilemedi" << std::endl;
                            // closesocket(clientSocket);
                            break;
                        }
                        std::cout << "Soket numarası " << clientSocket << " tarafından " << channelName << " kanalına katılma talebi gönderildi\n";
                    }
                    else if (tokens[0] == "PART")
                    {
                        // Kullanıcı bir kanaldan ayrılmak istiyor
                        std::string channelName = tokens[1];
                        std::cout << "Soket numarası " << clientSocket << " tarafından " << channelName << " kanalından ayrılma talebi geldi\n";
                        // Kanaldan ayrılma işlemi gerçekleştirilir
                        // ...
                    }
                    else
                    {
                        // Bilinmeyen bir komut gönderildi
                        std::cout << "Soket numarası " << clientSocket << " tarafından bilinmeyen bir komut gönderildi: " << message << "\n";
                    }
                }
            }
        }
    }
    // Server soketini kapat
    close(serverSocket);
	shutdown(serverSocket, SHUT_RDWR); 
    return (0);
}

