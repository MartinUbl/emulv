# Modulární emulátor platformy RISC-V pro výukové účely (BP 2023/24)

# Zadání

Bakalářská práce v oborech Informatika (Bc), Výpočetní technika (Bc), Informační systémy (Bc).

### Modulární emulátor platformy RISC-V pro výukové účely

1. Seznamte se s architekturou RISC-V a nejrozšířenějšími implementacemi její instrukční sady
2. Analyzujte dostupná řešení pro emulaci a ladění programů pro tuto architekturu
3. Navrhněte nástroj, který umožní emulovat platformu RISC-V s možností definovat a konfigurovat periferie odděleně od jeho jádra a provádět
   úkony související s laděním
4. Implementujte tento nástroj jako modulární, umožněte vytvářet periferie definované v dynamicky linkovaných knihovnách
5. Otestujte řešení na sadě standardních úloh a zhodnoťte dosažené výsledky

| Téma vypsal                | Vypsáno pro akademický rok | Zadáno komu |
|----------------------------|----------------------------|-------------|
| *Ing. Martin Úbl* (UN 332) | 2023/2024 (dne 2023-04-11) | Jonáš Dufek |

# Dosavadní stav

### 26.8.2023 - 21.9.2023

---

* odstraněny dočasné / nevyužité soubory
* Upravena adresářová struktura repozitáře (zpřehlednění)
* Původní CMake soubor byl odstraněn, nahrazen CMakeList soubory v každé složce, byly využity CMake moduly, jednotlivé logické části
  aplikace jsou nyní vzájemně nezávislé (každá část má svůj "target")
* Přidán Doxygen pro generování dokumentace - programovou dokumentaci lze nyní automaticky vygenerovat pomocí CMake
* GoogleTest je nyní integrován do hlavního CMakeList souboru, unit testy lze nyní spustit pomocí CMake)
* Původní unit testy byly upraveny, některé nové byly přidány
* Přidán CMakePresets.json (snadnější nastavení parametrů CMake jako adresáře Qt)
* Přidán install.cmake script, lze pomocí něj vygenerovat NSIS instalátor pro Windows
* Přidán Spdlog logging framework, aplikace je pokryta logovacími výpisy
* Pokus o build aplikace pro Windows / MacOs / Linux pomocí docker scritpu
* EventEmitter byl vylepšen, nyní není třeba pro každý typ eventu vytvořit samostatnou třídu, využito std::any
* Rozhraní mezi GUI a zbytkem programu bylo odděleno do samostatného souboru EmulvApi.h
* Další drobné změny

### 19.2.2024 - ?

---

* Příprava nového GUI
*

# Závislosti projektu

### Knihovny nainstalové v OS potřebné pro kompilaci

| Název              | Účel                    |
|--------------------|-------------------------|
| Doxygen + GraphViz | Generování dokumentace  |
| Qt6                | Knihovna pro tvorbu GUI |

### Následující knihovny jsou automaticky staženy pomocí CMake:

| Název               | Odkaz                                                                                                      |
|---------------------|------------------------------------------------------------------------------------------------------------|
| libriscv            | [https://github.com/fwsGonzo/libriscv](https://github.com/fwsGonzo/libriscv)                               |
| doxygen-awesome-css | [https://github.com/jothepro/doxygen-awesome-css](https://github.com/jothepro/doxygen-awesome-css)         |
| JSON for Modern C++ | [https://github.com/nlohmann/json](https://github.com/nlohmann/json)                                       |
| riscv-disassembler  | [https://github.com/michaeljclark/riscv-disassembler](https://github.com/michaeljclark/riscv-disassembler) |
| spdlog              | [https://github.com/gabime/spdlog](https://github.com/gabime/spdlog)                                       |
| GoogleTest          | [https://github.com/google/googletest](https://github.com/google/googletest)                               |

