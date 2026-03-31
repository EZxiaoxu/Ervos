#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <iostream>
#include "serial_library/include/SerialPort.hpp"
#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include <cstdlib> 
#include <ctime> 
#include <thread>

using namespace geode::prelude;
using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

int msize = 0;
const char* portfile_name = "C:\\macros\\port.txt";
const char* swiftclickfile_name = "C:\\macros\\swift.txt";
#define MAX_DATA_LENGTH 1

bool activated = false;
SerialPort *arduino;
bool should_play = true;
int framecount = 0;
float fps = 240;
gd::string current_level;
int swiftclick;

vector<vector<int>> get_macro(gd::string level_name) {
    ifstream macro_file;
    vector<vector<int>> readable_macro;
    int macro_size;
    int input;
    int frame;
    int repetitions;
    stringstream file;
    std::string name = level_name.c_str();
    file << "C:\\macros\\" << name << ".soup";

    ifstream swiftclickfile;
    swiftclickfile.open(swiftclickfile_name);
    swiftclickfile >> swiftclick;

    macro_file.open(file.str());
    if (!macro_file.is_open()) return readable_macro;

    macro_file >> macro_size;
    for (int i = 0; i < macro_size; i++) {
        if (!(macro_file >> input >> frame >> repetitions)) break;
        readable_macro.push_back({input, frame, repetitions});
    }
    macro_file.close();
    msize = readable_macro.size();
    return readable_macro;
}

// --- FIXED PLAY_MACRO FUNCTION ---
void play_macro(vector<vector<int>> macro) {
    if (activated && !macro.empty()) {
        for (int i = 0; i < macro.size(); i++) {
            // Wait until the game reaches the frame specified in the .soup file
            while (PlayLayer::get() && PlayLayer::get()->m_gameState.m_currentProgress < macro[i][1] && should_play) {
                // Empty loop to wait for the frame
            }

            if (should_play && arduino && arduino->isConnected()) {
                if (macro[i][0] == 1) {
                    // JUMP / HOLD START
                    arduino->writeSerialPort("1", MAX_DATA_LENGTH);
                }
                else if (macro[i][0] == 6) {
                    // RELEASE / HOLD END
                    arduino->writeSerialPort("6", MAX_DATA_LENGTH);
                }
                else if (macro[i][0] == 2) {
                    // SWIFT CLICK LOGIC
                    for (int j = 0; j < macro[i][2]; j++) {
                        arduino->writeSerialPort("1", MAX_DATA_LENGTH);
                        auto time = std::chrono::steady_clock::now();
                        while (std::chrono::steady_clock::now() - time < microseconds(swiftclick)) continue;
                        arduino->writeSerialPort("1", MAX_DATA_LENGTH);
                    }
                }
            }
            else if (!should_play) {
                break;
            }
        }
    }
}

class $modify(MenuLayer) {
    void onMoreGames(CCObject* sender) {
        if (!activated) {
            ifstream portfile(portfile_name);
            std::string portname;
            if (portfile >> portname) {
                arduino = new SerialPort(portname.c_str());
                sleep_for(milliseconds(200)); 
                if (arduino->isConnected()) {
                    arduino->writeSerialPort("6", MAX_DATA_LENGTH); // Ensure off on startup
                    activated = true;
                    FLAlertLayer::create("Arduino", "Connected to " + portname, "OK")->show();
                } else {
                    FLAlertLayer::create("Error", "Could not connect to " + portname, "OK")->show();
                }
            }
        }
        else {
            if (arduino) {
                arduino->writeSerialPort("6", MAX_DATA_LENGTH);
                arduino->closeSerial();
                delete arduino;
                arduino = nullptr;
            }
            activated = false;
            FLAlertLayer::create("Arduino", "Disconnected", "OK")->show();
        }
    }
};

class $modify(PlayerObject) {
    void playerDestroyed(bool p0) {
        PlayerObject::playerDestroyed(p0);
        if (activated) {
            should_play = false;
            std::thread restart_thread = std::thread{[]{ 
                sleep_for(milliseconds(50)); 
                if (arduino) arduino->writeSerialPort("6", MAX_DATA_LENGTH); 
            }};
            restart_thread.detach();
        }
    }
};

class $modify(PlayLayer) {
    void levelComplete() {
        PlayLayer::levelComplete();
        if (activated) {
            should_play = false;
            std::thread restart_thread = std::thread{[]{ 
                sleep_for(milliseconds(50)); 
                if (arduino) arduino->writeSerialPort("6", MAX_DATA_LENGTH); 
            }};
            restart_thread.detach();
        }
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        if (activated) {
            current_level = m_level->m_levelName;
            should_play = true;
            std::thread play_thread = std::thread{[&]{ 
                play_macro(get_macro(current_level)); 
            }};
            play_thread.detach();
        }
    }

    void onQuit() {
        PlayLayer::onQuit();
        if (activated) {
            should_play = false;
            if (arduino) arduino->writeSerialPort("6", MAX_DATA_LENGTH);
        }
    }

    void pauseGame(bool p0) {
        PlayLayer::pauseGame(p0);
        if (activated) {
            should_play = false; // Stop the macro thread
            if (arduino) arduino->writeSerialPort("6", MAX_DATA_LENGTH);
        }
    }
};
