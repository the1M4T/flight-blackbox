# Flight Blackbox – Rejestrator Telemetrii Pokładowej

Moduł w języku C++ realizujący pakowanie bitowe danych telemetrycznych do pojedynczego słowa 32-bitowego (`uint32_t`), wyliczanie sumy kontrolnej oraz zapis historii w statycznym buforze kołowym.

## 1. Struktura projektu

```text
flight-blackbox/
├── include/
│   └── TelemetryBlackBox.h    # Deklaracja klasy oraz interfejs API
├── src/
│   └── TelemetryBlackBox.cpp  # Implementacja logiki bitowej i bufora
├── tests/
│   └── test_gtest.cpp         # Testy jednostkowe GoogleTest (GTest)
└── README.md                  # Dokumentacja projektu
```

## 2. Opis plików

* **`include/TelemetryBlackBox.h`** – definiuje publiczny interfejs modułu, typ wyliczeniowy faz lotu oraz prywatne składowe klasy wraz ze statyczną tablicą bufora (brak dynamicznej alokacji pamięci).
* **`src/TelemetryBlackBox.cpp`** – zawiera implementację metod aktualizujących stan, kodowanie ramki 32-bitowej z sumą kontrolną oraz weryfikację i dekodowanie ostatniego wpisu na standardowe wyjście.
* **`tests/test_gtest.cpp`** – zestaw 4 testów jednostkowych opartych o framework **GoogleTest (GTest)**:
  1. `EmptyBuffer` – weryfikacja zachowania przy próbie odczytu z pustego bufora.
  2. `DisarmAndLandedStatesForceZeroAltitude` – wymuszenie zerowej wysokości w stanach początkowym i końcowym niezależnie od wskazań czujnika.
  3. `BaroErrorForcesMaxAltitude` – nadrzędny priorytet flagi błędu czujnika wymuszający maksymalną wartość pola wysokości (`0xFFFF`).
  4. `CircularBufferOverwrite` – sprawdzenie poprawności nadpisywania najstarszych danych po przekroczeniu pojemności bufora.

## 3. Układ bitowy ramki (`uint32_t`)

| Bity | Pole | Zakres | Opis |
| :--- | :--- | :--- | :--- |
| **0–2** | `state` | `0..4` | Faza lotu (0..4) |
| **3** | `parachute` | `0..1` | Status spadochronu |
| **4** | `baro_error` | `0..1` | Status błędu czujnika ciśnienia |
| **5–7** | `reserved` | `0` | Bity zarezerwowane |
| **8–23** | `altitude` | `0..65535` | Wysokość |
| **24–31** | `checksum` | `0x00..0xFF` | Suma kontrolna pierwszych trzech bajtów |

## 4. Kompilacja i uruchomienie (Arch Linux)

Do zbudowania i uruchomienia testów wymagany jest kompilator `g++` (wspierający C++17) oraz pakiet `gtest` (`sudo pacman -S gtest`).

1. **Kompilacja projektu z testami:**
   ```bash
   g++ -std=c++17 -Iinclude src/TelemetryBlackBox.cpp tests/test_gtest.cpp -lgtest -lgtest_main -pthread -o run_gtests
   ```

2. **Uruchomienie testów:**
   ```bash
   ./run_gtests
   ```