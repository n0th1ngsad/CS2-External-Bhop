#include "Memory/memory.h"
#include <iostream>
#include <Windows.h>
#include <thread>

constexpr uint32_t FLAG_STAND = 65665;
constexpr uint32_t FLAG_CROUCH = 65667;
constexpr uint32_t JUMP_TRIGGER_ON = 65537;
constexpr uint32_t JUMP_TRIGGER_OFF = 256;

int LOCAL_PLAYER_OFFSET = 0x17D47E0;
int FORCE_JUMP_OFFSET = 0x17CD510;
int PLAYER_FLAGS_OFFSET = 0x3CC;

int main() {
    Memory memory("cs2.exe");
    std::uintptr_t clientModuleBase = memory.GetModuleAddress("client.dll");
    std::uintptr_t forceJumpAddress = clientModuleBase + FORCE_JUMP_OFFSET;

    while (true) {
        std::uintptr_t localPlayerBase = memory.Read<std::uintptr_t>(clientModuleBase + LOCAL_PLAYER_OFFSET);
        uint32_t playerFlags = memory.Read<std::uint32_t>(localPlayerBase + PLAYER_FLAGS_OFFSET);

        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            if (playerFlags == FLAG_STAND || playerFlags == FLAG_CROUCH) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                memory.Write<uint32_t>(forceJumpAddress, JUMP_TRIGGER_ON);
            }
            else {
                memory.Write<uint32_t>(forceJumpAddress, JUMP_TRIGGER_OFF);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
