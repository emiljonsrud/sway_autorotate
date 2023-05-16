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

            for (
                std::map<std::string, std::string>::const_iterator orientationPair = orientationToDegrees.begin();
                orientationPair != orientationToDegrees.end(); 
                ++orientationPair
                ) {
                if (result.find(orientationPair -> first) != result.npos) {
                    command = "swaymsg output eDP-1 transform " + orientationPair -> second;
                    std::cout << "executing: " << command << std::endl;
                    std::system(command.c_str());
                }
            }
        }
    }
    return 0;
}

