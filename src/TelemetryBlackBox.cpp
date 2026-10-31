#include "TelemetryBlackBox.h"
#include <iostream>
#include <iomanip>

void TelemetryBlackBox::onRocketStateChange(RocketState state) { //faza lotu rakiety
    if (state >= DISARM && state <= LANDED) {
        current_state_ = state;
    }
}

void TelemetryBlackBox::onParachuteDeployed() { //stan wyzwolenia spadochronu
    parachute_deployed_ = true;
}

void TelemetryBlackBox::onBaroReading(bool error, uint16_t value) { //stan odczytu barometru
    baro_error_ = error;
    altitude_ = value; 
}

void TelemetryBlackBox::recordCurrentState() { //zapisanie aktualnego stanu do bufora
    uint16_t encoded_altitude = altitude_;
    if (current_state_ == DISARM || current_state_ == LANDED) {
        encoded_altitude = 0;
    }
    if (baro_error_) {
        encoded_altitude = 0xFFFF;
    }
    uint8_t byte0 = (static_cast<uint8_t>(current_state_) & 0x07) //pierwszy bajt
                    | ((parachute_deployed_ ? 1U : 0U) << 3) 
                    | ((baro_error_ ? 1U : 0U) << 4);
    uint8_t byte1 = static_cast<uint8_t>(encoded_altitude & 0xFF); //drugi bajt
    uint8_t byte2 = static_cast<uint8_t>((encoded_altitude >> 8) & 0xFF); //trzeci bajt
    uint8_t checksum = byte0 ^ byte1 ^ byte2;
    uint32_t frame = (static_cast<uint32_t>(byte0))
                   | (static_cast<uint32_t>(byte1) << 8)
                   | (static_cast<uint32_t>(byte2) << 16)
                   | (static_cast<uint32_t>(checksum) << 24);
    buffer_[head_] = frame; //zapis do bufora
    head_ = (head_ + 1) % BUFFER_SIZE; //iteracja indeksu bufora
    if (count_ < BUFFER_SIZE) {
        count_++; 
    }
} 

uint32_t TelemetryBlackBox::getLastDataframe() { //pobranie ostatniego zapisanego stanu z bufora
    if (count_ == 0) {
        return 0xFFFFFFFF; //pusty bufor
    }
    size_t last_index = (head_ == 0) ? (BUFFER_SIZE - 1) : (head_ - 1);
    uint32_t frame = buffer_[last_index];
    uint8_t byte0 = frame & 0xFF;
    uint8_t byte1 = (frame >> 8) & 0xFF;
    uint8_t byte2 = (frame >> 16) & 0xFF;
    uint8_t received_checksum = (frame >> 24) & 0xFF;
    if ((byte0 ^ byte1 ^ byte2) != received_checksum) {
        return 0xFFFFFFFF;
    }
    uint8_t state = frame & 0x07;
    bool parachute = (frame >> 3) & 0x01;
    bool baro_error = (frame >> 4) & 0x01;
    uint16_t altitude = (frame >> 8) & 0xFFFF;
    std::cout << "--- Telemetria [Ramka: 0x" << std::hex << std::uppercase 
              << std::setw(8) << std::setfill('0') << frame << std::dec << "] ---\n"
              << "Faza lotu        : " << static_cast<int>(state) << "\n"
              << "Spadochron       : " << (parachute ? "OTWARTY" : "SCHOWANY") << "\n"
              << "Blad barometru   : " << (baro_error ? "AWARIA" : "BRAK") << "\n"
              << "Wysokosc         : " << altitude << " ft\n"
              << "Suma kontrolna   : 0x" << std::hex << static_cast<int>(received_checksum) << std::dec << "\n";
    return frame;
} 


