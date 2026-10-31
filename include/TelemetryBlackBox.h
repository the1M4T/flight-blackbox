 #pragma once                                                                                                                                                           
    #include <cstdint>                                                                                                                                                     
                                                                                                                                                                           
    enum RocketState : uint8_t {                                                                                                                                           
        DISARM    = 0,                                                                                                                                                     
        ARM       = 1,                                                                                                                                                     
        FLIGHT    = 2,                                                                                                                                                     
        PARACHUTE = 3,                                                                                                                                                     
        LANDED    = 4                                                                                                                                                      
    };                                                                                                                                                                     
                                                                                                                                                                           
    class TelemetryBlackBox {                                                                                                                                              
    public:                                                                                                                                                                
        void onRocketStateChange(RocketState state);                                                                                                                       
        void onParachuteDeployed();                                                                                                                                        
                                                                                                                                                                           
        /**                                                                                                                                                                
         * @param error - Informuje czy wystąpił błąd barometru                                                                                                            
         * @param value - Odczyt wysokości w stopach                                                                                                                       
         */                                                                                                                                                                
        void onBaroReading(bool error, uint16_t value);                                                                                                                    
                                                                                                                                                                           
        /**                                                                                                                                                                
         * @brief Pobiera bieżący stan, składa go w 32-bitową ramkę z sumą kontrolną                                                                                       
         *        i zapisuje do bufora.                                                                                                                                    
         */                                                                                                                                                                
        void recordCurrentState();                                                                                                                                         
                                                                                                                                                                           
        /**                                                                                                                                                                
         * @brief Funkcja pobiera ostatnią ramkę z bufora kołowego, dekoduje ją oraz                                                                                       
         *        loguje zdekodowane wartości w czytelny sposób na konsolę.                                                                                                
         * @return Ostatnia ramka zapisana do bufora, jeśli xor się zgadza.                                                                                                
         *         0xFFFFFFFF jeśli suma kontrolna jest niepoprawna.                                                                                                       
         */                                                                                                                                                                
        uint32_t getLastDataframe();                                                                                                                                       
                                                                                                                                                                           
    private:                                                                                                                                                                            
    };                