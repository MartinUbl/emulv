# Modulární emulátor platformy RISC-V

## Původní požadavky zadání

* Původně emulátor RISC-V určen pro předmět KIV/OS
* Cílem je emulace základní RV ISA, a ovládání pomocí GUI (Tok instrukcí - disassembly, obsah registrů, memory view, debugging - brekpointy, krokování)
* Emulátor musí zahrnovat možnost připojit periférie (V GUI realizované pomocí podokna)
* Periférie by měly kopírovat hardware MC [*Sipeed Longan Nano GD32VF103CBT6*](https://www.seeedstudio.com/Sipeed-Longan-Nano-V1-1-p-5118.html)
* Výsledný frontend by měl být multiplatformní

## Aktuální TODO
### Hlavní
1) ~~Pokrýt "kritické" sekce kodu logovacími výpisy~~
2) Přepracování architektury aplikace, úprava neefektivních algoritmů
3) Přepracování a vylepšení GUI
4) Přepracování a vylepšení UART / GPIO periférií
5) Lepší pokrytí aplikace unit testy
6) Risc-v kompilátor?
7) ?

### Vedlejší
1) CMake install script pro Linux
2) Správné použití namespace
3) Modernizace kodu - přidání smart pointerů, apod.
4) Pragma v header souborech 
5) Na windows z neznámého důvodu nejde vypisovat nic na STDOUT, pokud není program spuštěn v admin režimu, nebo s připojeným debuggerem

## Changelog projektu
* Byly odstraněny temporary / nepotřebné soubory
  * Tímto krokem byla zlepšena přehlednost souborů projektu, zároveň nebylo nic ztraceno, všechny smazané soubory se nachází v původním gitlab repozitáři
* Předelána CMake struktura projektu
  * upravena adresářová struktura; CMake soubory jsou nyní správně dekomponovány na jednotlivé targety, díky tomu je build process nyní spolehlivější a přehlednější; využito bylo také CMake modulů, které zabraňují vznik duplicitního kodu a zpřehledňují CMake soubory (využitím include() příkazů)
* Přidána Doxygen dokumentace
  * Doxygen dokumentaci je nyní možné vygenerovat pomocí CMake targetu "doxygen", CMake navíc stáhne a aplikuje "doxygen awesome CSS" knihovnu
  * Po nainstalování GraphViz a Doxygen na počítači kde je prováděna kompilace se dokumentace vygeneruje včetně UML dependency, class grafů, apod.
* Integrace GoogleTest frameworku do CMake
  * Rízení GoogleTest unit testů je nyní integrováno do CMake souboru tohoto projektu
* Přidání CMakePresets.json 
  * Byl přidán konfigurační soubor pro parametry CMake; Tento soubor nahrazuje ruční zadávání parametrů příkazové řádky, lze tak jednoduše zvolit například Release-Windows preset a CMake parametry budou automaticky nakonfigurovány podle tohoto json souboru
  * Je možné, že na jiném počítači bude vždy potřeba některé části tohoto souboru přepsat (konkrétně polohu adresáře Qt na windows), stále je to ale mnohem lepší a jednoduché řešení než soubor neposkytovat vůbec
* Přidán logging framework
  * Byl přidán logovací framework Spdlog, tímto krokem je ulehčeno debuggování a sledování stavu aplikace
* CMake instalační skripty
  * Na OS Windows je nyní možno pomocí CMake vygenerovat grafický NSIS instalátor, Linux zatím podporován není, ale neměl by být problém ho přidat
* Docker build skript
  * Byl přidán Docker build skript, jehož cílem byla jednoduchá kompilace Release verze aplikace na jakémkoliv počítači; v aktuální podobě je tento skript schopen úspěšně zkompilovat "Hello world" Qt aplikaci pro Linux, MacOs (x86-64 a ARM) a Windows (x86-64); Kompilace tohoto projektu ale při aktuálním nastavení selhává, kvůli různým specifickým problémům při kompilaci a linkování, tyto problémy by bylo do budoucna možné odstranit metodou "pokus-omyl" (předpokládaná časová náročnost 2 až 3 dny práce)
  * Velký benefit by mohl být WebAssembly build této aplikace, který by umožnil spuštění v jakémkoliv kompatibilním webovém prohlížeci na jakékoliv platformě. Teoreticky nic nebrání tomu aby tento build byl proveden. Pokus bylo o použití prostředí Emscripten, které je založeno na kompilátoru LLVM/Clang, ukázalo se, že je třeba využít speciální verze Qt6 které je zkompilováno pro multithreading (Emscripten - Clang vyžaduje následující: pokud je jeden soubor zkompilován s -pthread přepínačem, tak tímto způsobem musí být zkompilovány všechny soubory včetně knihoven, jinak nastane error při linkování), tento problém byl vyřešen použítím modifikované Qt6 webassembly docker image; Následně bylo možné úspěšně zkompilovat "hello world" Qt projekt, opět ale nastal problém s kompilací tohoto projektu, proces linkování vždy selhal s chybou undefined symbol "getrandom", tuto chybu se nepodařilo odstranit, zřejmě je způsobena nekompatibilitou Libriscv knihovny a aktuální verze Emscripten


## Dosavadní SW dependence projektu nutné ke kompilaci

* Programovací jazyk: C++
* Programová dokumentace: Doxygen + GraphViz
  * Pro generování dokumentace je potřeba oba nainstalovat na systému kde se provádí kompilace
* Knihovna pro tvorbu GUI: Qt6
  * Je potřeba nainstalovat na systému kde se prování kompilace, poté případně v CMakePresets.json upravit cestu k adresáři instalace Qt
* Následující knihovny jsou automaticky staženy pomocí CMake:
  * libriscv knihovna pro emulaci ISA: https://github.com/fwsGonzo/libriscv
  * doxygen-awesome-css: https://github.com/jothepro/doxygen-awesome-css
  * JSON for Modern C++: https://github.com/nlohmann/json
  * riscv-disassembler: https://github.com/michaeljclark/riscv-disassembler
  * spdlog: https://github.com/gabime/spdlog
  * GoogleTest: https://github.com/google/googletest


