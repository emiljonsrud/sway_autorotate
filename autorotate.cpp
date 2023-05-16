#include <iostream>
#include <cstdio>
#include <array>
#include <memory>
#include <map>

const std::map<std::string, std::string> orientationToDegrees = {
    {"normal", "0"},
    {"right-up", "90"},
    {"bottom-up", "180"},
    {"left-up", "270"}
};

int main() {
    std::array<char, 128> buffer;
    std::string result;
    std::string orientation;
    std::string command;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("monitor-sensor", "r"), pclose);

    if (!pipe) {
        std::cerr << "popen() failed!" << std::endl;
        return 1;
    }
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr) {
            result = buffer.data();

            std::size_t last_space = result.find_last_of(" ");
            orientation = result.substr(last_space+1);
            orientation.erase(orientation.length()-1); // Remove newline

            try {
                command = "swaymsg output eDP-1 transform " + orientationToDegrees.at(orientation);
                std::cout << "executing: " << command << std::endl;
                std::system(command.c_str());
            } catch (std::out_of_range) {
                std::cout << "invalid orientation: " << orientation << std::endl;
            }
        }
    }
    return 0;
}

