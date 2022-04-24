#include <string>

class ui {
private:

public:

    void showScanning(std::string ip, uint16_t port);
    void showMessage(std::string message);
    void showProcess(double process);

    ui();
    ~ui();
};
