# Do zrobienia:
### Warstwa sprzętowa i Embedded (Raspberry Pi Pico)
- [ ] Przygotowanie środowiska programistycznego dla mikrokontrolera Raspberry Pi Pico.
- [ ] Podłączenie i podstawowa konfiguracja modułu inercyjnego IMU 10-DoF.
- [ ] Odczyt danych z akcelerometru i żyroskopu.
- [ ] Odczyt danych z magnetometru.
- [ ] Odczyt danych z barometru statycznego.
- [ ] Podłączenie i konfiguracja systemu pomiaru ciśnienia różnicowego.
- [ ] Podłączenie i odczyt danych z modułu GPS.
- [ ] Implementacja ciągłej akwizycji i wstępnego przetwarzania zebranych danych na poziomie mikrokontrolera.
- [ ] Uruchomienie strumieniowania zebranych danych z Raspberry Pi Pico do komputera PC (np. przez UART/USB).

### Warstwa programowa PC i Przetwarzanie Danych
- [ ] Inicjalizacja projektu aplikacji graficznej na PC.
- [ ] Napisanie modułu odpowiedzialnego za odbiór i parsowanie strumienia danych z mikrokontrolera.
- [ ] Implementacja algorytmów fuzji danych w celu integracji surowych odczytów z czujników i odfiltrowania zakłóceń.

### Wizualizacja Kokpitu (Interfejs Użytkownika)
- [ ] Zaprojektowanie i stworzenie głównego układu graficznego dla interfejsów PFD (Primary Flight Display) oraz MFD (Multi-Function Display).
- [ ] Implementacja sztucznego horyzontu (reagującego na dane z akcelerometru i żyroskopu).
- [ ] Implementacja wirtualnego kompasu i wskaźnika kursu (na bazie danych z magnetometru).
- [ ] Implementacja wysokościomierza oraz wariometru (wykorzystujących spadek ciśnienia z barometru).
- [ ] Implementacja prędkościomierza IAS (na podstawie danych z czujnika ciśnienia różnicowego).
- [ ] Wyświetlanie prędkości nad ziemią (GS) oraz rzeczywistego kąta ścieżki (COG) z modułu GPS.

### Integracja i Testy
- [ ] Zbudowanie fizycznej makiety samolotu integrującej mikrokontroler i wszystkie układy pomiarowe.
- [ ] Kalibracja sensorów (w szczególności magnetometru i IMU) w docelowym środowisku.
- [ ] Testowanie responsywności aplikacji PC na fizyczne zmiany położenia makiety
---

Należy pamiętać, że powyższa lista zadań jest orientacyjna i może ulec zmianie w trakcie realizacji projektu. 
Każde zadanie powinno być dokładnie przemyślane i zaplanowane, aby zapewnić płynny przebieg prac oraz osiągnięcie zamierzonych celów projektu.
Dana lista nie jest wyczerpująca i może być rozszerzana o dodatkowe funkcjonalności lub poprawki w miarę postępu prac.
Nie stanowi też ona roadmapy, kolejności realizacji poszczególnych zadań, a jedynie zbiór punktów do zrealizowania w ramach projektu.
