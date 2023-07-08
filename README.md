# Modulární emulátor platformy RISC-V
## Původní zadání:

Pro potřeby výuky vznikla potřeba mít emulátor hardwarové platformy RISC-V. V
předmětu KIV/OS momentálně používáme hardware založený na ARM procesorech,
nicméně budoucnost vidíme v použití platformy RISC-V. Jde o open-source
platformu, která je důkladně zdokumentovaná a existuje k ní spousty materiálů jak ve
formě výukových prostředků, tak např. již hotových knihoven pro emulaci instrukční
sady.

V rámci tohoto zadání byste vytvořili emulátor platformy RISC-V (základní podoby
jeho ISA), který by zahrnoval i uživatelské rozhraní. V tomto rozhraní by byl vidět
aktuální tok instrukcí, obsah registrů a byla by možnost prohlížet i emulovanou
paměť. Mimo to by bylo velice vhodné umožnit vkládat do kódu breakpointy a kód
umět krokovat. Emulátor by měl být i dostatečně modulární - můžeme například chtít
“připojit” modul na určité emulované piny procesoru (např. LED, spínač/přepínač, v
budoucnu i displej nebo jiné složitější periferie), přičemž emulátor by pro každý z
těchto modulů uměl otevřít podokno s emulovanou, potenciálně ovladatelnou
podobou této periferie. Emulátor by měl být multiplatformní (v současnosti postačuje
Debian GNU/Linux a MS Windows 10). Preferovaným, nicméně ne vyžadovaným
implementačním jazykem je C++.


## Analýza problému a informace o projektu


### Stručné zadání
Cílem je vytvořit emulátor platformy RISC-V včetně grafického rozhraní.
Grafické rozhraní by mělo zahrnovat jednoduchý debugger. K implementaci backendu
(emulace instrukcí) zadavatel preferuje využití některé již existující knihovny. 
Emulátor by měl emulovat i některé periférie, v tomto případě by emulovaná
periferní zařízení měla fungovat přesně tak, jako na mikrokontroleru
[*Sipeed Longan Nano GD32VF103CBT6*](https://www.seeedstudio.com/Sipeed-Longan-Nano-V1-1-p-5118.html).

### Požadovaná funkcionalita
* **Grafické rozhraní (frontend)**
  * Možnost importování souboru se strojovým kódem
  * Prostor pro zobrazení instrukcí programu (disassembly)
  * Tlačítka debuggeru
  * Zobrazení registrů (včetně možnosti přepnutí mezi Hex / Decimal)
  * Zobrazení paměti
  * Prostor pro zobrazení periferií (Buď nějaká samostatná okna, nebo "tabView" se záložkami)
  * Záhlaví okna včetně položek File, Edit, Help, Settings, ...
  * V Settings **možnost editace konfiguračního souboru** 
    * Pro konfigurační soubor zřejmě použít JSON nebo něco podobného
    * Konfigurační soubor bude obsahovat informace o periferiích apod.
  * Případná další rozšíření:
    * File explorer
    * ~~Možnost přepínat mezi více soubory~~
    * Terminal podokno
    * ~~Run podokno~~
    

* **Interface (komunikační rozhraní mezi GUI a Emulátorem instrukcí)**
  * Načtení programu
  * Nástroj pro sledování registrů
  * Nástroj pro čtení paměti
  * Komunikace periferií
  * Zajištění komunikace debuggeru (?)
  * ~~Výstupy programu (?)~~
  

* **Emulace instrukcí (backend)**
  * Inicializace emulátoru
  * Zajistit dekódování a provedení instrukcí (pomocí knihovny)
  * Mapování periferií a jejich správná funkčnost
    * GPIO piny
    * UART
    * ~~Případně další:~~
      * ~~Timer~~
      * ~~ADC~~
      * ~~DMA controller (?)~~

### Aktuální stav projektu

| **Kategorie** | **Úkol**                              | **Stav** |
|---------------|---------------------------------------|----------|
| GUI           | GUI navrhnuto                         | OK       |
| GUI           | GUI plně implementováno               | OK       |
| GUI           | Konfigurační soubor                   | OK       |
| GUI           | Implementovány reakce na události GUI | OK       |
| Interface     | Implementována základní funkcionalita | OK       |
| Interface     | Kompletně implementováno              | ?        |
| Emulátor      | Inicializace emulátoru                | OK       |
| Emulátor      | Dekódování a provedení instrukcí      | OK       |
| Emulátor      | GPIO                                  | ?        |
| Emulátor      | UART                                  | ?        |

### Použité technologie a knihovny

* Programovací jazyk: C++
* ~~Programová dokumentace: Doxygen~~
* Knihovna pro tvorbu GUI: Qt6
* Knihovna pro emulaci ISA: [libriscv](https://github.com/fwsGonzo/libriscv)

