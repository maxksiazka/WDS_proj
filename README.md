# Wizualizacja kokpitu samolotu 

## O projekcie
Projekt polega na zaprojektowaniu i implementacji aplikacji graficznej mimikującej nowoczesny "szklany kokpit" samolotu.

## Warstwa sprzętowa (Embedded i Czujniki)
Projekt obejmuje warstwę embedded, w której centralną funkcję pełni mikrokontroler **Raspberry Pi Pico 2W**. Do mikrokontrolera podłączony jest zestaw następujących układów pomiarowych:
* **Moduł inercyjny (IMU 10-DoF)** - zawierający akcelerometr, żyroskop, magnetometr oraz barometr statyczny. - *modGY-86*:
  * **Akcelerometr i żyroskop**: Służą do wyznaczania sztucznego horyzontu (ang. *attitude indicator*).
  * **Magnetometr**: Umożliwia sterowanie wirtualnym kompasem (ang. *heading indicator*) oraz orientowanie wskaźnika kursu.
  * **Barometr statyczny**: Służy do przeliczania spadku ciśnienia w celu określenia wysokości lotu oraz uzyskania prędkości pionowej, pełniąc funkcję wariometru.
* **System pomiaru ciśnienia różnicowego** - *Hx-710B*: Działa jako prędkościomierz ciśnieniowy, mający na celu uzyskanie prędkości przyrządowej statku powietrznego (ang. *indicated airspeed* - IAS).
* **Moduł GPS** - *NEO-M8N*: Wykorzystywany do reprezentacji prędkości nad ziemią (ang. *ground speed* - GS) oraz rzeczywistego kąta ścieżki (ang. *course over ground* - COG).

## Architektura oprogramowania
* Architektura systemu opiera się na ciągłej akwizycji wartości z wymienionych układów pomiarowych, ich wstępnym przetwarzaniu na poziomie mikrokontrolera oraz strumieniowaniu do komputera PC.
* Algorytmy te integrują surowe odczyty, tworząc spójny, odporny na zakłócenia i responsywny interfejs łączący funkcjonalności ekranów PFD (ang. *Primary Flight Display*) oraz MFD (ang. *Multi-Function Display*).
* Intefejs będzie osobną aplikacją desktopową, która będzie odbierać dane z mikrokontrolera i wizualizować je w czasie rzeczywistym za pomocą biblioteki graficznej Qt6.

## Specyfikacja finalnego produktu
* Finalnym produktem jest zintegrowany system pomiarowo-wizualizacyjny, który ma na celu imitację samolotu.
* Część sprzętowa, mająca formę fizycznej makiety, pełni funkcję uproszczonego układu AHRS (ang. *Attitude and Heading Reference System*) zintegrowanego z czujnikiem ciśnienia różnicowego (pomiar prędkości przyrządowej) oraz odbiornikiem GPS.
* Zbierane dane są strumieniowane do aplikacji na komputerze PC, która realizuje funkcje interfejsów PFD oraz MFD.
* Gotowa aplikacja bezpośrednio reaguje na zmiany położenia makiety oraz zmiany środowiskowe. **Data**: 20 marca 2026.

